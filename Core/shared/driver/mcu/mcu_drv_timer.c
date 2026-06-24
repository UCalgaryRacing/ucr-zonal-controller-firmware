#include "mcu_drv_timer.h"

static status_t mcu_drv_timer_map_status(HAL_StatusTypeDef hal_status)
{
    return (hal_status == HAL_OK) ? OK : ERROR_GENERAL;
}

status_t mcu_drv_timer_start_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    if (timer_handle == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    return mcu_drv_timer_map_status(HAL_TIM_IC_Start_IT(timer_handle, channel));
}

status_t mcu_drv_timer_stop_input_capture_it(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    if (timer_handle == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    return mcu_drv_timer_map_status(HAL_TIM_IC_Stop_IT(timer_handle, channel));
}

uint32_t mcu_drv_timer_get_counter(TIM_HandleTypeDef *timer_handle)
{
    if (timer_handle == NULL)
    {
        return 0U;
    }

    return __HAL_TIM_GET_COUNTER(timer_handle);
}

uint32_t mcu_drv_timer_get_capture(TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    if (timer_handle == NULL)
    {
        return 0U;
    }

    return HAL_TIM_ReadCapturedValue(timer_handle, channel);
}

bool mcu_drv_timer_is_handle(const TIM_HandleTypeDef *timer_handle, const TIM_HandleTypeDef *expected)
{
    return (timer_handle != NULL) && (expected != NULL) && (timer_handle == expected);
}

bool mcu_drv_timer_is_active_channel(const TIM_HandleTypeDef *timer_handle, uint32_t channel)
{
    if (timer_handle == NULL)
    {
        return false;
    }

    uint32_t active_channel = timer_handle->Channel;

    if (channel == TIM_CHANNEL_1)
    {
        return (active_channel == HAL_TIM_ACTIVE_CHANNEL_1);
    }
    if (channel == TIM_CHANNEL_2)
    {
        return (active_channel == HAL_TIM_ACTIVE_CHANNEL_2);
    }
    if (channel == TIM_CHANNEL_3)
    {
        return (active_channel == HAL_TIM_ACTIVE_CHANNEL_3);
    }
    if (channel == TIM_CHANNEL_4)
    {
        return (active_channel == HAL_TIM_ACTIVE_CHANNEL_4);
    }

    return false;
}
