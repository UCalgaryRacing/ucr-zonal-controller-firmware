#ifndef INS_SVC_WHEEL_SPEED_H_
#define INS_SVC_WHEEL_SPEED_H_

#include "com_typ_common.h"

status_t ins_svc_wheel_speed_init(void);

void ins_svc_wheel_speed_on_rising_edge(void);

void ins_svc_wheel_speed_update(void);

#endif /* INS_SVC_WHEEL_SPEED_H_ */
