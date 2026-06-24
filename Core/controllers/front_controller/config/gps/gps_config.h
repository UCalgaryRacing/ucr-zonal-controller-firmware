#ifndef CONFIG_GPS_CONFIG_H_
#define CONFIG_GPS_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "gps_typ_common.h"
#include "gps_config_pinout.h"

/*============================================================================*/
/* System Topology                                                            */
/*============================================================================*/

/* NovAtel OEM7600 connected via UART3, DMA circular RX, IDLE line detection */

/*============================================================================*/
/* UART Configuration                                                         */
/*============================================================================*/

/* Blocking TX timeout used for probe and configuration command transmission */
#define GPS_UART_TX_TIMEOUT_MS      50U

/*============================================================================*/
/* DMA / Ring Buffer Sizing                                                   */
/*============================================================================*/

/* Circular DMA receive buffer.  Must be large enough that the DMA does not
 * wrap during the worst-case burst between IDLE callbacks. */
#define GPS_DMA_BUFFER_SIZE         512U

/* Software ring buffer consumed by the service layer.
 * Must be a power of 2 and >= GPS_DMA_BUFFER_SIZE.  Sized for ~400 ms of
 * worst-case ingress (BESTGNSSPOS + INSPVAS + RAWIMUSX combined) so brief
 * task jitter does not overflow before the next drain. */
#define GPS_RING_BUFFER_SIZE        4096U

/*============================================================================*/
/* OEM7 Binary Log Parser Configuration                                      */
/*============================================================================*/

/* Sync bytes shared by all OEM7 binary message formats */
#define GPS_OEM7_SYNC_1             0xAAU
#define GPS_OEM7_SYNC_2             0x44U

/* Sync3 selects the header format:
 *   0x12 — Full binary header  (technically variable length?? Seems like 28 bytes)
 *   0x13 — Short binary header (12 bytes, fixed) */
#define GPS_OEM7_SYNC_3_LONG        0x12U
#define GPS_OEM7_SYNC_3_SHORT       0x13U

/* OEM7 header lengths in bytes (including the 3 sync bytes) */
#define GPS_OEM7_LONG_HDR_LEN       28U
#define GPS_OEM7_SHORT_HDR_LEN      12U

/* CRC-32 field appended after the message body (same for both formats) */
#define GPS_OEM7_CRC_LEN            4U

/* CRC-32 polynomial used by OEM7 (ITU-T V.42 / IEEE 802.3, reflected form).
 * Initial value: 0x00000000.  No final XOR. */
#define GPS_OEM7_CRC_POLY           0xEDB88320L

/* Maximum binary log body length the parser will buffer before discarding */
#define GPS_MAX_LOG_BODY_LEN        256U

/* OEM7 binary message IDs (add as logs are enabled) */
#define GPS_OEM7_BESTGNSSPOS_ID     1429U
#define GPS_OEM7_RAWIMUSX_ID        1462U
#define GPS_OEM7_INSPVAS_ID         508U

/*============================================================================*/
/* Probe / Configuration Timing                                               */
/*============================================================================*/

/* \r\n sent every N task ticks (10 ms each) while waiting for the receiver
 * to respond.  500 ms interval prevents flooding the RX buffer. */
#define GPS_PROBE_INTERVAL_TICKS    50U

/* Give up probing after this many attempts (~10 seconds) */
#define GPS_PROBE_MAX_ATTEMPTS      20U

/* Ticks to wait after the final config command response before switching to
 * the RUNNING state, allowing late-arriving bytes to be drained. */
#define GPS_CONFIG_SETTLE_TICKS     20U

/* Per-command response timeout (ticks).  If the receiver has not produced a
 * "<OK" acknowledgement within this window, the service transitions to
 * FAULT so the rest of the task does not stall. */
#define GPS_CONFIG_RESPONSE_TIMEOUT_TICKS   20U

/* Maximum bytes drained from the ring buffer per service update.
 * Limits worst-case time spent in gps_svc_update per 10 ms tick. */
#define GPS_SVC_READ_CHUNK_SIZE     128U

/*============================================================================*/
/* Log Configuration                                                          */
/*============================================================================*/

/**
 * Describes a single OEM7 output log to be enabled during startup.
 *
 * @field command  Full ASCII command string sent to the receiver, including
 *                 the trailing \r\n.  Example:
 *                   "LOG THISPORT BESTPOSB ONTIME 0.1\r\n"
 *                 The array is sentinel-terminated: the last entry has
 *                 command == NULL.
 *
 * @field msg_id   OEM7 binary message ID of the log enabled by this command.
 *                 Used for cross-referencing with the parser dispatch table.
 */
typedef struct
{
    const char *command;
    uint16_t    msg_id;
} gps_oem7_log_config_t;

/* Sentinel-terminated list of one-shot setup commands.
 * Sent before LOG subscriptions, one per tick, with response check. */
extern const char * const gps_oem7_setup_commands[];

/* Sentinel-terminated array of logs to configure on startup.
 * Defined in gps_config.c; iterate until command == NULL. */
extern const gps_oem7_log_config_t gps_oem7_log_configs[];

/*============================================================================*/
/* Hardware Descriptor                                                        */
/*============================================================================*/

typedef struct
{
    UART_HandleTypeDef *uart_handle;
    DMA_HandleTypeDef  *dma_handle;
} gps_hw_t;

extern const gps_hw_t gps_hw_config;

#endif /* CONFIG_GPS_CONFIG_H_ */
