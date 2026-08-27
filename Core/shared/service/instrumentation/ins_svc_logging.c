#include "ins_svc_logging.h"
#include "can_svc_can.h"
#include "ins_data.h"

void ins_svc_can_tx_rear_wheel_speed_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = REAR_WHEEL_SPEED_CAN_ID;
	msg.dlc = 8;

    float rl_wheel_speed_rpm = ins_data_get_wheel_speed_rpm(RL_WHEEL_SPEED);
    float rr_wheel_speed_rpm = ins_data_get_wheel_speed_rpm(RR_WHEEL_SPEED);

    struct ucr_03_rear_wheel_speed_t rear_wheel_speed_data_can_msg;

    rear_wheel_speed_data_can_msg.rear_left_wheel_rpm = rl_wheel_speed_rpm;
    rear_wheel_speed_data_can_msg.rear_right_wheel_rpm = rr_wheel_speed_rpm;

    ucr_03_rear_wheel_speed_pack(msg.data, &rear_wheel_speed_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void ins_svc_can_tx_rear_suspension_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = REAR_SUSPENSION_CAN_ID;
	msg.dlc = 8;

    float rl_suspension = ins_data_get_suspension_travel(RL_SUSPENSION);
    float rr_suspension = ins_data_get_suspension_travel(RR_SUSPENSION);

    struct ucr_03_rear_suspension_t rear_suspension_data_can_msg;

    rear_suspension_data_can_msg.rear_left_suspension_potentiometer = rl_suspension;
    rear_suspension_data_can_msg.rear_right_suspension_potentiometer = rr_suspension;

    ucr_03_front_suspension_pack(msg.data, &rear_suspension_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}
