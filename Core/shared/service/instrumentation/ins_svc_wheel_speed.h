#ifndef INS_SVC_WHEEL_SPEED_H_
#define INS_SVC_WHEEL_SPEED_H_

#include "com_typ_common.h"
#include <stdint.h>

#include "ins_config_sensor_id.h"

status_t ins_svc_wheel_speed_init(void);

void ins_svc_wheel_speed_on_rising_edge(ins_sensor_id_t id);

void ins_svc_wheel_speed_update(ins_sensor_id_t id);

#endif /* INS_SVC_WHEEL_SPEED_H_ */
