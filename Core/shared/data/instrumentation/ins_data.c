#include "ins_data.h"
#include <string.h>

static bool g_initialized;

static ins_wheel_speed_data_t g_ins_wheel_speed_data;
static ins_suspension_data_t g_ins_suspension_data;

static float g_steering_angle;

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/
static bool ins_data_is_valid_wheel_sensor(ins_sensor_id_t id);
static bool ins_data_is_valid_suspension_sensor(ins_sensor_id_t id);

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/
// TODO: add mutex protection
status_t ins_data_init(void)
{
    memset(&g_ins_wheel_speed_data, 0, sizeof(g_ins_wheel_speed_data));
    g_ins_wheel_speed_data.front_left.wheel_rpm = 0;
    g_ins_wheel_speed_data.front_right.wheel_rpm = 0;
    g_ins_wheel_speed_data.rear_left.wheel_rpm = 0;
    g_ins_wheel_speed_data.rear_right.wheel_rpm = 0;
    
    g_ins_wheel_speed_data.front_left.wheel_valid = false;
    g_ins_wheel_speed_data.front_right.wheel_valid = false;
    g_ins_wheel_speed_data.rear_left.wheel_valid = false;
    g_ins_wheel_speed_data.rear_right.wheel_valid = false;    
   
    g_ins_wheel_speed_data.front_left.wheel_period_ticks = 0U;
    g_ins_wheel_speed_data.front_right.wheel_period_ticks = 0U;
    g_ins_wheel_speed_data.rear_left.wheel_period_ticks = 0U;
    g_ins_wheel_speed_data.rear_right.wheel_period_ticks = 0U;

    memset(&g_ins_suspension_data, 0, sizeof(g_ins_suspension_data));
    g_ins_suspension_data.front_left_suspension = 0;
    g_ins_suspension_data.front_right_suspension = 0;
    g_ins_suspension_data.rear_left_suspension = 0;
    g_ins_suspension_data.rear_right_suspension = 0;

    g_steering_angle = 0;

    g_initialized = true;

    return OK;
}


/*============================================================================*/
/* Private Helper Functions                                                   */
/*============================================================================*/

static bool ins_data_is_valid_wheel_sensor(ins_sensor_id_t id)
{
    return (id < INS_WHEEL_SPEED_SENSOR_COUNT);
}

static bool ins_data_is_valid_suspension_sensor(ins_sensor_id_t id)
{
    return ((id >= INS_WHEEL_SPEED_SENSOR_COUNT) && (id < INS_TOTAL_NUM_SENSORS));
}

/*============================================================================*/
/* Wheel Speed Data Access                                                    */
/*============================================================================*/
//TODO: add mutex protection for these data accesses
/* Wheel Speed Setters*/

status_t ins_data_set_wheel_speed_rpm(ins_sensor_id_t wheel_sensor_id, float wheel_rpm)
{
    if (!ins_data_is_valid_wheel_sensor(wheel_sensor_id))
    {
        return ERROR_GENERAL;
    }

    if (wheel_sensor_id == FL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_left.wheel_rpm = wheel_rpm;
    }

    else if (wheel_sensor_id == FR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_right.wheel_rpm = wheel_rpm;
    }

    else if (wheel_sensor_id == RL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_left.wheel_rpm = wheel_rpm;
    }

    else if (wheel_sensor_id == RR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_right.wheel_rpm = wheel_rpm;
    }

    return OK;
}

status_t ins_data_set_wheel_period_ticks(ins_sensor_id_t wheel_sensor_id, uint32_t period_ticks) // no getter for now
{
    if (!ins_data_is_valid_wheel_sensor(wheel_sensor_id))
    {
        return ERROR_GENERAL;
    }

    if (wheel_sensor_id == FL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_left.wheel_period_ticks = period_ticks;
    }

    else if (wheel_sensor_id == FR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_right.wheel_period_ticks = period_ticks;
    }

    else if (wheel_sensor_id == RL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_left.wheel_period_ticks = period_ticks;
    }

    else if (wheel_sensor_id == RR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_right.wheel_period_ticks = period_ticks;
    }

    return OK;
}

status_t ins_data_set_wheel_speed_valid(ins_sensor_id_t wheel_sensor_id, bool valid) // no getter for now
{
    if (!ins_data_is_valid_wheel_sensor(wheel_sensor_id))
    {
        return ERROR_GENERAL;
    }

    if (wheel_sensor_id == FL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_left.wheel_valid = valid;
    }

    else if (wheel_sensor_id == FR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.front_right.wheel_valid = valid;
    }

    else if (wheel_sensor_id == RL_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_left.wheel_valid = valid;
    }

    else if (wheel_sensor_id == RR_WHEEL_SPEED)
    {
        g_ins_wheel_speed_data.rear_right.wheel_valid = valid;
    }

    return OK;
}

float ins_data_get_wheel_speed_rpm(ins_sensor_id_t wheel_sensor_id)
{
    if (!ins_data_is_valid_wheel_sensor(wheel_sensor_id))
    {
        return 0.0f;
    }

    if (wheel_sensor_id == FL_WHEEL_SPEED)
    {
        return g_ins_wheel_speed_data.front_left.wheel_rpm;
    }

    else if (wheel_sensor_id == FR_WHEEL_SPEED)
    {
        return g_ins_wheel_speed_data.front_right.wheel_rpm;
    }

    else if (wheel_sensor_id == RL_WHEEL_SPEED)
    {
        return g_ins_wheel_speed_data.rear_left.wheel_rpm;
    }

    else if (wheel_sensor_id == RR_WHEEL_SPEED)
    {
        return g_ins_wheel_speed_data.rear_right.wheel_rpm;
    }

    return 0.0f;
}


/*============================================================================*/
/* Suspension Data Access                                                     */
/*============================================================================*/
//TODO: add mutex protection for these data accesses

status_t ins_data_set_suspension_travel(ins_sensor_id_t suspension_sensor_id, float susp_travel) // TODO: specify unit for distance param
{
    if (!ins_data_is_valid_suspension_sensor(suspension_sensor_id))
    {
        return ERROR_GENERAL;
    }

    if (suspension_sensor_id == FL_SUSPENSION)
    {
        g_ins_suspension_data.front_left_suspension = susp_travel;
    }

    else if (suspension_sensor_id == FR_SUSPENSION)
    {
        g_ins_suspension_data.front_right_suspension = susp_travel;
    }

    else if (suspension_sensor_id == RL_SUSPENSION)
    {
        g_ins_suspension_data.rear_left_suspension = susp_travel;
    }

    else if (suspension_sensor_id == RR_SUSPENSION)
    {
        g_ins_suspension_data.rear_right_suspension = susp_travel;
    }

    return OK;
}

float ins_data_get_suspension_travel(ins_sensor_id_t suspension_sensor_id)
{
    if (!ins_data_is_valid_suspension_sensor(suspension_sensor_id))
    {
        return 0.0f;
    }

    if (suspension_sensor_id == FL_SUSPENSION)
    {
        return g_ins_suspension_data.front_left_suspension;
    }

    else if (suspension_sensor_id == FR_SUSPENSION)
    {
        return g_ins_suspension_data.front_right_suspension;
    }

    else if (suspension_sensor_id == RL_SUSPENSION)
    {
        return g_ins_suspension_data.rear_left_suspension;
    }

    else if (suspension_sensor_id == RR_SUSPENSION)
    {
        return g_ins_suspension_data.rear_right_suspension;
    }

    return 0.0f;}

/*============================================================================*/
/* Other Instrumentation Data Access                                          */
/*============================================================================*/

void ins_data_set_steering_angle(float steering_angle)
{
    g_steering_angle = steering_angle;
}

float ins_data_get_steering_angle(void)
{
    return g_steering_angle;
}