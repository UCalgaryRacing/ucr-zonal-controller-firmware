#ifndef INS_SVC_LOGGING_H_
#define INS_SVC_LOGGING_H_

// rear instrumentation
void ins_svc_can_tx_rear_wheel_speed_data(void);
void ins_svc_can_tx_rear_suspension_data(void);

void ins_svc_can_tx_steering_angle_data(void);
void ins_svc_can_tx_coolant_temp_data(void);

// front instrumentation
void ins_svc_can_tx_front_wheel_speed_data(void);
void ins_svc_can_tx_front_suspension_data(void);

#endif /*INS_SVC_LOGGING_H_*/