#include "mco_svc_can_tx.h"

#include "can_svc_can.h"
#include "can_config.h"
#include "tcu_data.h"

void mco_svc_can_tx_motor_request_data(float torque_request_percent,
                                       bool power_limit_active,
                                       bool traction_control_active)
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = MOTOR_REQUEST_CAN_ID;
    msg.dlc = UCR_03_MOTOR_REQUEST_LENGTH;

    tcu_motor_request_t motor_request;
    tcu_data_get_motor_request(&motor_request);
    motor_request.torque_request_percent = torque_request_percent;
    tcu_data_set_motor_request(&motor_request);

    struct ucr_03_motor_request_t motor_request_can_msg;
    motor_request_can_msg.torque_request_percent = torque_request_percent;
    motor_request_can_msg.speed_request_percent =
        tcu_data_get_traction_control_bamocar_n_lim_percent();
    motor_request_can_msg.measured_slip_ratio =
        tcu_data_get_traction_control_measured_slip_ratio();
    motor_request_can_msg.desired_slip_ratio =
        tcu_data_get_traction_control_desired_slip_ratio();
    motor_request_can_msg.traction_control_active = (uint8_t)traction_control_active;
    motor_request_can_msg.power_limit_active = (uint8_t)power_limit_active;
    ucr_03_motor_request_pack(msg.data, &motor_request_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}
