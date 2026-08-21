// #include "can_config.h"
// #include "com_typ_common.h"
// #include "ucr_03.h"
// #include "can_typ_common.h"
// #include "can_svc_can.h"
// #include "ins_data.h"
// #include "ins_svc_can_route.h"

// static void ins_svc_can_rx_front_wheel_speed_data(can_msg_t *msg);
// static bool g_initialized = false;

// status_t ins_svc_can_route_init(void)
// {
//     com_svc_can_register(FRONT_WHEEL_SPEED_CAN_ID , ins_svc_can_rx_front_wheel_speed_data);
//     g_initialized = true;
//     return OK;
// }


// void ins_svc_can_rx_front_wheel_speed_data(can_msg_t *msg)
// {
//     //received by front controller
//     struct ucr_03_front_wheel_speed_t front_wheel_speed_can_msg;
//     ucr_03_front_wheel_speed_unpack(&front_wheel_speed_can_msg, msg->data, msg->dlc);


//     ins_data_set_wheel_rpm(INS_WHEEL_SPEED_SENSOR_FRONT_LEFT, front_wheel_speed_can_msg.front_left_wheel_rpm);
// }
