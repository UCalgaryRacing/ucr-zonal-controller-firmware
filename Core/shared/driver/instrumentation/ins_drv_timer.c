#include "ins_drv_timer.h"
#include "mcu_drv_timer.h"

status_t ins_drv_timer_start_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    return mcu_drv_timer_start_input_capture_it(timer_handle, channel);
}

status_t ins_drv_timer_stop_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    return mcu_drv_timer_stop_input_capture_it(timer_handle, channel);
}

uint32_t ins_drv_timer_get_counter(TIM_HandleTypeDef *timer_handle)
{
    return mcu_drv_timer_get_counter(timer_handle);
}

uint32_t ins_drv_timer_get_capture(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    return mcu_drv_timer_get_capture(timer_handle, channel);
}

bool ins_drv_timer_is_handle(const TIM_HandleTypeDef *timer_handle, const TIM_HandleTypeDef *expected)
{
    return mcu_drv_timer_is_handle(timer_handle, expected);
}

bool ins_drv_timer_is_active_channel(const TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    return mcu_drv_timer_is_active_channel(timer_handle, channel);
}
