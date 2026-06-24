#include "rco_svc_logging.h"
#include "can_svc_can.h"
#include "rco_data.h"

void rco_svc_can_tx_rco_data(){
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = REAR_CONTROL_CAN_ID;
	msg.dlc = 1;

    rco_data_t rco_data;
    rco_data_get_data(&rco_data);

    struct ucr_03_rear_control_t rear_control_data;
    rear_control_data.brake_light = rco_data.brake_light_on;
    rear_control_data.drs = rco_data.drs_active;
    rear_control_data.reset_button = rco_data.reset_pressed;
    rear_control_data.tssi_faulted = rco_data.tssi_faulted;
    ucr_03_rear_control_pack(msg.data, &rear_control_data, msg.dlc);

    com_svc_can_transmit(&msg);
}


void rco_svc_can_tx_rco_cooling_data(){
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = REAR_COOLING_CONTROL_CAN_ID;
	msg.dlc = 20;

    rear_cooling_control_data_t rco_cooling_control_data;
    rco_data_get_cooling_data(&rco_cooling_control_data);



    struct ucr_03_rear_cooling_control_t rear_cooling_control_msg;

    rear_cooling_control_msg.accumulator_fan1_duty_cycle = 100;
    rear_cooling_control_msg.accumulator_fan2_duty_cycle = 100;
    
    rear_cooling_control_msg.accumulator_fan1_enable = rco_cooling_control_data.accumulator_fan_1_enable;
    rear_cooling_control_msg.accumulator_fan2_enable = rco_cooling_control_data.accumulator_fan_2_enable;

    rear_cooling_control_msg.tractive_fan1_duty_cycle = 100;
    rear_cooling_control_msg.tractive_fan2_duty_cycle = 100;

    rear_cooling_control_msg.tractive_fan1_enable = rco_cooling_control_data.tractive_fan_1_enable;
    rear_cooling_control_msg.tractive_fan2_enable = rco_cooling_control_data.tractive_fan_2_enable;

    rear_cooling_control_msg.tractive_pump1_enable = rco_cooling_control_data.tractive_pump_1_enable;
    rear_cooling_control_msg.tractive_pump2_enable = rco_cooling_control_data.tractive_pump_2_enable;


    ucr_03_rear_cooling_control_pack(msg.data, &rear_cooling_control_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}
