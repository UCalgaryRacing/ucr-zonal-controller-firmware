#ifndef DRIVER_GPS_DRV_OEM7_H_
#define DRIVER_GPS_DRV_OEM7_H_

#include <stdint.h>
#include <stdbool.h>
#include "gps_typ_common.h"
#include "gps_config.h"
#include "stm32h7xx_hal.h"

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

/**
 * @brief  Initialise the OEM7 UART driver internal state.
 *         Must be called before gps_drv_oem7_start().
 *
 * @param  hw  Pointer to the hardware descriptor (from gps_config).
 * @return GPS_OK on success, GPS_ERROR_INVALID_PARAM if hw is NULL.
 */
gps_status_t gps_drv_oem7_init(const gps_hw_t *hw);

/**
 * @brief  Start circular DMA reception with IDLE line interrupt.
 *         The DMA peripheral must be configured in circular mode in CubeMX.
 *         Half-transfer interrupt is disabled so only IDLE and full-transfer
 *         events populate the ring buffer.
 *
 * @param  hw  Pointer to the hardware descriptor.
 * @return GPS_OK on success, GPS_ERROR_COMMUNICATION if HAL call fails.
 */
gps_status_t gps_drv_oem7_start(const gps_hw_t *hw);

/*============================================================================*/
/* ISR Entry Point                                                            */
/*============================================================================*/

/**
 * @brief  Forward HAL_UARTEx_RxEventCallback events into the driver.
 *
 *         HAL calls HAL_UARTEx_RxEventCallback (a weak symbol overridden in
 *         gps_drv_oem7.c) on HALF transfer, FULL transfer, and IDLE line
 *         events.  That override calls this function.  If the project grows to
 *         include other UART peripherals that also use RxEventCallback, add
 *         their dispatch calls alongside this one inside the override.
 *
 * @param  huart  UART handle from the HAL callback.
 * @param  size   Absolute write position in the DMA buffer (bytes filled so
 *                far in the current DMA cycle, as reported by the HAL).
 */
void gps_drv_oem7_rx_event_callback(UART_HandleTypeDef *huart, uint16_t size);

/*============================================================================*/
/* Command Transmit                                                           */
/*============================================================================*/

/**
 * @brief  Send a null-terminated ASCII command string to the OEM7600.
 *
 *         Wraps HAL_UART_Transmit with a blocking timeout defined in
 *         gps_config.h (GPS_UART_TX_TIMEOUT_MS).  The caller must not include
 *         the null terminator in the string — length is computed internally
 *         via strlen.
 *
 * @param  cmd  Null-terminated ASCII command, e.g. "UNLOGALL\r\n".
 * @return GPS_OK on success, GPS_ERROR_NOT_INITIALIZED if the driver has not
 *         been initialised, GPS_ERROR_COMMUNICATION if the HAL call fails.
 */
gps_status_t gps_drv_oem7_send_command(const char *cmd);

/*============================================================================*/
/* Ring Buffer Access (service layer only)                                   */
/*============================================================================*/

/**
 * @brief  Return the number of unread bytes available in the ring buffer.
 *         Safe to call from the service task; head is declared volatile.
 */
uint32_t gps_drv_oem7_bytes_available(void);

/**
 * @brief  Read up to len bytes from the ring buffer into dst.
 *
 * @param  dst  Destination buffer.
 * @param  len  Maximum number of bytes to read.
 * @return Actual number of bytes copied.
 */
uint32_t gps_drv_oem7_read(uint8_t *dst, uint32_t len);

#endif /* DRIVER_GPS_DRV_OEM7_H_ */
