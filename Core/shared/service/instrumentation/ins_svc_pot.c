#include "ins_svc_pot.h"
#include "ins_data.h"
#include "ins_svc_ads124s08.h"

//static float ins_svc_pots_steering_voltage_to_angle(float voltage)
//{
//    return;
//}

void ins_svc_update_rear_pots(ins_sensor_id_t left_sensor_id, ins_sensor_id_t right_sensor_id)
{
    const ins_channel_id_t *left_pot_channel_id = ins_svc_get_channel_id(left_sensor_id);
    const ins_channel_id_t *right_pot_channel_id = ins_svc_get_channel_id(right_sensor_id);
    
    ins_suspension_data_t suspension_data;

    if ((left_sensor_id < INS_TOTAL_NUM_SENSORS) && (right_sensor_id < INS_TOTAL_NUM_SENSORS))
    {
        float rl_pot_v = 0;
        float rr_pot_v = 0;

        ins_svc_ads124s08_get_analog_voltage(*left_pot_channel_id, &rl_pot_v);
        ins_svc_ads124s08_get_analog_voltage(*right_pot_channel_id, &rr_pot_v);

        suspension_data.rear_left_suspension = rl_pot_v;
        suspension_data.rear_right_suspension = rr_pot_v;
    }

    ins_data_set_suspension_travel(left_sensor_id, suspension_data.rear_left_suspension);
    ins_data_set_suspension_travel(right_sensor_id, suspension_data.rear_right_suspension);    

}

void ins_svc_update_front_pots(ins_sensor_id_t left_sensor_id, ins_sensor_id_t right_sensor_id)
{
    const ins_channel_id_t *left_pot_channel_id = ins_svc_get_channel_id(left_sensor_id);
    const ins_channel_id_t *right_pot_channel_id = ins_svc_get_channel_id(right_sensor_id);
    
    ins_suspension_data_t suspension_data;

    if ((left_sensor_id < INS_TOTAL_NUM_SENSORS) && (right_sensor_id < INS_TOTAL_NUM_SENSORS))
    {
        float fl_pot_v = 0;
        float fr_pot_v = 0;

        ins_svc_ads124s08_get_analog_voltage(*left_pot_channel_id, &fl_pot_v);
        ins_svc_ads124s08_get_analog_voltage(*right_pot_channel_id, &fr_pot_v);

        suspension_data.front_left_suspension = fl_pot_v;
        suspension_data.front_right_suspension = fr_pot_v;
    }

    ins_data_set_suspension_travel(left_sensor_id, suspension_data.front_left_suspension);
    ins_data_set_suspension_travel(right_sensor_id, suspension_data.front_right_suspension);    

}


void ins_svc_pots_update_steering_angle(void)
{
    const ins_channel_id_t *steering_pot_channel_id = ins_svc_get_channel_id(STEERING_POT);

    float steering_pot_v = 0;

    ins_svc_ads124s08_get_analog_voltage(*steering_pot_channel_id, &steering_pot_v);

    // TODO: calculate steering angle from pot voltage using private function 

    ins_data_set_steering_angle(steering_pot_v);
}
