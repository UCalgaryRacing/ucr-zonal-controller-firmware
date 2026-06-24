#ifndef DRIVERS_PDM_DRV_VNF9Q20F_H_
#define DRIVERS_PDM_DRV_VNF9Q20F_H_

#include <stdint.h>
#include <stdbool.h>
#include "pdm_drv_vnf9q20f_regs.h"
#include "pdm_config.h"

/*============================================================================*/
/* Driver Initialization                                                      */
/*============================================================================*/

int vnf9q20f_driver_init();
int vnf9q20f_device_init(const vnf9q20f_hw_t *hw, bool enter_normal);
int vnf9q20f_sync_shadow_to_device(const vnf9q20f_hw_t *hw);
int vnf9q20f_sync_device_to_shadow(const vnf9q20f_hw_t *hw);

/*============================================================================*/
/* Channel Enable/Disable Control                                             */
/*============================================================================*/

int vnf9q20f_enable_channel(const vnf9q20f_hw_t *hw, bool enable);

/*============================================================================*/
/* PWM Configuration                                                          */
/*============================================================================*/

int vnf9q20f_set_duty_cycle_percent(const vnf9q20f_hw_t *hw, uint8_t duty_percent);
int vnf9q20f_set_duty_cycle_raw(const vnf9q20f_hw_t *hw, uint16_t duty_raw);
uint8_t vnf9q20f_get_duty_cycle_percent(const vnf9q20f_hw_t *hw);
int vnf9q20f_set_pwm_frequency(const vnf9q20f_hw_t *hw, vnf9q20f_pwmfcy_t pwmfcy);
int vnf9q20f_set_channel_phase(const vnf9q20f_hw_t *hw, uint8_t phase_value);

/*============================================================================*/
/* Channel Mode Configuration                                                 */
/*============================================================================*/

int vnf9q20f_set_channel_mode(const vnf9q20f_hw_t *hw, vnf9q20f_mode_t mode);
vnf9q20f_mode_t vnf9q20f_get_channel_mode(const vnf9q20f_hw_t *hw);
int vnf9q20f_set_switching_slope(const vnf9q20f_hw_t *hw, vnf9q20f_slope_t slope);

/*============================================================================*/
/* I²t Protection Configuration                                               */
/*============================================================================*/

int vnf9q20f_set_i2t_protection(const vnf9q20f_hw_t *hw, vnf9q20f_inom_t inom, vnf9q20f_tnom_t tnom);
int vnf9q20f_get_i2t_protection(const vnf9q20f_hw_t *hw, vnf9q20f_inom_t *inom, vnf9q20f_tnom_t *tnom);

/*============================================================================*/
/* Current Sense Configuration and Reading                                    */
/*============================================================================*/

int vnf9q20f_set_current_sense_mode(const vnf9q20f_hw_t *hw, vnf9q20f_spcr_t spcr);
int vnf9q20f_read_current_sense(const vnf9q20f_hw_t *hw, uint16_t *adc_value);
float vnf9q20f_adc_to_current(const vnf9q20f_hw_t *hw, uint16_t adc_value);

/*============================================================================*/
/* Status and Fault Reading                                                   */
/*============================================================================*/

// int vnf9q20f_read_channel_faults(const vnf9q20f_hw_t *hw);
// int vnf9q20f_read_channel_info(const vnf9q20f_hw_t *hw);
// int vnf9q20f_read_channel_status(const vnf9q20f_hw_t *hw, uint16_t *status, uint8_t *gsb);
// int vnf9q20f_clear_channel_faults(const vnf9q20f_hw_t *hw);

/*============================================================================*/
/* Device Mode Control                                                        */
/*============================================================================*/

int vnf9q20f_software_reset(const vnf9q20f_hw_t *hw);
int vnf9q20f_enter_normal_mode(const vnf9q20f_hw_t *hw);
int vnf9q20f_enter_failsafe_mode(const vnf9q20f_hw_t *hw);

/*============================================================================*/
/* Standby Control (STDBY_NOT Pin)                                            */
/*============================================================================*/

int vnf9q20f_set_stdby_active(const vnf9q20f_hw_t *hw);
int vnf9q20f_set_stdby_standby(const vnf9q20f_hw_t *hw);
int vnf9q20f_toggle_stdby(const vnf9q20f_hw_t *hw);

/*============================================================================*/
/* Watchdog Control                                                           */
/*============================================================================*/

int vnf9q20f_toggle_watchdog(const vnf9q20f_hw_t *hw);

/*============================================================================*/
/* Low-Level Register Access (for advanced use)                               */
/*============================================================================*/

int vnf9q20f_write_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr, uint16_t data);
int vnf9q20f_read_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr,
                           uint16_t *data, uint8_t *gsb);

#endif /* DRIVERS_PDM_DRV_VNF9Q20F_H_ */
