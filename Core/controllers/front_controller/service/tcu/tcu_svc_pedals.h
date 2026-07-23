#ifndef TCU_SVC_PEDALS_H_
#define TCU_SVC_PEDALS_H_

#include "com_typ_common.h"
#include "mcu_config_analog_channels.h"
#include "tcu_data.h"

void tcu_svc_apps_update(mcu_analog_channel_id_t apps_1_channel_id, mcu_analog_channel_id_t apps_2_channel_id);
void tcu_svc_bse_update(mcu_analog_channel_id_t bse_1_channel_id);

status_t tcu_svc_pedal_get_apps_data(tcu_apps_data_t *apps_data);
float tcu_svc_pedal_get_apps_percent(void);
bool tcu_svc_pedal_is_apps_plausible(void);
bool tcu_svc_pedal_is_apps_fault_active(void);
tcu_sensor_validity_t tcu_svc_pedal_get_apps1_validity(void);
tcu_sensor_validity_t tcu_svc_pedal_get_apps2_validity(void);
void tcu_svc_pedal_clear_apps_fault(void);

status_t tcu_svc_pedal_get_bse_data(tcu_bse_data_t *bse_data);
float tcu_svc_pedal_get_bse_percent(void);
float tcu_svc_pedal_get_bse_pressure_psi(void);
bool tcu_svc_pedal_is_braking(void);
bool tcu_svc_pedal_is_hard_braking(void);
tcu_sensor_validity_t tcu_svc_pedal_get_bse_validity(void);
bool tcu_svc_pedal_is_bse_fault_active(void);
void tcu_svc_pedal_clear_bse_fault(void);

#endif /* TCU_SVC_PEDALS_H_ */
