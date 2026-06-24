#ifndef MCO_SVC_CAN_TX_H_
#define MCO_SVC_CAN_TX_H_

#include <stdbool.h>

void mco_svc_can_tx_motor_request_data(float torque_request_percent,
                                       bool power_limit_active,
                                       bool traction_control_active);

#endif /* MCO_SVC_CAN_TX_H_ */
