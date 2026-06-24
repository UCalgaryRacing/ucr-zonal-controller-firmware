#ifndef TCU_SVC_FAULT_H
#define TCU_SVC_FAULT_H

void tcu_svc_fault_check_apps_plausibility(void);

void tcu_svc_fault_check_apps_bse_plausibility(void);

void tcu_svc_fault_check_apps_sensor_range(void);

void tcu_svc_fault_check_bse_sensor_range(void);

void tcu_svc_fault_evaluate(void);

#endif /* TCU_SVC_FAULT_H */
