#include "ins_svc_logging.h"
#include "can_svc_can.h"
#include "ins_data.h"

void ins_svc_can_tx_wheel_speed_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
	msg.id = FRONT_WHEEL_SPEED_CAN_ID;
	msg.dlc = UCR_03_FRONT_WHEEL_SPEED_LENGTH;

    float FL_wheel_speed = ins_data_get_wheel_rpm(INS_WHEEL_SPEED_SENSOR_FRONT_LEFT);
    float FR_wheel_speed = ins_data_get_wheel_rpm(INS_WHEEL_SPEED_SENSOR_FRONT_RIGHT);

    struct ucr_03_front_wheel_speed_t front_wheel_speed_data_can_msg;
    front_wheel_speed_data_can_msg.front_left_wheel_rpm = FL_wheel_speed;
    front_wheel_speed_data_can_msg.front_right_wheel_rpm = FR_wheel_speed;
    ucr_03_front_wheel_speed_pack(msg.data, &front_wheel_speed_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}
