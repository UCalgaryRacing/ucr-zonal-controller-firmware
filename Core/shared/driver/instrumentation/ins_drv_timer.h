#ifndef DRIVER_INS_DRV_TIMER_H_
#define DRIVER_INS_DRV_TIMER_H_

#include <stdbool.h>
#include <stdint.h>
#include "com_typ_common.h"
#include "stm32h7xx_hal.h"

status_t ins_drv_timer_start_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel);

status_t ins_drv_timer_stop_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel);

uint32_t ins_drv_timer_get_counter(TIM_HandleTypeDef *timer_handle);

uint32_t ins_drv_timer_get_capture(TIM_HandleTypeDef *timer_handle, uint32_t channel);

bool ins_drv_timer_is_handle(const TIM_HandleTypeDef *timer_handle, const TIM_HandleTypeDef *expected);

bool ins_drv_timer_is_active_channel(const TIM_HandleTypeDef *timer_handle, uint32_t channel);

#endif /* DRIVER_INS_DRV_TIMER_H_ */
