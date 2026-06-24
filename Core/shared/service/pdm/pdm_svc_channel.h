#ifndef SERVICES_PDM_SVC_CHANNEL_H_
#define SERVICES_PDM_SVC_CHANNEL_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"
#include "pdm_config.h"


status_t pdm_svc_channel_update(pdm_channel_id_t channel_id);

/*============================================================================*/
/* Service Initialization                                                     */
/*============================================================================*/

status_t pdm_svc_channel_init(pdm_channel_id_t channel_id);

/*============================================================================*/
/* Channel Enable/Disable                                                     */
/*============================================================================*/

status_t pdm_svc_channel_enable(pdm_channel_id_t channel_id, bool enable);
bool pdm_svc_channel_is_enabled(pdm_channel_id_t channel_id);
status_t pdm_svc_channel_enable_all(void);
status_t pdm_svc_channel_disable_all(void);

/*============================================================================*/
/* PWM Control                                                                */
/*============================================================================*/

status_t pdm_svc_channel_set_duty(pdm_channel_id_t channel_id, uint8_t duty_percent);
uint8_t pdm_svc_channel_get_duty(pdm_channel_id_t channel_id);

/*============================================================================*/
/* Channel Mode Configuration                                                 */
/*============================================================================*/

status_t pdm_svc_channel_set_mode(pdm_channel_id_t channel_id, pdm_channel_mode_t mode);

/*============================================================================*/
/* Current and Power Monitoring                                               */
/*============================================================================*/
status_t pdm_svc_glv_readings_init(void);
status_t pdm_svc_read_glv_voltage(float *voltage_v);
status_t pdm_svc_read_glv_current(float *current_a);
status_t pdm_svc_read_glv_power(float *power_w);
status_t pdm_svc_read_pdm_temperature(float *temperature_c);
status_t pdm_svc_read_glv_charge(float *charge_c);
status_t pdm_svc_read_glv_energy(float *energy_wh);
status_t pdm_svc_update_glv_data(void);

/*============================================================================*/
/* Current and Power Monitoring                                               */
/*============================================================================*/

status_t pdm_svc_channel_read_current(pdm_channel_id_t channel_id, float *current_a);

/*============================================================================*/
/* Fault Management                                                           */
/*============================================================================*/

status_t pdm_svc_channel_clear_faults(pdm_channel_id_t channel_id);
bool pdm_svc_channel_any_fault(void);

/*============================================================================*/
/* Configuration Application                                                  */
/*============================================================================*/

status_t pdm_svc_channel_apply_config(pdm_channel_id_t channel_id,const pdm_channel_config_t *config);
status_t pdm_svc_channel_apply_all_config(void);

typedef struct
{
    bool enabled;

    //currently these settings probably dont change from config defaults, except maybe pwm
    union
    {
        tps4xxxx_config_t tps4xxxx_config;
        vnf9q20f_config_t vnf9q20f_config;
    } device_settings;

} pdm_channel_runtime_config_t;

extern pdm_channel_runtime_config_t pdm_runtime_config[PDM_TOTAL_CHANNELS];

#endif /* SERVICES_PDM_SVC_CHANNEL_H_ */
