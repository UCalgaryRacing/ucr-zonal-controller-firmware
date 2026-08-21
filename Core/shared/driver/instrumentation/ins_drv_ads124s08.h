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

#include <com_typ_common.h>

#define ADS124S08_SFOCAL_DELAY_MS               5U
#define ADS124S08_TD_RSSC_SETTLING_TIME_MS      10U

// shadow functions
void ins_drv_ads124s08_shadow_init_default(const ads124s08_hw_t *hw);
status_t ins_drv_ads124s08_write_shadow(const ads124s08_hw_t *hw);
status_t ins_drv_ads124s08_read_shadow(const ads124s08_hw_t *hw);

// reading
status_t ins_drv_ads124s08_start_internal_calibration(const ads124s08_hw_t *hw);
status_t ins_drv_ads124s08_start_conversion(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw);
status_t ins_drv_ads124s08_read_channel(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw, uint8_t *raw_data_buffer);

status_t ins_drv_ads124s08_read_gpio(const ads124s08_hw_t *hw, const ads124s08_input_mux_t gpio_pin, bool *data);

// initialization
void ins_drv_ads124s08_cs_high(const ads124s08_hw_t *hw);
void ins_drv_ads124s08_reset_module(void);
status_t ins_drv_ads124s08_init_device(const ads124s08_hw_t *hw);

#endif /* DRIVER_INS_DRV_ADS124S08_H_ */
