/*
 * inst_svc_ads124s08.h
 *
 *  Created on: Apr 29, 2026
 *      Author: f-dunnwolbaum
 */

#ifndef SERVICE_INST_SVC_ADS124S08_H_
#define SERVICE_INST_SVC_ADS124S08_H_

#include "ins_config.h"
#include <stdint.h>

void ins_svc_ads124s08_init(ads124s08_hw_t *hw);

float ins_svc_ads124s08_read_channel(instrumentation_channel_t channel);
#endif /* SERVICE_INST_SVC_ADS124S08_H_ */
