#ifndef SERVICE_GPS_SVC_H_
#define SERVICE_GPS_SVC_H_

#include <stdint.h>
#include <stdbool.h>
#include "gps_typ_common.h"
#include "gps_config.h"

/*============================================================================*/
/* Service State Machine                                                      */
/*============================================================================*/

typedef enum
{
    GPS_SVC_STATE_PROBING = 0, /* Sending CR/LF probes; waiting for any RX response */
    GPS_SVC_STATE_CONFIGURING, /* Sending log configuration commands                */
    GPS_SVC_STATE_RUNNING,     /* Parsing binary logs from the ring buffer           */
    GPS_SVC_STATE_FAULT,       /* Unrecoverable error (e.g. probe timeout)           */
} gps_svc_state_t;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

/**
 * @brief  Initialise the GPS service and underlying UART driver.
 *         Must be called once before gps_svc_start().
 */
gps_status_t gps_svc_init(void);

/**
 * @brief  Start circular DMA reception.
 *         Must be called after gps_svc_init() and after the HAL UART
 *         peripheral has been initialised by MX_USART3_UART_Init().
 */
gps_status_t gps_svc_start(void);

/*============================================================================*/
/* Periodic Update (call from task_fast every 10 ms)                         */
/*============================================================================*/

/**
 * @brief  Step the GPS service state machine.
 *
 *         - PROBING:     Sends a CR/LF probe every GPS_PROBE_INTERVAL_TICKS
 *                        ticks and watches for any bytes in the ring buffer.
 *                        On first response, transitions to CONFIGURING.
 *         - CONFIGURING: Sends binary log configuration commands, then waits
 *                        GPS_CONFIG_SETTLE_TICKS before transitioning to
 *                        RUNNING.
 *         - RUNNING:     Drains up to GPS_SVC_READ_CHUNK_SIZE bytes from the
 *                        ring buffer and feeds them through the binary frame
 *                        parser.
 *
 * @return GPS_OK in normal operation, GPS_ERROR_TIMEOUT if probing fails.
 */
gps_status_t gps_svc_update(void);

/*============================================================================*/
/* State Access                                                               */
/*============================================================================*/

/**
 * @brief  Return the current service state machine state.
 */
gps_svc_state_t gps_svc_get_state(void);

#endif /* SERVICE_GPS_SVC_H_ */
