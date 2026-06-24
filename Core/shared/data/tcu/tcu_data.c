#include "tcu_data.h"
#include <string.h>

/*============================================================================*/
/* Private Data Storage                                                       */
/*============================================================================*/

static tcu_apps_data_t g_apps_data;
static tcu_apps_plausibility_t g_apps_plausibility;
static tcu_bse_data_t g_bse_data;
static tcu_motor_data_t g_motor_data;
static tcu_motor_request_t g_motor_request;
static tcu_motor_status_t g_motor_status;
static tcu_traction_control_data_t g_traction_control_data;
static bool g_rtd_pressed;
static tcu_fault_status_t g_fault_status;
static tcu_app_state_t g_tcu_state;
static bool g_initialized = false;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

//TODO configure default values as invaldi
status_t tcu_data_init(void)
{
    memset(&g_apps_data, 0, sizeof(g_apps_data));
    g_apps_data.sensor1.validity = TCU_SENSOR_VALID;
    g_apps_data.sensor2.validity = TCU_SENSOR_VALID;

    memset(&g_apps_plausibility, 0, sizeof(g_apps_plausibility));
    g_apps_plausibility.is_plausible = false;
    g_apps_plausibility.implausibility_active = false;

    memset(&g_bse_data, 0, sizeof(g_bse_data));
    g_bse_data.sensor.validity = TCU_SENSOR_VALID;
    g_bse_data.is_braking = false;
    g_bse_data.is_hard_braking = false;

    memset(&g_motor_data, 0, sizeof(g_motor_data));

    memset(&g_motor_request, 0, sizeof(g_motor_request));
    g_motor_request.enable = true;

    memset(&g_motor_status, 0, sizeof(g_motor_status));

    memset(&g_traction_control_data, 0, sizeof(g_traction_control_data));
    g_traction_control_data.bamocar_n_lim_percent = 100.0f;

    g_rtd_pressed = false;

    memset(&g_fault_status, 0, sizeof(g_fault_status));

    g_initialized = true;

    return OK;
}

/*============================================================================*/
/* APPS Data Access                                                           */
/*============================================================================*/

//TODO: add mutex protection for these data accesses
status_t tcu_data_set_apps(const tcu_apps_data_t *apps_data)
{
    if (apps_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_apps_data = *apps_data;

    return OK;
}

status_t tcu_data_get_apps(tcu_apps_data_t *apps_data)
{
    if (apps_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *apps_data = g_apps_data;

    return OK;
}

float tcu_data_get_apps_percent(void)
{
    if (g_apps_plausibility.implausibility_active)
    {
        return 0.0f;
    }
    return g_apps_data.pedal_percent;
}

bool tcu_data_get_apps_fault(void)
{
    return g_apps_plausibility.implausibility_active;
}

/*============================================================================*/
/* APPS Plausibility Access                                                   */
/*============================================================================*/

void tcu_data_set_apps_plausibility(const tcu_apps_plausibility_t *plausibility)
{
    if (plausibility == NULL)
    {
        return;
    }

    g_apps_plausibility = *plausibility;
}

void tcu_data_get_apps_plausibility(tcu_apps_plausibility_t *plausibility)
{
    if (plausibility == NULL)
    {
        return;
    }

    *plausibility = g_apps_plausibility;
}

/*============================================================================*/
/* BSE Data Access                                                            */
/*============================================================================*/

status_t tcu_data_set_bse(const tcu_bse_data_t *bse_data)
{
    if (bse_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_bse_data = *bse_data;

    return OK;
}

status_t tcu_data_get_bse(tcu_bse_data_t *bse_data)
{
    if (bse_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *bse_data = g_bse_data;

    return OK;
}

bool tcu_data_get_brake_active(void)
{
    return g_bse_data.is_braking;
}

bool tcu_data_get_hard_braking(void)
{
    return g_bse_data.is_hard_braking;
}

/*============================================================================*/
/* Motor Data Access                                                          */
/*============================================================================*/

status_t tcu_data_set_motor(const tcu_motor_data_t *motor_data)
{
    if (motor_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_motor_data = *motor_data;

    return OK;
}

status_t tcu_data_get_motor(tcu_motor_data_t *motor_data)
{
    if (motor_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *motor_data = g_motor_data;

    return OK;
}

float tcu_data_get_motor_rpm(void)
{
    return g_motor_data.rpm;
}

void tcu_data_set_motor_rpm(float rpm)
{
    g_motor_data.rpm = rpm;
}

void tcu_data_set_motor_active_current(float active_current_a)
{
    g_motor_data.active_current_a = active_current_a;
}

void tcu_data_set_motor_reactive_current(float reactive_current_a)
{
    g_motor_data.reactive_current_a = reactive_current_a;
}

void tcu_data_set_motor_ac_current(float ac_current_a)
{
    g_motor_data.ac_current_a = ac_current_a;
}

float tcu_data_get_motor_temp(void)
{
    return g_motor_data.motor_temp_c;
}

void tcu_data_set_motor_temp(float temp)
{
    g_motor_data.motor_temp_c = temp;
}

float tcu_data_get_inverter_temp(void)
{
    return g_motor_data.inverter_temp_c;
}


void tcu_data_set_inverter_temp(float temp)
{
    g_motor_data.inverter_temp_c = temp;
}

status_t tcu_data_get_motor_status(tcu_motor_status_t *motor_status)
{
    if (motor_status == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *motor_status = g_motor_status;

    return OK;
}

void tcu_data_set_bamocar_status_display(const tcu_bamocar_status_display_t *status)
{
    if (status == NULL)
    {
        return;
    }

    g_motor_status.status = *status;
}

void tcu_data_set_bamocar_error_display(const tcu_bamocar_error_display_t *error)
{
    if (error == NULL)
    {
        return;
    }

    g_motor_status.error = *error;
}

/*============================================================================*/
/* Traction Control Data Access                                               */
/*============================================================================*/

status_t tcu_data_set_traction_control(const tcu_traction_control_data_t *traction_control_data)
{
    if (traction_control_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_traction_control_data = *traction_control_data;

    return OK;
}

status_t tcu_data_get_traction_control(tcu_traction_control_data_t *traction_control_data)
{
    if (traction_control_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *traction_control_data = g_traction_control_data;

    return OK;
}

float tcu_data_get_traction_control_measured_slip_ratio(void)
{
    return g_traction_control_data.measured_slip_ratio;
}

void tcu_data_set_traction_control_measured_slip_ratio(float measured_slip_ratio)
{
    g_traction_control_data.measured_slip_ratio = measured_slip_ratio;
}

float tcu_data_get_traction_control_desired_slip_ratio(void)
{
    return g_traction_control_data.desired_slip_ratio;
}

void tcu_data_set_traction_control_desired_slip_ratio(float desired_slip_ratio)
{
    g_traction_control_data.desired_slip_ratio = desired_slip_ratio;
}

float tcu_data_get_traction_control_bamocar_n_lim_percent(void)
{
    return g_traction_control_data.bamocar_n_lim_percent;
}

void tcu_data_set_traction_control_bamocar_n_lim_percent(float bamocar_n_lim_percent)
{
    g_traction_control_data.bamocar_n_lim_percent = bamocar_n_lim_percent;
}

/*============================================================================*/
/* Motor Request Data Access                                                  */
/*============================================================================*/

status_t tcu_data_set_motor_request(const tcu_motor_request_t *motor_request_data)
{
    if (motor_request_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_motor_request = *motor_request_data;

    return OK;
}

status_t tcu_data_get_motor_request(tcu_motor_request_t *motor_request_data)
{
    if (motor_request_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *motor_request_data = g_motor_request;

    return OK;
}

bool tcu_data_get_motor_request_enabled()
{
    return g_motor_request.enable;
}

bool tcu_data_get_motor_request_reverse()
{
    return g_motor_request.reverse;
}


/*============================================================================*/
/* RTD Status Access                                                          */
/*============================================================================*/


void tcu_data_set_rtd_pressed(bool rtd_pressed)
{
    g_rtd_pressed = rtd_pressed;
}

bool tcu_data_get_rtd_pressed()
{
    return g_rtd_pressed;
}

/*============================================================================*/
/* Fault Status Access                                                        */
/*============================================================================*/

status_t tcu_data_set_fault_status(const tcu_fault_status_t *fault_status)
{
    if (fault_status == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_fault_status = *fault_status;

    return OK;
}

status_t tcu_data_get_fault_status(tcu_fault_status_t *fault_status)
{
    if (fault_status == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *fault_status = g_fault_status;

    return OK;
}

bool tcu_data_get_fault_active(void)
{
    return g_fault_status.apps_implausibility
        || g_fault_status.apps_sensor1_range
        || g_fault_status.apps_sensor2_range
        || g_fault_status.bse_range
        || g_fault_status.apps_bse_plausibility;
}

bool tcu_data_is_motor_enabled(void)
{
    //TODO IMPLEMENT THIS ONCE RTD AND TS DONE
//    if (!g_rtd_status.rtd_active)
//    {
//        return false;
//    }

    if (tcu_data_get_fault_active())
    {
        return false;
    }

    return true;
}


void tcu_data_set_state(tcu_app_state_t tcu_state)
{
    g_tcu_state = tcu_state;
}

tcu_app_state_t tcu_data_get_state()
{
    return g_tcu_state;
}
