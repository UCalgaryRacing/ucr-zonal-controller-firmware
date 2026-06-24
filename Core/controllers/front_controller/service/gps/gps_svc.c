#include "gps_svc.h"
#include "gps_drv_oem7.h"
#include "gps_config.h"
#include "ucr_03.h"
#include "can_svc_can.h"
#include <string.h>

/*============================================================================*/
/* Private Types - Binary Frame Parser                                        */
/*============================================================================*/

typedef enum
{
    GPS_PARSE_STATE_SYNC1 = 0, /* Hunting for 0xAA                                    */
    GPS_PARSE_STATE_SYNC2,     /* Expecting 0x44                                      */
    GPS_PARSE_STATE_SYNC3,     /* Expecting 0x12 (long binary) or 0x13 (short binary) */
    GPS_PARSE_STATE_HEADER,    /* Accumulating remaining header bytes                 */
    GPS_PARSE_STATE_BODY,      /* Accumulating body bytes                             */
    GPS_PARSE_STATE_CRC,       /* Accumulating 4-byte CRC                             */
} gps_parse_state_t;

typedef struct
{
    gps_parse_state_t state;

    /* Working buffer: sized for the largest possible frame (long header + max body + CRC) */
    uint8_t  msg_buf[GPS_OEM7_LONG_HDR_LEN + GPS_MAX_LOG_BODY_LEN + GPS_OEM7_CRC_LEN];
    uint16_t msg_pos;        /* Next write index into msg_buf                          */

    /* Decoded from the header once it is complete */
    uint8_t  hdr_len;        /* Total header length (byte 3 of header)                */
    uint16_t body_len;       /* Message body length (LE, offset depends on format)     */
    uint16_t msg_id;         /* Message ID (bytes 4-5, LE, same in both formats)       */
    bool     is_short_binary; /* true = Sync3 was 0x13; false = 0x12                  */
} gps_parser_t;

/* Sub-phases of the CONFIGURING state: first the ASCII setup commands
 * (UNLOGALL, CONNECTIMU, SETINSTRANSLATION, SETINSROTATION), then the log
 * subscriptions, then a brief settle before moving to RUNNING. */
typedef enum
{
    GPS_CONFIG_PHASE_SETUP = 0,
    GPS_CONFIG_PHASE_LOGS,
    GPS_CONFIG_PHASE_DONE,
} gps_config_phase_t;

/*============================================================================*/
/* Private State                                                              */
/*============================================================================*/

static gps_svc_state_t g_state        = GPS_SVC_STATE_PROBING;
static uint32_t        g_tick_count   = 0U;
static gps_parser_t    g_parser;

/* Probe phase counters */
static uint32_t g_probe_count     = 0U;
static uint32_t g_probe_last_tick = 0U;

/* Config-phase state machine: walks gps_oem7_setup_commands[] and then
 * gps_oem7_log_configs[], sending one command at a time and waiting for a
 * "<OK" ASCII ack before moving on. */
static gps_config_phase_t g_config_phase   = GPS_CONFIG_PHASE_SETUP;
static uint8_t            g_config_idx     = 0U;
static bool               g_awaiting_resp  = false;
static uint32_t           g_cmd_sent_tick  = 0U;

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/

static gps_status_t gps_svc_probe_step(void);
static gps_status_t gps_svc_config_step(void);
static gps_status_t gps_svc_run_step(void);
static void         gps_svc_parser_reset(gps_parser_t *p);
static void         gps_svc_parser_feed_byte(gps_parser_t *p, uint8_t byte);
static void         gps_svc_dispatch_log(uint16_t msg_id,
                                         const uint8_t *hdr,
                                         const uint8_t *body,
                                         uint16_t body_len);
static uint32_t     gps_crc32(const uint8_t *buf, uint32_t len);

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

gps_status_t gps_svc_init(void)
{
    gps_svc_parser_reset(&g_parser);

    g_state            = GPS_SVC_STATE_PROBING;
    g_tick_count       = 0U;
    g_probe_count      = 0U;
    g_probe_last_tick  = 0U;
    g_config_phase     = GPS_CONFIG_PHASE_SETUP;
    g_config_idx       = 0U;
    g_awaiting_resp    = false;
    g_cmd_sent_tick    = 0U;

    return gps_drv_oem7_init(&gps_hw_config);
}

gps_status_t gps_svc_start(void)
{
    return gps_drv_oem7_start(&gps_hw_config);
}

/*============================================================================*/
/* Periodic Update                                                            */
/*============================================================================*/

gps_status_t gps_svc_update(void)
{
    g_tick_count++;

    switch (g_state)
    {
        case GPS_SVC_STATE_PROBING:
            return gps_svc_probe_step();

        case GPS_SVC_STATE_CONFIGURING:
            return gps_svc_config_step();

        case GPS_SVC_STATE_RUNNING:
            return gps_svc_run_step();

        case GPS_SVC_STATE_FAULT:
        default:
            return GPS_ERROR;
    }
}

/*============================================================================*/
/* State Machine Steps                                                        */
/*============================================================================*/

/**
 * @brief  PROBING state handler.
 *
 *         Sends "UNLOGALL\r\n" every GPS_PROBE_INTERVAL_TICKS ticks.  The
 *         OEM7600 responds with "<OK" when the command is accepted.
 *         On receiving a response that
 *         begins with "<OK", the receiver is considered awake and the
 *         state machine advances to CONFIGURING.  Responses beginning
 *         with "<ERROR" (or no response) are ignored and the probe is
 *         retried.
 *
 *         ME_READY is not monitored because it is not connected to the MCU.
 *         This might be nice for future revision of backplane + MCU...
 */
static void gps_svc_drain_ring_buffer(void)
{
    uint8_t drain[64];
    while (gps_drv_oem7_bytes_available() > 0U)
    {
        gps_drv_oem7_read(drain, sizeof(drain));
    }
}

static gps_status_t gps_svc_probe_step(void)
{
    /* Check for a response from the previous probe */
    if (gps_drv_oem7_bytes_available() > 0U)
    {
        /* Read just enough to check the response prefix */
        uint8_t prefix[5];
        uint32_t n = gps_drv_oem7_read(prefix, sizeof(prefix));

        /* Accept if the response starts with "<OK" */
        if (n >= 5U && prefix[2] == '<' && prefix[3] == 'O' && prefix[4] == 'K')
        {
            /* Drain the rest of the VERSION response before moving on */
            gps_svc_drain_ring_buffer();
            g_state         = GPS_SVC_STATE_CONFIGURING;
            g_config_phase  = GPS_CONFIG_PHASE_SETUP;
            g_config_idx    = 0U;
            g_awaiting_resp = false;
            g_cmd_sent_tick = g_tick_count;
            return GPS_OK;
        }

        /* <ERROR or garbage — drain and keep probing */
        gps_svc_drain_ring_buffer();
    }

    /* Time to send another probe? */
    if ((g_tick_count - g_probe_last_tick) >= GPS_PROBE_INTERVAL_TICKS)
    {
        g_probe_last_tick = g_tick_count;
        g_probe_count++;

        gps_drv_oem7_send_command("UNLOGALL\r\n");

        if (g_probe_count >= GPS_PROBE_MAX_ATTEMPTS)
        {
            g_state = GPS_SVC_STATE_FAULT;
            return GPS_ERROR_TIMEOUT;
        }
    }

    return GPS_OK;
}

/**
 * @brief  Returns the command string for the current phase+index, or NULL
 *         if the current index has reached the sentinel of its array.
 */
static const char *gps_svc_current_config_command(void)
{
    switch (g_config_phase)
    {
        case GPS_CONFIG_PHASE_SETUP:
            return gps_oem7_setup_commands[g_config_idx];
        case GPS_CONFIG_PHASE_LOGS:
            return gps_oem7_log_configs[g_config_idx].command;
        case GPS_CONFIG_PHASE_DONE:
        default:
            return NULL;
    }
}

/**
 * @brief  Advance to the next command.  If the current phase's array is
 *         exhausted, advance to the next phase (SETUP → LOGS → DONE).
 */
static void gps_svc_advance_config_index(void)
{
    g_config_idx++;

    /* If the next slot is the sentinel, roll over to the next phase */
    if (gps_svc_current_config_command() == NULL)
    {
        g_config_idx = 0U;
        if (g_config_phase == GPS_CONFIG_PHASE_SETUP)
        {
            g_config_phase = GPS_CONFIG_PHASE_LOGS;
        }
        else
        {
            g_config_phase = GPS_CONFIG_PHASE_DONE;
        }
    }
}

/**
 * @brief  CONFIGURING state handler.
 *
 *         Walks the setup and log-subscription command lists one entry per
 *         tick.  Setup commands are sent with per-command ack: on "<OK" we
 *         advance, on "<ERROR" or timeout the service transitions to FAULT.
 *         LOG commands are fire-and-forget — once the first LOG kicks off
 *         a binary stream, responses interleave with binary frames on the
 *         same UART and per-command ack detection is unreliable.  Once
 *         both lists are exhausted, a short settle window drains the ring
 *         buffer before the parser is reset and the service transitions
 *         to RUNNING.
 *
 *         NovAtel abbreviated-ASCII responses are framed as:
 *             \r\n<OK\r\n[<parameters>]
 *         so the prefix check examines bytes [2..4] for '<', 'O', 'K'.
 */
static gps_status_t gps_svc_config_step(void)
{
    /* All commands sent and ack'd — settle briefly, then start parsing logs */
    if (g_config_phase == GPS_CONFIG_PHASE_DONE)
    {
        if ((g_tick_count - g_cmd_sent_tick) >= GPS_CONFIG_SETTLE_TICKS)
        {
            gps_svc_drain_ring_buffer();
            gps_svc_parser_reset(&g_parser);
            g_state = GPS_SVC_STATE_RUNNING;
        }
        return GPS_OK;
    }

    /* Waiting for the receiver to acknowledge the most recent command */
    if (g_awaiting_resp)
    {
        if (gps_drv_oem7_bytes_available() >= 5U)
        {
            uint8_t prefix[5];
            (void)gps_drv_oem7_read(prefix, sizeof(prefix));

            if (prefix[2] == '<' && prefix[3] == 'O' && prefix[4] == 'K')
            {
                /* Drain any trailing response bytes before the next command */
                gps_svc_drain_ring_buffer();
                g_awaiting_resp = false;
                gps_svc_advance_config_index();
                g_cmd_sent_tick = g_tick_count;
            }
            else
            {
                /* "<ERROR" or unexpected prefix — abort config */
                gps_svc_drain_ring_buffer();
                g_state = GPS_SVC_STATE_FAULT;
                return GPS_ERROR;
            }
        }
        else if ((g_tick_count - g_cmd_sent_tick)
                 >= GPS_CONFIG_RESPONSE_TIMEOUT_TICKS)
        {
            g_state = GPS_SVC_STATE_FAULT;
            return GPS_ERROR_TIMEOUT;
        }

        return GPS_OK;
    }

    /* Idle between commands — send the next one */
    const char *cmd = gps_svc_current_config_command();
    if (cmd == NULL)
    {
        /* Defensive: advance_config_index keeps this from happening, but if
         * it does we simply enter the settle period. */
        g_config_phase  = GPS_CONFIG_PHASE_DONE;
        g_cmd_sent_tick = g_tick_count;
        return GPS_OK;
    }

    gps_drv_oem7_send_command(cmd);
    g_cmd_sent_tick = g_tick_count;

    /* LOG commands kick off binary streams that interleave with subsequent
     * command responses on the same UART, so per-command ack detection is
     * unreliable.  Fire-and-forget through the LOGS phase; the DONE-phase
     * settle drains whatever the receiver emitted in response. */
    if (g_config_phase == GPS_CONFIG_PHASE_LOGS)
    {
        gps_svc_advance_config_index();
    }
    else
    {
        g_awaiting_resp = true;
    }

    return GPS_OK;
}

/**
 * @brief  RUNNING state handler.
 *
 *         Drains every available byte from the ring buffer through the
 *         binary frame parser.  Reads in GPS_SVC_READ_CHUNK_SIZE bursts
 *         to bound stack usage; loops until the buffer is empty so high-
 *         rate streams (e.g. RAWIMUSX at 125 Hz) cannot accumulate.
 */
static gps_status_t gps_svc_run_step(void)
{
    uint8_t  chunk[GPS_SVC_READ_CHUNK_SIZE];

    for (;;)
    {
        uint32_t n = gps_drv_oem7_read(chunk, sizeof(chunk));
        if (n == 0U)
        {
            break;
        }

        for (uint32_t i = 0U; i < n; i++)
        {
            gps_svc_parser_feed_byte(&g_parser, chunk[i]);
        }
    }

    return GPS_OK;
}

/*============================================================================*/
/* Binary Frame Parser                                                        */
/*============================================================================*/

/**
 * @brief  NovAtel OEM7 binary message header layouts:
 *
 *  Long binary header (28 bytes, Sync3 = 0x12):
 *   [0]     Sync1          = 0xAA
 *   [1]     Sync2          = 0x44
 *   [2]     Sync3          = 0x12
 *   [3]     Header Len     (uint8,  = 28)
 *   [4-5]   Message ID     (uint16 LE)
 *   [6]     Message Type
 *   [7]     Port Address
 *   [8-9]   Message Len    (uint16 LE)  — body length
 *   [10-11] Sequence
 *   [12]    Idle Time
 *   [13]    Time Status
 *   [14-15] GPS Week       (uint16 LE)
 *   [16-19] GPS ms         (uint32 LE)
 *   [20-23] Receiver Status
 *   [24-25] Reserved
 *   [26-27] Receiver SW Version
 *
 *  Short binary header (12 bytes, Sync3 = 0x13):
 *   [0]     Sync1          = 0xAA
 *   [1]     Sync2          = 0x44
 *   [2]     Sync3          = 0x13
 *   [3]     Header Len     (uint8,  = 12)
 *   [4-5]   Message ID     (uint16 LE)
 *   [6-7]   Message Len    (uint16 LE)  — body length
 *   [8-9]   Sequence       (uint16 LE)
 *   [10]    Idle Time
 *   [11]    Time Status
 *
 *  Both formats are followed by body (Message Len bytes) then a
 *  4-byte CRC-32 computed over header + body.
 */

static void gps_svc_parser_reset(gps_parser_t *p)
{
    p->state           = GPS_PARSE_STATE_SYNC1;
    p->msg_pos         = 0U;
    p->hdr_len         = 0U;
    p->body_len        = 0U;
    p->msg_id          = 0U;
    p->is_short_binary = false;
}

static void gps_svc_parser_feed_byte(gps_parser_t *p, uint8_t byte)
{
    switch (p->state)
    {
        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_SYNC1:
        /*--------------------------------------------------------------------*/
            if (byte == GPS_OEM7_SYNC_1)
            {
                gps_svc_parser_reset(p);
                p->msg_buf[p->msg_pos++] = byte;
                p->state = GPS_PARSE_STATE_SYNC2;
            }
            break;

        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_SYNC2:
        /*--------------------------------------------------------------------*/
            if (byte == GPS_OEM7_SYNC_2)
            {
                p->msg_buf[p->msg_pos++] = byte;
                p->state = GPS_PARSE_STATE_SYNC3;
            }
            else
            {
                gps_svc_parser_reset(p);
            }
            break;

        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_SYNC3:
        /*--------------------------------------------------------------------*/
            if (byte == GPS_OEM7_SYNC_3_LONG)
            {
                p->msg_buf[p->msg_pos++] = byte;
                p->is_short_binary = false;
                p->state = GPS_PARSE_STATE_HEADER;
            }
            else if (byte == GPS_OEM7_SYNC_3_SHORT)
            {
                p->msg_buf[p->msg_pos++] = byte;
                p->is_short_binary = true;
                /* Short binary header is fixed at 12 bytes; there is no
                 * header-length field in the frame itself. */
                p->hdr_len = GPS_OEM7_SHORT_HDR_LEN;
                p->state   = GPS_PARSE_STATE_HEADER;
            }
            else
            {
                gps_svc_parser_reset(p);
            }
            break;

        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_HEADER:
        /*--------------------------------------------------------------------*/
            /* Guard against buffer overrun: the long header is the largest */
            if (p->msg_pos >= GPS_OEM7_LONG_HDR_LEN)
            {
                gps_svc_parser_reset(p);
                break;
            }

            p->msg_buf[p->msg_pos] = byte;

            if (p->is_short_binary)
            {
                /* Short binary header layout:
                 *   [3]     Message Length (uint8)  — body length in bytes
                 *   [4-5]   Message ID     (uint16 LE)
                 *   [6-7]   GPS Week       (uint16 LE)
                 *   [8-11]  GPS Milliseconds (uint32 LE) */
                if (p->msg_pos == 3U)
                {
                    p->body_len = (uint16_t)byte;
                    if (p->body_len > GPS_MAX_LOG_BODY_LEN)
                    {
                        gps_svc_parser_reset(p);
                        break;
                    }
                }
                else if (p->msg_pos == 4U)
                {
                    p->msg_id = (uint16_t)byte;
                }
                else if (p->msg_pos == 5U)
                {
                    p->msg_id |= (uint16_t)((uint16_t)byte << 8U);
                }
            }
            else
            {
                /* Long binary header layout:
                 *   [3]     Header Length  (uint8, = 28)
                 *   [4-5]   Message ID     (uint16 LE)
                 *   [6]     Message Type
                 *   [7]     Port Address
                 *   [8-9]   Message Length (uint16 LE) — body length */
                if (p->msg_pos == 3U)
                {
                    p->hdr_len = byte;
                    if (p->hdr_len != GPS_OEM7_LONG_HDR_LEN)
                    {
                        gps_svc_parser_reset(p);
                        break;
                    }
                }
                else if (p->msg_pos == 4U)
                {
                    p->msg_id = (uint16_t)byte;
                }
                else if (p->msg_pos == 5U)
                {
                    p->msg_id |= (uint16_t)((uint16_t)byte << 8U);
                }
                else if (p->msg_pos == 8U)
                {
                    p->body_len = (uint16_t)byte;
                }
                else if (p->msg_pos == 9U)
                {
                    p->body_len |= (uint16_t)((uint16_t)byte << 8U);
                    if (p->body_len > GPS_MAX_LOG_BODY_LEN)
                    {
                        gps_svc_parser_reset(p);
                        break;
                    }
                }
            }

            p->msg_pos++;

            /* Transition once the full header has been consumed */
            if (p->hdr_len > 0U && p->msg_pos == (uint16_t)p->hdr_len)
            {
                p->state = (p->body_len > 0U) ? GPS_PARSE_STATE_BODY
                                               : GPS_PARSE_STATE_CRC;
            }
            break;

        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_BODY:
        /*--------------------------------------------------------------------*/
        {
            uint16_t body_offset = p->msg_pos - (uint16_t)p->hdr_len;

            if (body_offset >= p->body_len ||
                p->msg_pos >= sizeof(p->msg_buf))
            {
                gps_svc_parser_reset(p);
                break;
            }

            p->msg_buf[p->msg_pos++] = byte;
            body_offset++;

            if (body_offset == p->body_len)
            {
                p->state = GPS_PARSE_STATE_CRC;
            }
            break;
        }

        /*--------------------------------------------------------------------*/
        case GPS_PARSE_STATE_CRC:
        /*--------------------------------------------------------------------*/
        {
            uint16_t total_before_crc = (uint16_t)p->hdr_len + p->body_len;
            uint16_t crc_offset       = p->msg_pos - total_before_crc;

            if (crc_offset >= GPS_OEM7_CRC_LEN ||
                p->msg_pos >= sizeof(p->msg_buf))
            {
                gps_svc_parser_reset(p);
                break;
            }

            p->msg_buf[p->msg_pos++] = byte;
            crc_offset++;

            if (crc_offset == GPS_OEM7_CRC_LEN)
            {
                /* Full message in buffer — verify CRC */
                uint32_t computed = gps_crc32(p->msg_buf, total_before_crc);
                uint32_t received;
                memcpy(&received, &p->msg_buf[total_before_crc], sizeof(uint32_t));

                if (computed == received)
                {
                    gps_svc_dispatch_log(p->msg_id,
                                         p->msg_buf,
                                         &p->msg_buf[p->hdr_len],
                                         p->body_len);
                }
                /* Whether CRC matched or not, reset for the next message */
                gps_svc_parser_reset(p);
            }
            break;
        }

        default:
            gps_svc_parser_reset(p);
            break;
    }
}

/*============================================================================*/
/* Log Dispatch                                                               */
/*============================================================================*/

/*
 * OEM7 binary log body layouts used below (all little-endian):
 *
 *  BESTGNSSPOS (long header, msg_id 1429, body >= 72 B):
 *    [0-3]   sol_status  (uint32)
 *    [4-7]   pos_type    (uint32)
 *    [8-15]  latitude    (double, deg)
 *    [16-23] longitude   (double, deg)
 *    [24-31] height      (double, m)
 *    [32-35] undulation  (float)
 *    [36-39] datum_id    (uint32)
 *    [40-43] lat_std     (float, m)
 *    [44-47] lon_std     (float, m)
 *    [48-51] hgt_std     (float, m)
 *
 *  INSPVAS (short header, msg_id 508, body = 88 B):
 *    [0-3]   gnss_week   (uint32)
 *    [4-11]  gnss_secs   (double, seconds into week)
 *    [12-19] latitude    (double, deg)
 *    [20-27] longitude   (double, deg)
 *    [28-35] height      (double, m)
 *    [36-43] north_vel   (double, m/s)
 *    [44-51] east_vel    (double, m/s)
 *    [52-59] up_vel      (double, m/s)
 *    [60-67] roll        (double, deg)
 *    [68-75] pitch       (double, deg)
 *    [76-83] azimuth     (double, deg)
 *    [84-87] status      (uint32 enum)
 *
 *  RAWIMUSX (short header, msg_id 1462, body = 40 B):
 *    [0]     imu_info    (uint8)
 *    [1]     imu_type    (uint8)
 *    [2-3]   gnss_week   (uint16)
 *    [4-11]  gnss_secs   (double)
 *    [12-15] imu_status  (uint32)
 *    [16-19] z_accel     (int32)
 *    [20-23] neg_y_accel (int32)
 *    [24-27] x_accel     (int32)
 *    [28-31] z_gyro      (int32)
 *    [32-35] neg_y_gyro  (int32)
 *    [36-39] x_gyro      (int32)
 *
 *  Long-header gnss_week/gnss_ms for BESTGNSSPOS come from the header:
 *    hdr[14-15] GPS week (uint16), hdr[16-19] GPS ms (uint32).
 */

static void gps_svc_dispatch_bestgnsspos(const uint8_t *hdr, const uint8_t *body)
{
    struct ucr_03_best_gnsspos_t frame;
    uint32_t sol_stat;
    uint32_t pos_type;

    memcpy(&sol_stat,          &body[0],  4);
    memcpy(&pos_type,          &body[4],  4);
    memcpy(&frame.lat,         &body[8],  8);
    memcpy(&frame.lng,         &body[16], 8);
    memcpy(&frame.hgt,         &body[24], 8);
    memcpy(&frame.lat_std_dev, &body[40], 4);
    memcpy(&frame.long_std_dev,&body[44], 4);
    memcpy(&frame.height_std_dev, &body[48], 4);
    memcpy(&frame.gnss_week,   &hdr[14],  2);
    memcpy(&frame.gnss_ms,     &hdr[16],  4);
    frame.sol_stat = (uint8_t)sol_stat;
    frame.pos_type = (uint8_t)pos_type;

    can_msg_t msg;
    msg.channel = CAN1;
    msg.id      = UCR_03_BEST_GNSSPOS_FRAME_ID;
    msg.dlc     = UCR_03_BEST_GNSSPOS_LENGTH;
    ucr_03_best_gnsspos_pack(msg.data, &frame, msg.dlc);
    com_svc_can_transmit(&msg);
}

static void gps_svc_dispatch_inspvas(const uint8_t *hdr, const uint8_t *body)
{
    uint32_t gnss_week;
    double   lat, lng, hgt;
    uint32_t status;
    uint32_t gnss_ms;
    can_msg_t msg;

    memcpy(&gnss_week, &body[0],  4);
    memcpy(&lat,       &body[12], 8);
    memcpy(&lng,       &body[20], 8);
    memcpy(&hgt,       &body[28], 8);
    memcpy(&status,    &body[84], 4);

    /* Short binary header: bytes [8-11] = GPS milliseconds (uint32 LE) */
    memcpy(&gnss_ms, &hdr[8], 4);

    struct ucr_03_inspvas_gps_t gps_frame;
    gps_frame.gnss_week   = gnss_week;
    gps_frame.gnss_ms     = gnss_ms;
    gps_frame.gnss_lat    = lat;
    gps_frame.gnss_long   = lng;
    gps_frame.gnss_height = hgt;
    gps_frame.status      = status;

    msg.channel = CAN1;
    msg.id      = UCR_03_INSPVAS_GPS_FRAME_ID;
    msg.dlc     = UCR_03_INSPVAS_GPS_LENGTH;
    ucr_03_inspvas_gps_pack(msg.data, &gps_frame, msg.dlc);
    com_svc_can_transmit(&msg);

    struct ucr_03_inspvas_imu_t imu_frame;
    memcpy(&imu_frame.north_vel, &body[36], 8);
    memcpy(&imu_frame.east_vel,  &body[44], 8);
    memcpy(&imu_frame.up_vel,    &body[52], 8);
    memcpy(&imu_frame.roll,      &body[60], 8);
    memcpy(&imu_frame.pitch,     &body[68], 8);
    memcpy(&imu_frame.azimuth,   &body[76], 8);
    imu_frame.status  = status;
    imu_frame.gnss_ms = gnss_ms;

    msg.channel = CAN1;
    msg.id      = UCR_03_INSPVAS_IMU_FRAME_ID;
    msg.dlc     = UCR_03_INSPVAS_IMU_LENGTH;
    ucr_03_inspvas_imu_pack(msg.data, &imu_frame, msg.dlc);
    com_svc_can_transmit(&msg);
}

static void gps_svc_dispatch_rawimusx(const uint8_t *hdr, const uint8_t *body)
{
    struct ucr_03_rawimu_t frame;
    uint16_t gnss_week;
    uint32_t imu_status;

    /* Short binary header: bytes [6-7] = GPS week, [8-11] = GPS ms */
    memcpy(&gnss_week,    &hdr[6], 2);
    memcpy(&frame.gnss_ms,&hdr[8], 4);

    memcpy(&imu_status,       &body[12], 4);
    memcpy(&frame.z_accel,    &body[16], 4);
    memcpy(&frame.neg_y_accel,&body[20], 4);
    memcpy(&frame.x_accel,    &body[24], 4);
    memcpy(&frame.z_gyro,     &body[28], 4);
    memcpy(&frame.neg_y_gyro, &body[32], 4);
    memcpy(&frame.x_gyro,     &body[36], 4);

    frame.gnss_week = (uint32_t)gnss_week;
    frame.status    = (uint8_t)imu_status;

    can_msg_t msg;
    msg.channel = CAN1;
    msg.id      = UCR_03_RAWIMU_FRAME_ID;
    msg.dlc     = UCR_03_RAWIMU_LENGTH;
    ucr_03_rawimu_pack(msg.data, &frame, msg.dlc);
    com_svc_can_transmit(&msg);
}

static void gps_svc_dispatch_log(uint16_t msg_id,
                                  const uint8_t *hdr,
                                  const uint8_t *body,
                                  uint16_t body_len)
{
    switch (msg_id)
    {
        case GPS_OEM7_BESTGNSSPOS_ID:
            if (body_len >= 52U)
            {
                gps_svc_dispatch_bestgnsspos(hdr, body);
            }
            break;

        case GPS_OEM7_INSPVAS_ID:
            if (body_len >= 88U)
            {
                gps_svc_dispatch_inspvas(hdr, body);
            }
            break;

        case GPS_OEM7_RAWIMUSX_ID:
            if (body_len >= 40U)
            {
                gps_svc_dispatch_rawimusx(hdr, body);
            }
            break;

        default:
            break;
    }
}

/*============================================================================*/
/* CRC-32 (OEM7 variant: poly GPS_OEM7_CRC_POLY, init 0, no final XOR)     */
/*============================================================================*/

static uint32_t gps_crc32(const uint8_t *buf, uint32_t len)
{
    uint32_t crc = 0U;

    for (uint32_t i = 0U; i < len; i++)
    {
        uint32_t b = (crc ^ (uint32_t)buf[i]) & 0xFFU;
        for (uint8_t bit = 0U; bit < 8U; bit++)
        {
            if (b & 1U)
                b = (b >> 1U) ^ GPS_OEM7_CRC_POLY;
            else
                b >>= 1U;
        }
        crc = (crc >> 8U) ^ b;
    }

    return crc;
}

/*============================================================================*/
/* State Access                                                               */
/*============================================================================*/

gps_svc_state_t gps_svc_get_state(void)
{
    return g_state;
}
