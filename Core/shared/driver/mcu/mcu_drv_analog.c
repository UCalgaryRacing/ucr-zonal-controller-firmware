#include "mcu_drv_analog.h"

static uint16_t adc_dma_buffers[MCU_TOTAL_ANALOG_CHANNELS][MCU_MAX_ADC_CHANNELS];

static bool g_driver_initialized = false;

status_t mcu_drv_analog_driver_init(void)
{
    if (g_driver_initialized)
    {
        return OK;
    }

    g_driver_initialized = true;
    return OK;
}

int mcu_drv_analog_init(const analog_hw_t *hw, const mcu_analog_settings_t *settings)
{
    if (hw == NULL || settings == NULL)
    {
        return ERROR_GENERAL;
    }

    if (hw->adc_context->calibrated == false)
    {
        if (HAL_ADCEx_Calibration_Start(hw->adc_context->adc_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
        {
            return ERROR_GENERAL;
        }

        hw->adc_context->calibrated = true;
    }

    return OK;
}

status_t mcu_drv_analog_start_adc(const analog_hw_t *hw)
{
    if (hw == NULL)
    {
        return ERROR_GENERAL;
    }

    if (hw->adc_context->dma_started == false)
    {
        if (HAL_ADC_Start_DMA(hw->adc_context->adc_handle,
                              (uint32_t *)adc_dma_buffers[hw->adc_buffer_number],
                              hw->adc_context->adc_channels) != HAL_OK)
        {
            return ERROR_GENERAL;
        }

        hw->adc_context->dma_started = true;
    }

    return OK;
}

status_t mcu_drv_analog_read_raw(const analog_hw_t *hw, uint16_t *adc_value)
{
    if (hw == NULL || adc_value == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    *adc_value = adc_dma_buffers[hw->adc_buffer_number][hw->adc_buffer_index];

    return OK;
}
