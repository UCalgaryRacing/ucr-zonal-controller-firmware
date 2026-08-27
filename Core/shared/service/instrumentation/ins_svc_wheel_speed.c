#include "ins_svc_wheel_speed.h"
#include "ins_data.h"
#include "ins_svc_channel.h"

void ins_svc_update_wheel_speed(ins_sensor_id_t sensor_id)
{
    float wheel_speed = 0;

    const ins_channel_id_t *channel_id =  ins_svc_get_channel_id(sensor_id);

    ins_svc_get_analog_voltage(*channel_id, &wheel_speed);

    ins_data_set_wheel_speed_rpm(sensor_id, wheel_speed);
}

void ins_svc_update_wheel_speed_gpio(ins_sensor_id_t sensor_id)
{
    bool data = false;

    const ins_channel_id_t *channel_id =  ins_svc_get_channel_id(sensor_id);

    ins_svc_read_gpio(*channel_id, &data);

    ins_data_set_wheel_speed_rpm(sensor_id, data);
}