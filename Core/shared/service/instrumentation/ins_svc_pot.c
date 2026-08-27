#include "ins_svc_pot.h"
#include "ins_data.h"
#include "ins_svc_channel.h"

void ins_svc_update_pots(ins_sensor_id_t left_sensor_id, ins_sensor_id_t right_sensor_id)
{
    const ins_channel_id_t *left_pot_channel_id = ins_svc_get_channel_id(left_sensor_id);
    const ins_channel_id_t *right_pot_channel_id = ins_svc_get_channel_id(right_sensor_id);
    
    ins_suspension_data_t suspension_data;

    if ((left_sensor_id != NULL) && (right_sensor_id != NULL))
    {
        float rl_pot_v = 0;
        float rr_pot_v = 0;

        ins_svc_get_analog_voltage(*left_pot_channel_id, &rl_pot_v);
        ins_svc_get_analog_voltage(*right_pot_channel_id, &rr_pot_v);

        suspension_data.rear_left_suspension = rl_pot_v;
        suspension_data.rear_right_suspension = rr_pot_v;
    }

    ins_data_set_suspension_travel(left_sensor_id, suspension_data.rear_left_suspension);
    ins_data_set_suspension_travel(right_sensor_id, suspension_data.rear_right_suspension);    

}