#include "mco_svc_bamocar.h"
#include "can_svc_can.h"
#include "tcu_data.h"

static status_t mco_svc_bamocar_set_torque_raw(int16_t torque_raw);
static status_t mco_svc_bamocar_set_rpm_raw(int16_t rpm_raw);
static void mco_svc_bamocar_build_message(uint8_t *msg_data, uint8_t reg, int16_t value);
static status_t mco_svc_bamocar_transmit_message(const uint8_t *data);
static void mco_svc_bamocar_build_request(uint8_t *msg_data, uint8_t reg, uint8_t interval);
static void mco_svc_bamocar_parse_raw_value(uint8_t *message, uint8_t *reg, int16_t *data);
static void mco_svc_bamocar_parse_status_display(uint32_t raw, tcu_bamocar_status_display_t *status);
static void mco_svc_bamocar_parse_error_display(uint32_t raw, tcu_bamocar_error_display_t *error);

static float mco_svc_bamocar_raw_to_rpm(int16_t raw_value);
static float mco_svc_bamocar_raw_to_motor_temp(int16_t raw_value);
static float mco_svc_bamocar_raw_to_inverter_temp(int16_t raw_value);
static float mco_svc_bamocar_raw_to_current(int16_t raw_value);

#define BAMOCAR_STATUS_BIT(raw, bit) (((raw) >> (bit)) & 0x1U)
#define BAMOCAR_ERROR_BIT(raw, bit)  (((raw) >> (bit)) & 0x1U)


static bool g_initialized = false;


status_t mco_svc_bamocar_init(void)
{
    com_svc_can_register(BAMOCAR_CAN_RX_ID, mco_svc_bamocar_can_rx);
    //TODO start CAN here?
    g_initialized = true;
    return OK;
}


/*============================================================================*/
/* Message Building Helpers                                                   */
/*============================================================================*/

static void mco_svc_bamocar_build_message(uint8_t *msg_data, uint8_t reg, int16_t value)
{
    /* Bamocar message format: [register, data_low, data_high] */
    msg_data[0] = reg;
    msg_data[1] = (uint8_t)(value & 0xFF);
    msg_data[2] = (uint8_t)((value >> 8) & 0xFF);
}

static void mco_svc_bamocar_build_request(uint8_t *msg_data, uint8_t reg, uint8_t interval)
{
    /* Request format: [0x3D, register_to_request, interval] */
    msg_data[0] = BAMOCAR_REG_REQUEST_DATA;
    msg_data[1] = reg;
    msg_data[2] = interval;
}

/*============================================================================*/
/* Internal Transmit Helper                                                   */
/*============================================================================*/

static status_t mco_svc_bamocar_transmit_message(const uint8_t *data)
{
    can_msg_t message;
    
    message.channel = CAN2;
    message.id = BAMOCAR_CAN_TX_ID;
    message.dlc = 3;
    message.data[0] = data[0];
    message.data[1] = data[1];
    message.data[2] = data[2];
    
    com_svc_can_transmit(&message);
    return OK;
}

/*============================================================================*/
/* Torque Control                                                             */
/*============================================================================*/

status_t mco_svc_bamocar_set_torque_percent(float torque_percent)
{
    int16_t torque_raw;

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    torque_percent = clamp_float(torque_percent, 0.0f, 100.0f);

    torque_raw = (int16_t)(torque_percent * (float)BAMOCAR_MOTOR_TORQUE_MAX_RAW / 100.0f);

    return mco_svc_bamocar_set_torque_raw(torque_raw);
}


status_t mco_svc_bamocar_set_torque_nm(float torque_request_nm, bool rms)
{
    int16_t torque_raw;

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    torque_request_nm = clamp_float(torque_request_nm, 0.0f, BAMOCAR_MOTOR_MAX_NM);

    float ac_current_request_rms = torque_request_nm / BAMOCAR_MOTOR_KT_CONSTANT;

    if (rms)
    {
        torque_raw = (int16_t)(ac_current_request_rms * (float)BAMOCAR_MOTOR_TORQUE_MAX_RAW  * 1.414 / BAMOCAR_PEAK_CURRENT);
    }
    else
    {
        torque_raw = (int16_t)(ac_current_request_rms * (float)BAMOCAR_MOTOR_TORQUE_MAX_RAW  / BAMOCAR_PEAK_CURRENT);
    }

    return mco_svc_bamocar_set_torque_raw(torque_raw);
}

status_t mco_svc_bamocar_set_rpm(float rpm)
{
    int16_t rpm_raw;

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    rpm = clamp_float(rpm, 0.0f, BAMOCAR_MOTOR_MAX_RPM);

    rpm_raw = (int16_t)(rpm / BAMOCAR_MOTOR_MAX_RPM * (float)BAMOCAR_MOTOR_RPM_MAX_RAW);

    return mco_svc_bamocar_set_rpm_raw(rpm_raw);
}

status_t mco_svc_bamocar_set_n_lim_percent(float n_lim_percent)
{
    int16_t n_lim_raw;

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    n_lim_percent = clamp_float(n_lim_percent, 0.0f, 100.0f);
    n_lim_raw = (int16_t)(n_lim_percent * (float)BAMOCAR_CMD_MAX_VALUE / 100.0f);

    return mco_svc_bamocar_write_register(BAMOCAR_REG_N_LIM, n_lim_raw);
}

static status_t mco_svc_bamocar_set_torque_raw(int16_t torque_raw)
{
    uint8_t msg_data[3];

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    torque_raw = clamp_int16(torque_raw, BAMOCAR_MOTOR_TORQUE_MIN_RAW, BAMOCAR_MOTOR_TORQUE_MAX_RAW);

    mco_svc_bamocar_build_message(msg_data, BAMOCAR_REG_TORQUE_CMD, torque_raw);
    
    return mco_svc_bamocar_transmit_message(msg_data);
}

static status_t mco_svc_bamocar_set_rpm_raw(int16_t rpm_raw)
{
    uint8_t msg_data[3];

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    rpm_raw = clamp_int16(rpm_raw, BAMOCAR_MOTOR_RPM_MIN_RAW, BAMOCAR_MOTOR_RPM_MAX_RAW);

    mco_svc_bamocar_build_message(msg_data, BAMOCAR_REG_RPM_CMD, rpm_raw);
    
    return mco_svc_bamocar_transmit_message(msg_data);
}

/*============================================================================*/
/* Reading Data                                                               */
/*============================================================================*/

status_t mco_svc_bamocar_request_data(uint8_t reg, uint8_t interval)
{
    uint8_t msg_data[3];

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    mco_svc_bamocar_build_request(msg_data, (uint8_t)reg, (uint8_t)interval);
    
    return mco_svc_bamocar_transmit_message(msg_data);
}

/*============================================================================*/
/* Generic Register Access                                                    */
/*============================================================================*/

status_t mco_svc_bamocar_write_register(uint8_t reg, int16_t value)
{
    uint8_t msg_data[3];

    if (!g_initialized)
    {
        return ERROR_NOT_INITIALIZED;
    }

    mco_svc_bamocar_build_message(msg_data, (uint8_t)reg, value);
    
    return mco_svc_bamocar_transmit_message(msg_data);
}

/*============================================================================*/
/* Response Parsing Functions                                                 */
/*============================================================================*/

static void mco_svc_bamocar_parse_raw_value(uint8_t *message, uint8_t *reg, int16_t *data)
{
    /* Bamocar response format: [register, data_low, data_high] */
    *reg  = message[0];
    *data = (int16_t)((uint16_t)message[1] | ((uint16_t)message[2] << 8));

    return;
}

static float mco_svc_bamocar_raw_to_rpm(int16_t raw_value)
{
    return ((float)raw_value * BAMOCAR_MOTOR_MAX_RPM / BAMOCAR_CMD_MAX_VALUE);
}

//TODO: convert this interpolation into utility function
static float mco_svc_bamocar_raw_to_motor_temp(int16_t raw_value)
{
    static const float motor_temperatures_c[] =          {0,    10,    20,    30,    40,    50,    60,    70,    80,    90,    100,   110,   120,   130,   140,   150};
    static const int16_t motor_thermistor_values_raw[] = {9400, 10000, 10500, 11100, 11600, 12200, 12800, 13400, 13800, 14400, 14900, 15400, 15900, 16300, 16600, 16800};

    const uint8_t lut_size = sizeof(motor_temperatures_c) / sizeof(motor_temperatures_c[0]);

    raw_value = clamp_int16(raw_value, motor_thermistor_values_raw[0], motor_thermistor_values_raw[lut_size - 1]);

    for (uint8_t i = 0; i < lut_size - 1; i++)
    {
        if (raw_value < motor_thermistor_values_raw[i + 1])
        {
            float x0 = motor_thermistor_values_raw[i];
            float x1 = motor_thermistor_values_raw[i + 1];
            float y0 = motor_temperatures_c[i];
            float y1 = motor_temperatures_c[i + 1];

            float t = (raw_value - x0) / (x1 - x0);
            return y0 + t * (y1 - y0);
        }
    }

    return motor_temperatures_c[lut_size - 1];
}

static float mco_svc_bamocar_raw_to_inverter_temp(int16_t raw_value)
{
    static const float inverter_temperatures_c[] =          {0,     10,    20,    30,    40,    50,    60,    70,    80,    90,    100,   110,   120};
    static const int16_t inverter_thermistor_values_raw[] = {17151, 17688, 18386, 19247, 20250, 21357, 22515, 23671, 24775, 25792, 26702, 27497, 28170};

    const uint8_t lut_size = sizeof(inverter_temperatures_c) / sizeof(inverter_temperatures_c[0]);

    raw_value = clamp_int16(raw_value, inverter_thermistor_values_raw[0], inverter_thermistor_values_raw[lut_size - 1]);

    for (uint8_t i = 0; i < lut_size - 1; i++)
    {
        if (raw_value < inverter_thermistor_values_raw[i + 1])
        {
            float x0 = inverter_thermistor_values_raw[i];
            float x1 = inverter_thermistor_values_raw[i + 1];
            float y0 = inverter_temperatures_c[i];
            float y1 = inverter_temperatures_c[i + 1];

            float t = (raw_value - x0) / (x1 - x0);
            return y0 + t * (y1 - y0);
        }
    }

    return inverter_temperatures_c[lut_size - 1];
}

static float mco_svc_bamocar_raw_to_current(int16_t raw_value)
{
    //these magic numbers are default read only values in registers 0xD9 and 0xC6, see BAMOCAR_FAQ manual
    return ((float)raw_value * 400 / 1070);
}

static void mco_svc_bamocar_parse_status_display(uint32_t raw, tcu_bamocar_status_display_t *status)
{
    status->ena = (bool)BAMOCAR_STATUS_BIT(raw, 0);
    status->nc_r0 = (bool)BAMOCAR_STATUS_BIT(raw, 1);
    status->lim_plus = (bool)BAMOCAR_STATUS_BIT(raw, 2);
    status->lim_minus = (bool)BAMOCAR_STATUS_BIT(raw, 3);
    status->ok = (bool)BAMOCAR_STATUS_BIT(raw, 4);
    status->icns = (bool)BAMOCAR_STATUS_BIT(raw, 5);
    status->t_nlim = (bool)BAMOCAR_STATUS_BIT(raw, 6);
    status->pn = (bool)BAMOCAR_STATUS_BIT(raw, 7);
    status->ni = (bool)BAMOCAR_STATUS_BIT(raw, 8);
    status->n0 = (bool)BAMOCAR_STATUS_BIT(raw, 9);
    status->rsw = (bool)BAMOCAR_STATUS_BIT(raw, 10);
    status->cal0 = (bool)BAMOCAR_STATUS_BIT(raw, 11);
    status->cal = (bool)BAMOCAR_STATUS_BIT(raw, 12);
    status->tol = (bool)BAMOCAR_STATUS_BIT(raw, 13);
    status->rdy = (bool)BAMOCAR_STATUS_BIT(raw, 14);
    status->brk0 = (bool)BAMOCAR_STATUS_BIT(raw, 15);
    status->sign_mag = (bool)BAMOCAR_STATUS_BIT(raw, 16);
    status->nclip = (bool)BAMOCAR_STATUS_BIT(raw, 17);
    status->nclip_plus = (bool)BAMOCAR_STATUS_BIT(raw, 18);
    status->nclip_minus = (bool)BAMOCAR_STATUS_BIT(raw, 19);
    status->lrd_dig = (bool)BAMOCAR_STATUS_BIT(raw, 20);
    status->luse_rchd = (bool)BAMOCAR_STATUS_BIT(raw, 21);
    status->lrd_n = (bool)BAMOCAR_STATUS_BIT(raw, 22);
    status->ird_ti = (bool)BAMOCAR_STATUS_BIT(raw, 23);
    status->ird_tir = (bool)BAMOCAR_STATUS_BIT(raw, 24);
    status->ird10_hz = (bool)BAMOCAR_STATUS_BIT(raw, 25);
    status->ird_tm = (bool)BAMOCAR_STATUS_BIT(raw, 26);
    status->ird_ana = (bool)BAMOCAR_STATUS_BIT(raw, 27);
    status->lwcns = (bool)BAMOCAR_STATUS_BIT(raw, 28);
    status->rfe_pulse = (bool)BAMOCAR_STATUS_BIT(raw, 29);
    status->fiwe = (bool)BAMOCAR_STATUS_BIT(raw, 30);
    status->hnd_whl = (bool)BAMOCAR_STATUS_BIT(raw, 31);
}

static void mco_svc_bamocar_parse_error_display(uint32_t raw, tcu_bamocar_error_display_t *error)
{
    error->bad_paras = (bool)BAMOCAR_ERROR_BIT(raw, 0);
    error->power_fault = (bool)BAMOCAR_ERROR_BIT(raw, 1);
    error->rfe_fault = (bool)BAMOCAR_ERROR_BIT(raw, 2);
    error->bus_timeout = (bool)BAMOCAR_ERROR_BIT(raw, 3);
    error->feedback = (bool)BAMOCAR_ERROR_BIT(raw, 4);
    error->power_voltage = (bool)BAMOCAR_ERROR_BIT(raw, 5);
    error->motor_temp_fault = (bool)BAMOCAR_ERROR_BIT(raw, 6);
    error->device_temp_fault = (bool)BAMOCAR_ERROR_BIT(raw, 7);
    error->over_voltage = (bool)BAMOCAR_ERROR_BIT(raw, 8);
    error->i_peak_fault = (bool)BAMOCAR_ERROR_BIT(raw, 9);
    error->raceaway = (bool)BAMOCAR_ERROR_BIT(raw, 10);
    error->user = (bool)BAMOCAR_ERROR_BIT(raw, 11);
    error->hw_err = (bool)BAMOCAR_ERROR_BIT(raw, 14);
    error->ballast_fault = (bool)BAMOCAR_ERROR_BIT(raw, 15);
    error->warning0 = (bool)BAMOCAR_ERROR_BIT(raw, 16);
    error->illegal_status = (bool)BAMOCAR_ERROR_BIT(raw, 17);
    error->safe_in = (bool)BAMOCAR_ERROR_BIT(raw, 18);
    error->motor_temp_warn = (bool)BAMOCAR_ERROR_BIT(raw, 22);
    error->device_temp_warn = (bool)BAMOCAR_ERROR_BIT(raw, 23);
    error->vout_sat = (bool)BAMOCAR_ERROR_BIT(raw, 24);
    error->i_peak_warn = (bool)BAMOCAR_ERROR_BIT(raw, 25);
    error->raceway = (bool)BAMOCAR_ERROR_BIT(raw, 26);
    error->i2_r = (bool)BAMOCAR_ERROR_BIT(raw, 28);
    error->ballast_warn = (bool)BAMOCAR_ERROR_BIT(raw, 31);
}

float mco_svc_bamocar_calculate_theoretical_dc_power(float ac_current, float dc_voltage, float vout_duty)
{
    float ac_voltage = dc_voltage / 1.414 * vout_duty * BAMOCAR_EFFICIENCY;
    float ac_power = ac_current * ac_voltage * 1.732;
    float dc_power = ac_power / BAMOCAR_EFFICIENCY;
    
    return dc_power;
}

/*============================================================================*/
/* CAN Receive Mapping                                                        */
/*============================================================================*/

void mco_svc_bamocar_can_rx(can_msg_t *msg)
{
    if (msg == NULL || msg->id != BAMOCAR_CAN_RX_ID)
    {
        return;
    }

    uint8_t reg;
    int16_t data;

    mco_svc_bamocar_parse_raw_value(msg->data, &reg, &data);

    //TODO add calls to set data layer structs from here
    switch (reg)
    {
        case BAMOCAR_REG_RPM_ACTUAL:
        {
            float rpm = mco_svc_bamocar_raw_to_rpm(data);
            tcu_data_set_motor_rpm(rpm);
            break;
        }
        case BAMOCAR_REG_MOTOR_TEMP:
        {
            float motor_temp = mco_svc_bamocar_raw_to_motor_temp(data);
            tcu_data_set_motor_temp(motor_temp);
            break;
        }
        case BAMOCAR_REG_INVERTER_TEMP:
        {
            float inverter_temp = mco_svc_bamocar_raw_to_inverter_temp(data);
            tcu_data_set_inverter_temp(inverter_temp);
            break;
        }
        case BAMOCAR_REG_ACTIVE_CURRENT:
        {
            float iq = mco_svc_bamocar_raw_to_current(data);
            tcu_data_set_motor_active_current(iq);
            break;
        }
        case BAMOCAR_REG_REACTIVE_CURRENT:
        {
            float id = mco_svc_bamocar_raw_to_current(data);
            tcu_data_set_motor_reactive_current(id);
            break;
        }
        case BAMOCAR_REG_AC_CURRENT:
        {
            float ac_current_a = mco_svc_bamocar_raw_to_current(data);
            tcu_data_set_motor_ac_current(ac_current_a);
            break;
        }
        case BAMOCAR_STATUS_INFO:
        {
            tcu_bamocar_status_display_t status;
            uint32_t status_raw = (uint32_t)(uint16_t)data;

            mco_svc_bamocar_parse_status_display(status_raw, &status);
            tcu_data_set_bamocar_status_display(&status);
            break;
        }
        case BAMOCAR_ERROR_INFO:
        {
            tcu_bamocar_error_display_t error;
            uint32_t error_raw = (uint32_t)(uint16_t)data;

            mco_svc_bamocar_parse_error_display(error_raw, &error);
            tcu_data_set_bamocar_error_display(&error);
            break;
        }
        default:
            break;
    }
}
