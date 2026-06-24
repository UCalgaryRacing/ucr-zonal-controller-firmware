#ifndef RCO_SVC_DRS_H
#define RCO_SVC_DRS_H

#include <stdbool.h>

void rco_svc_update_drs();
void rco_svc_set_drs(bool open);
void rco_svc_toggle_drs();
void rco_svc_drs_init();

#define VEHICLE_SPEED_DRS_ACTIVATION_LIMIT 789

#endif