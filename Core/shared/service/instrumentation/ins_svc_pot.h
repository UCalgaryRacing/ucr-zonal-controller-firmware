#ifndef INS_SVC_POT_H_
#define INS_SVC_POT_H_

#include "ins_config_sensor_id.h"

void ins_svc_update_rear_pots(ins_sensor_id_t left_sensor_id, ins_sensor_id_t right_sensor_id);
void ins_svc_update_front_pots(ins_sensor_id_t left_sensor_id, ins_sensor_id_t right_sensor_id);

void ins_svc_pots_update_steering_angle(void);

#endif /*INS_SVC_POT_H_*/
