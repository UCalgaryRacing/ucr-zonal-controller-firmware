#include "ins_config.h"
#include "ins_config_pinout.h"

const ins_wheel_speed_sensor_config_t ins_wheel_speed_sensor_config[INS_TOTAL_NUM_SENSORS] = 
{
    [FL_WHEEL_SPEED] = 
    {
        .hw = 
        {
            .timer_handle = INS_FL_WHEEL_TIMER_HANDLE,
            .timer_channel = INS_FL_WHEEL_TIMER_CHANNEL,
        },
    },

    [FR_WHEEL_SPEED] = 
    {
        .hw = 
        {
            .timer_handle = INS_FR_WHEEL_TIMER_HANDLE,
            .timer_channel = INS_FR_WHEEL_TIMER_CHANNEL,
        },
    },
};

