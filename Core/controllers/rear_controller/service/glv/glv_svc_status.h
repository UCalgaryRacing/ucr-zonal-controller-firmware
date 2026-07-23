#ifndef GLV_SVC_status_H_
#define GLV_SVC_status_H_

#include "mcu_config_analog_channels.h"
#include "com_typ_common.h"

float glv_svc_analog_update(mcu_analog_channel_id_t glv_channel_id);
void glv_svc_disable_glv(void);
void glv_svc_update_glv_status(void);
void glv_svc_apply_glv_plausability(void);

void glv_svc_init_soc(void);
void glv_svc_update_soc(void);

#endif /* GLV_SVC_status_H_ */
