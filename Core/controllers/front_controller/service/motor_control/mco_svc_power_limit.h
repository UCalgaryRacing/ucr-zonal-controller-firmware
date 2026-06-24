#ifndef MCO_SVC_POWER_LIMIT_H_
#define MCO_SVC_POWER_LIMIT_H_

void mco_svc_power_limit_init(void);
void mco_svc_power_limit_set_limit_w(float power_limit_w);
float mco_svc_power_limit_get_limit_w(void);
float mco_svc_power_limit_calculate_max_torque_percent1(void);
float mco_svc_power_limit_calculate_max_torque_percent2(void);
float mco_svc_power_limit_limit_torque_percent(float requested_torque_percent);

#endif /* MCO_SVC_POWER_LIMIT_H_ */
