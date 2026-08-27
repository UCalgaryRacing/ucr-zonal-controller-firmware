#include "ins_svc_logging.h"
#include "can_svc_can.h"
#include "ins_data.h"

void ins_svc_can_tx_front_wheel_speed_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = FRONT_WHEEL_SPEED_CAN_ID;
	msg.dlc = 8;

    float fl_wheel_speed_rpm = ins_data_get_wheel_speed_rpm(FL_WHEEL_SPEED);
    float fr_wheel_speed_rpm = ins_data_get_wheel_speed_rpm(FR_WHEEL_SPEED);

    struct ucr_03_front_wheel_speed_t front_wheel_speed_data_can_msg;

    front_wheel_speed_data_can_msg.front_left_wheel_rpm = fl_wheel_speed_rpm;
    front_wheel_speed_data_can_msg.front_right_wheel_rpm = fr_wheel_speed_rpm;

    ucr_03_front_wheel_speed_pack(msg.data, &front_wheel_speed_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void ins_svc_can_tx_front_suspension_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = FRONT_SUSPENSION_CAN_ID;
	msg.dlc = 8;

    float fl_suspension = ins_data_get_suspension_travel(FL_SUSPENSION);
    float fr_suspension = ins_data_get_suspension_travel(FR_SUSPENSION);

    struct ucr_03_front_suspension_t front_suspension_data_can_msg;

    front_suspension_data_can_msg.front_left_suspension = fl_suspension;
    front_suspension_data_can_msg.front_right_suspension = fr_suspension;

    ucr_03_front_suspension_pack(msg.data, &front_suspension_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}
