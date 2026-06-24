#include <string.h>
#include "rco_data.h"


/*============================================================================*/
/* Private Data Storage                                                       */
/*============================================================================*/

static rco_data_t g_rco_data;
static rear_cooling_control_data_t g_rear_cooling_control_data;
static bool g_initialized;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t rco_data_init(void)
{
    memset(&g_rco_data, 0, sizeof(g_rco_data));
    g_rco_data.tssi_faulted = false;
    g_rco_data.brake_light_on = false;
    g_rco_data.drs_active = false;
    g_rco_data.reset_pressed = false;

    memset(&g_rear_cooling_control_data, 0, sizeof(g_rear_cooling_control_data));

    g_initialized = true;

    return OK;
}


status_t rco_data_get_data(rco_data_t *rco_data)
{
    if (rco_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *rco_data = g_rco_data;

    return OK;
}

status_t rco_data_set_data(rco_data_t *rco_data)
{
    if (rco_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_rco_data = *rco_data;

    return OK;
}


/*============================================================================*/
/* Brake Light Data Access                                                    */
/*============================================================================*/

void rco_data_set_brake_light(bool brake_light_on)
{
    g_rco_data.brake_light_on = brake_light_on;
}

/*============================================================================*/
/* TSSI Data Access                                                           */
/*============================================================================*/

void rco_data_set_tssi_faulted(bool tssi_faulted)
{
    g_rco_data.tssi_faulted = tssi_faulted; // red light on
}

/*============================================================================*/
/* DRS Data Access                                                            */
/*============================================================================*/

void rco_data_set_drs_active(bool drs_active)
{
    g_rco_data.drs_active = drs_active;
}

bool rco_data_get_drs_active()
{
    return g_rco_data.drs_active;
}

/*============================================================================*/
/* Reset Button Data Access                                                   */
/*============================================================================*/

void rco_data_set_reset_pressed(bool reset_pressed)
{
    g_rco_data.reset_pressed = reset_pressed;
}

bool rco_data_get_reset_pressed(void)
{
    return g_rco_data.reset_pressed;
}

/*============================================================================*/
/* DRS Data Access                                                   */
/*============================================================================*/


void rco_data_set_drs(bool drs_active)
{
    g_rco_data.drs_active = drs_active;
}

bool rco_data_get_drs()
{
    return g_rco_data.drs_active;
}

/*============================================================================*/
/* Rear Cooling Control Data Access                                           */
/*============================================================================*/

status_t rco_data_get_cooling_data(rear_cooling_control_data_t *cooling_data)
{
    if (cooling_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *cooling_data = g_rear_cooling_control_data;

    return OK;
}

status_t rco_data_set_cooling_data(rear_cooling_control_data_t *cooling_data)
{
    if (cooling_data == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    g_rear_cooling_control_data = *cooling_data;

    return OK;
}

void rco_data_set_tractive_pump_1_enable(bool enable)
{
    g_rear_cooling_control_data.tractive_pump_1_enable = enable;
}

void rco_data_set_tractive_pump_2_enable(bool enable)
{
    g_rear_cooling_control_data.tractive_pump_2_enable = enable;
}

void rco_data_set_tractive_fan_1_enable(bool enable)
{
    g_rear_cooling_control_data.tractive_fan_1_enable = enable;
}

void rco_data_set_tractive_fan_2_enable(bool enable)
{
    g_rear_cooling_control_data.tractive_fan_2_enable = enable;
}

void rco_data_set_accumulator_fan_1_enable(bool enable)
{
    g_rear_cooling_control_data.accumulator_fan_1_enable = enable;
}

void rco_data_set_accumulator_fan_2_enable(bool enable)
{
    g_rear_cooling_control_data.accumulator_fan_2_enable = enable;
}

void rco_data_set_tractive_fan_1_duty_cycle(float duty_cycle)
{
    g_rear_cooling_control_data.tractive_fan_1_duty_cycle = duty_cycle;
}

void rco_data_set_tractive_fan_2_duty_cycle(float duty_cycle)
{
    g_rear_cooling_control_data.tractive_fan_2_duty_cycle = duty_cycle;
}

void rco_data_set_accumulator_fan_1_duty_cycle(float duty_cycle)
{
    g_rear_cooling_control_data.accumulator_fan_1_duty_cycle = duty_cycle;
}

void rco_data_set_accumulator_fan_2_duty_cycle(float duty_cycle)
{
    g_rear_cooling_control_data.accumulator_fan_2_duty_cycle = duty_cycle;
}

bool rco_data_get_tractive_pump_1_enable(void)
{
    return g_rear_cooling_control_data.tractive_pump_1_enable;
}

bool rco_data_get_tractive_pump_2_enable(void)
{
    return g_rear_cooling_control_data.tractive_pump_2_enable;
}

bool rco_data_get_tractive_fan_1_enable(void)
{
    return g_rear_cooling_control_data.tractive_fan_1_enable;
}

bool rco_data_get_tractive_fan_2_enable(void)
{
    return g_rear_cooling_control_data.tractive_fan_2_enable;
}

bool rco_data_get_accumulator_fan_1_enable(void)
{
    return g_rear_cooling_control_data.accumulator_fan_1_enable;
}

bool rco_data_get_accumulator_fan_2_enable(void)
{
    return g_rear_cooling_control_data.accumulator_fan_2_enable;
}

float rco_data_get_tractive_fan_1_duty_cycle(void)
{
    return g_rear_cooling_control_data.tractive_fan_1_duty_cycle;
}

float rco_data_get_tractive_fan_2_duty_cycle(void)
{
    return g_rear_cooling_control_data.tractive_fan_2_duty_cycle;
}

float rco_data_get_accumulator_fan_1_duty_cycle(void)
{
    return g_rear_cooling_control_data.accumulator_fan_1_duty_cycle;
}

float rco_data_get_accumulator_fan_2_duty_cycle(void)
{
    return g_rear_cooling_control_data.accumulator_fan_2_duty_cycle;
}
