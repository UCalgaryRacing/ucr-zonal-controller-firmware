#ifndef SERVICE_TCU_SVC_LOGGING_H_
#define SERVICE_TCU_SVC_LOGGING_H_

void tcu_svc_can_tx_bse_data();
void tcu_svc_can_tx_apps_data();
void tcu_svc_can_tx_motor_data();
void tcu_svc_can_tx_motor_status_data();
void tcu_svc_can_tx_tcu_faults_data();
void tcu_svc_can_tx_traction_control_data();
void tcu_svc_can_tx_front_tcu_state();

#endif /* SERVICE_TCU_SVC_LOGGING_H_ */
