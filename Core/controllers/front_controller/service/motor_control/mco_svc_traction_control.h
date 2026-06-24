#ifndef MCO_SVC_TRACTION_CONTROL_H_
#define MCO_SVC_TRACTION_CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

/* Slip-ratio N_LIM tuning — adjust on vehicle */
#define MCO_TC_N_LIM_ENGAGE_WHEEL_RPM      (30.0f)   /* Below: N_LIM held at 100% (launch deadband) */
#define MCO_TC_N_LIM_MIN_PERCENT           (20.0f)   /* Floor on computed N_LIM during low-speed rollout */
#define MCO_TC_N_LIM_FULL_TC_WHEEL_RPM     (50.0f)   /* Above: feedforward N_LIM used without floor */

void mco_svc_traction_control_init(void);

bool mco_svc_traction_control_set_current_limit_map(const float *rpm_breakpoints,
                                                    const float *current_limit_rms_a,
                                                    uint32_t table_size);
void mco_svc_traction_control_reset_current_limit_map(void);

float mco_svc_traction_control_calculate_max_torque_percent(void);

float mco_svc_traction_control_limit_torque_percent(float requested_torque_percent);
float mco_svc_traction_control_calculate_motor_speed_slip_ratio(void);
bool mco_svc_traction_control_update_bamocar_n_lim(float desired_slip_ratio);

#endif /* MCO_SVC_TRACTION_CONTROL_H_ */
