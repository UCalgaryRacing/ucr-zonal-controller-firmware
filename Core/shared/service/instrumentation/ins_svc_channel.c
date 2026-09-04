#include "ins_svc_channel.h"
#include "ins_data.h"
#include "ins_svc_ads124s08.h"

void ins_svc_update_channel(ins_sensor_id_t sensor_id)
{
    float raw_voltage = 0;

    const ins_channel_id_t *channel_id =  ins_svc_get_channel_id(sensor_id);

    ins_svc_ads124s08_get_analog_voltage(*channel_id, &raw_voltage);

    // ins_data_set_wheel_speed_rpm(sensor_id, raw_voltage);
}

void ins_svc_update_channel_gpio(ins_sensor_id_t sensor_id)
{
    bool data = false;

    const ins_channel_id_t *channel_id =  ins_svc_get_channel_id(sensor_id);

    ins_svc_ads124s08_read_gpio(*channel_id, &data);

    // ins_data_set_wheel_speed_rpm(sensor_id, data);
}