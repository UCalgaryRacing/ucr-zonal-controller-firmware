/*
 * ins_drv_ads124s08.h
 *
 *  Created on: Apr 29, 2026
 *      Author: f-dunnwolbaum
 */

#ifndef DRIVER_INS_DRV_ADS124S08_H_
#define DRIVER_INS_DRV_ADS124S08_H_

#include "ins_config.h"
#include "ins_drv_ads124s08_regs.h"
#include <stdint.h>

void ins_drv_ads124s08_init();
void ins_drv_ads124s08_write_shadow(const ads124s08_hw_t *hw);
void ins_drv_ads124s08_read_shadow(ads124s08_hw_t *hw);
void ins_drv_shadow_init_default(ads124s08_hw_t * hw);
void ins_drv_ads124s08_read_data(const ads124s08_hw_t *hw, uint8_t *data_dest, uint16_t length);
void ins_drv_ads124s08_send_command(const ads124s08_hw_t *hw, uint8_t *command, uint16_t length);
void ins_drv_ads124s08_start_internal_calibration(ads124s08_hw_t *hw);
void ins_drv_ads124s08_start_conversion(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw);
int32_t ins_drv_ads124s08_format_data(uint8_t * data_buffer);

#endif /* DRIVER_INS_DRV_ADS124S08_H_ */
