#include "gps_drv_oem7.h"
#include <string.h>

/*============================================================================*/
/* Private Types                                                              */
/*============================================================================*/

/* GPS_RING_BUFFER_SIZE must be a power of 2 and >= GPS_DMA_BUFFER_SIZE.
 * Defined in gps_config.h. */
#define GPS_RING_BUFFER_MASK  (GPS_RING_BUFFER_SIZE - 1U)

typedef struct
{
    uint8_t           buf[GPS_RING_BUFFER_SIZE];
    volatile uint32_t head;  /* Written by ISR; read by service layer        */
    uint32_t          tail;  /* Read and written exclusively by service layer */
} gps_ring_buffer_t;

/*============================================================================*/
/* Private State                                                              */
/*============================================================================*/

/* Circular DMA receive buffer.  Written exclusively by DMA hardware. */
static uint8_t g_dma_rx_buf[GPS_DMA_BUFFER_SIZE];

/* Last known DMA write position (absolute offset into g_dma_rx_buf).
 * Accessed only from the ISR callback — no synchronisation needed. */
static uint32_t g_dma_last_pos = 0U;

/* Software ring buffer consumed by the service layer. */
static gps_ring_buffer_t g_ring_buf;

/* Cached hardware descriptor set during init. */
static const gps_hw_t *g_hw = NULL;

static bool g_initialized = false;

/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/

/**
 * @brief  Push len bytes from src into the ring buffer.
 *
 *         Called exclusively from the ISR callback.  If the ring buffer is
 *         full, incoming bytes are silently dropped to prevent the head pointer
 *         from lapping the tail pointer (which would corrupt the tail reader's
 *         view of available bytes).
 */
static void ring_buf_push(gps_ring_buffer_t *rb, const uint8_t *src, uint32_t len)
{
    uint32_t space = GPS_RING_BUFFER_SIZE - (rb->head - rb->tail);
    if (len > space)
    {
        len = space;
    }

    for (uint32_t i = 0U; i < len; i++)
    {
        rb->buf[rb->head & GPS_RING_BUFFER_MASK] = src[i];
        rb->head++;
    }
}

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

gps_status_t gps_drv_oem7_init(const gps_hw_t *hw)
{
    if (hw == NULL || hw->uart_handle == NULL || hw->dma_handle == NULL)
    {
        return GPS_ERROR_INVALID_PARAM;
    }

    /* Release the OEM7600 from reset.  CubeMX initialises PA6 as a
     * push-pull output with ODR=0, holding nRESET asserted; the receiver
     * must be out of reset before any UART probe or command is sent. */
    HAL_GPIO_WritePin(GPS_OEM7_NRESET_PORT, GPS_OEM7_NRESET_PIN, GPIO_PIN_SET);

    g_hw            = hw;
    g_dma_last_pos  = 0U;
    g_ring_buf.head = 0U;
    g_ring_buf.tail = 0U;
    g_initialized   = true;

    return GPS_OK;
}

gps_status_t gps_drv_oem7_start(const gps_hw_t *hw)
{
    if (!g_initialized || hw == NULL)
    {
        return GPS_ERROR_NOT_INITIALIZED;
    }

    /* Start circular DMA reception with IDLE line detection.
     *
     * HAL_UARTEx_ReceiveToIdle_DMA enables the IDLE line interrupt and
     * fires HAL_UARTEx_RxEventCallback on:
     *   - DMA half-transfer  (Size = GPS_DMA_BUFFER_SIZE / 2)
     *   - DMA full-transfer  (Size = GPS_DMA_BUFFER_SIZE)
     *   - UART IDLE line     (Size = current write position)
     *
     * The DMA stream must be configured in circular mode in CubeMX so that
     * reception restarts automatically after each full-transfer event. */
    if (HAL_UARTEx_ReceiveToIdle_DMA(hw->uart_handle,
                                     g_dma_rx_buf,
                                     GPS_DMA_BUFFER_SIZE) != HAL_OK)
    {
        return GPS_ERROR_COMMUNICATION;
    }

    /* Suppress the half-transfer interrupt.  We only need IDLE and full-
     * transfer events — half-transfer fires at a fixed DMA position and
     * could split a single OEM7 binary log across two callbacks, making
     * the wrap-around logic in gps_drv_oem7_rx_event_callback unnecessarily
     * complicated. */
    __HAL_DMA_DISABLE_IT(hw->dma_handle, DMA_IT_HT);

    return GPS_OK;
}

/*============================================================================*/
/* Command Transmit                                                           */
/*============================================================================*/

gps_status_t gps_drv_oem7_send_command(const char *cmd)
{
    if (!g_initialized || g_hw == NULL)
    {
        return GPS_ERROR_NOT_INITIALIZED;
    }

    if (cmd == NULL)
    {
        return GPS_ERROR_INVALID_PARAM;
    }

    uint16_t len = (uint16_t)strlen(cmd);
    if (len == 0U)
    {
        return GPS_OK;
    }

    if (HAL_UART_Transmit(g_hw->uart_handle,
                          (const uint8_t *)cmd,
                          len,
                          GPS_UART_TX_TIMEOUT_MS) != HAL_OK)
    {
        return GPS_ERROR_COMMUNICATION;
    }

    return GPS_OK;
}

/*============================================================================*/
/* ISR Entry Point                                                            */
/*============================================================================*/

void gps_drv_oem7_rx_event_callback(UART_HandleTypeDef *huart, uint16_t size)
{
    if (!g_initialized || g_hw == NULL || huart->Instance != g_hw->uart_handle->Instance)
    {
        return;
    }

    uint32_t cur_pos = (uint32_t)size % GPS_DMA_BUFFER_SIZE;

    if (size > g_dma_last_pos)
    {
        /* Common case: DMA has advanced without wrapping. */
        ring_buf_push(&g_ring_buf,
                      &g_dma_rx_buf[g_dma_last_pos],
                      (uint32_t)size - g_dma_last_pos);
    }
    else if (size < g_dma_last_pos)
    {
        /* DMA buffer wrapped: copy tail of old cycle then head of new. */
        ring_buf_push(&g_ring_buf,
                      &g_dma_rx_buf[g_dma_last_pos],
                      GPS_DMA_BUFFER_SIZE - g_dma_last_pos);

        if (cur_pos > 0U)
        {
            ring_buf_push(&g_ring_buf, &g_dma_rx_buf[0U], cur_pos);
        }
    }
    /* size == g_dma_last_pos: no new bytes (should not occur in practice). */

    g_dma_last_pos = cur_pos;
}

/*============================================================================*/
/* Ring Buffer Access (service layer)                                        */
/*============================================================================*/

uint32_t gps_drv_oem7_bytes_available(void)
{
    /* head is volatile; the subtraction gives a snapshot of unread bytes.
     * Safe without a critical section on Cortex-M because both head and tail
     * are 32-bit aligned and therefore read/written atomically. */
    return g_ring_buf.head - g_ring_buf.tail;
}

uint32_t gps_drv_oem7_read(uint8_t *dst, uint32_t len)
{
    if (dst == NULL || len == 0U)
    {
        return 0U;
    }

    uint32_t available = g_ring_buf.head - g_ring_buf.tail;
    uint32_t to_read   = (len < available) ? len : available;

    for (uint32_t i = 0U; i < to_read; i++)
    {
        dst[i] = g_ring_buf.buf[g_ring_buf.tail & GPS_RING_BUFFER_MASK];
        g_ring_buf.tail++;
    }

    return to_read;
}

/*============================================================================*/
/* HAL Callback Override                                                      */
/*============================================================================*/

/**
 * @brief  Overrides the weak HAL default.  Called on HALF transfer,
 *         FULL transfer, and IDLE line events.
 *
 *         If other UART peripherals in this project require RxEventCallback
 *         handling, add their dispatch calls below alongside the OEM7 call.
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    gps_drv_oem7_rx_event_callback(huart, Size);
}
