/*
 * inst_svc_ads124s08.h
 *
 *  Created on: Apr 29, 2026
 *      Author: f-dunnwolbaum
 */

#ifndef SERVICE_INST_SVC_ADS124S08_H_
#define SERVICE_INST_SVC_ADS124S08_H_

#include "ins_config.h"
#include "ins_config_sensor_id.h"
#include <stdint.h>

#include "com_typ_common.h"

#define ADS124S08_SCALING_FACTOR 1.01686060601
#define ADS124S08_OFFSET_FACTOR  0.008060606060

status_t ins_svc_ads124s08_init(void);
status_t ins_svc_ads124s08_get_analog_voltage(const ins_channel_id_t channel_id, float *voltage);

const ins_channel_config_t *ins_svc_get_channel_config(ins_channel_id_t id);
const ins_channel_id_t *ins_svc_get_channel_id(ins_sensor_id_t id);

status_t ins_svc_ads124s08_read_gpio(ins_channel_id_t channel_id, bool *data);

void ins_svc_update_ads124s08(void); // for debugging, updates statically into ins_svc_ads124s08.c

#endif /* SERVICE_INST_SVC_ADS124S08_H_ */
