#include "ins_data.h"
#include <string.h>

typedef struct
{
    float wheel_rpm;
    bool wheel_valid;
    uint32_t wheel_period_ticks;
} ins_wheel_speed_data_t;

static ins_wheel_speed_data_t g_ins_wheel_speed_data[INS_WHEEL_SPEED_SENSOR_COUNT];

static bool ins_data_is_valid_sensor(ins_wheel_speed_sensor_t sensor)
{
    return (sensor >= INS_WHEEL_SPEED_SENSOR_FRONT_LEFT) && (sensor < INS_WHEEL_SPEED_SENSOR_COUNT);
}

status_t ins_data_init(void)
{
    memset(&g_ins_wheel_speed_data, 0, sizeof(g_ins_wheel_speed_data));
    return OK;
}

void ins_data_set_wheel_rpm(ins_wheel_speed_sensor_t sensor, float wheel_rpm)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return;
    }

    g_ins_wheel_speed_data[sensor].wheel_rpm = wheel_rpm;
}

float ins_data_get_wheel_rpm(ins_wheel_speed_sensor_t sensor)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return 0.0f;
    }

    return g_ins_wheel_speed_data[sensor].wheel_rpm;
}

void ins_data_set_wheel_valid(ins_wheel_speed_sensor_t sensor, bool valid)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return;
    }

    g_ins_wheel_speed_data[sensor].wheel_valid = valid;
}

bool ins_data_get_wheel_valid(ins_wheel_speed_sensor_t sensor)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return false;
    }

    return g_ins_wheel_speed_data[sensor].wheel_valid;
}

void ins_data_set_wheel_period_ticks(ins_wheel_speed_sensor_t sensor, uint32_t period_ticks)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return;
    }

    g_ins_wheel_speed_data[sensor].wheel_period_ticks = period_ticks;
}

uint32_t ins_data_get_wheel_period_ticks(ins_wheel_speed_sensor_t sensor)
{
    if (!ins_data_is_valid_sensor(sensor))
    {
        return 0U;
    }

    return g_ins_wheel_speed_data[sensor].wheel_period_ticks;
}
