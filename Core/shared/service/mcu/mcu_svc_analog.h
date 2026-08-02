#ifndef MCU_SVC_ANALOG_H_
#define MCU_SVC_ANALOG_H_

#include <stdbool.h>
#include <stdint.h>

#include "com_typ_common.h"
#include "mcu_config.h"
#include "utilities.h"

status_t mcu_svc_analog_init(mcu_analog_channel_id_t channel_id);
status_t mcu_svc_analog_start(mcu_analog_channel_id_t channel_id);
status_t mcu_svc_get_analog_voltage(mcu_analog_channel_id_t channel_id, float *voltage);
status_t mcu_svc_get_analog_percent(mcu_analog_channel_id_t channel_id, float *percentage);
status_t mcu_svc_voltage_to_percent(const mcu_analog_config_t *channel_config, float voltage, float *percentage);
const mcu_analog_config_t* mcu_svc_get_channel_config(mcu_analog_channel_id_t channel_id);

typedef struct
{
    bool enabled;
    //add fault stuff here
} mcu_analog_runtime_config_t;

#endif /* MCU_SVC_ANALOG_H_ */
