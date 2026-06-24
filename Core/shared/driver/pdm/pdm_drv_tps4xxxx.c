#include "pdm_drv_tps4xxxx.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

static bool g_driver_initialized = false;

/*============================================================================*/
/* Driver Initialization                                                      */
/*============================================================================*/

int tps4xxxx_driver_init(void)
{
    if (g_driver_initialized)
    {
        return 0;
    }

    g_driver_initialized = true;
    return 0;
}

int tps4xxxx_device_init(const tps4xxxx_hw_t *hw, const tps4xxxx_config_t *settings)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    HAL_GPIO_WritePin(hw->diag_port, hw->diag_pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(hw->seh_port, hw->seh_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hw->sel_port, hw->sel_pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(hw->input_port, hw->input_pin, GPIO_PIN_RESET);

    return 0;
}

/*============================================================================*/
/* Channel Enable/Disable Control                                             */
/*============================================================================*/

int tps4xxxx_enable_channel(const tps4xxxx_hw_t *hw, bool enable)
{

    if (!g_driver_initialized)
    {
        return -1;
    }

    HAL_GPIO_WritePin(hw->input_port, hw->input_pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);

    return 0;
}

/*============================================================================*/
/* Diagnostic Configuration                                                   */
/*============================================================================*/

int tps4xxxx_enable_diag(const tps4xxxx_hw_t *hw, bool enable)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    HAL_GPIO_WritePin(hw->diag_port, hw->diag_pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);

    return 0;
}

/*============================================================================*/
/* Fault Status Reading                                                       */
/*============================================================================*/

bool tps4xxxx_read_fault(const tps4xxxx_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    /* FAULT is active LOW (open-drain from IC) */
    GPIO_PinState fault_state = HAL_GPIO_ReadPin(hw->fault_port, hw->fault_pin);
    return (fault_state == GPIO_PIN_RESET);
}

/*============================================================================*/
/* Current Sense Reading                                                      */
/*============================================================================*/

int tps4xxxx_read_current_sense_adc(ADC_HandleTypeDef *adc_handle, uint16_t *adc_value)
{
    if (!g_driver_initialized || adc_value == NULL)
    {
        return -1;
    }

    if (HAL_ADC_Start(adc_handle) != HAL_OK)
    {
        return -1;
    }

    if (HAL_ADC_PollForConversion(adc_handle, 10) != HAL_OK)
    {
        HAL_ADC_Stop(adc_handle);
        return -1;
    }

    *adc_value = (uint16_t)HAL_ADC_GetValue(adc_handle);

    HAL_ADC_Stop(adc_handle);

    return 0;
}

//need to add inlines / getter at service level to calculate this ratio to pass in 
int tps4xxxx_read_channel_current(const tps4xxxx_hw_t *hw, float ratio, float *current_a)
{
    if (!g_driver_initialized || current_a == NULL)
    {
        return -1;
    }

    uint16_t adc_value;
    if (tps4xxxx_read_current_sense_adc(hw->adc_handle, &adc_value) != 0)
    {
        return -1;
    }

    *current_a = tps4xxxx_adc_to_current(adc_value, ratio);

    return 0;
}

float tps4xxxx_adc_to_current(uint16_t adc_value, float ratio)
{
    /*
     * Convert ADC count to load current in Amperes.
     * All parameters (K_CS, R_CS) determined by IC type.
     *
     * Formula: I_OUT = ADC_value * (V_REF / ADC_MAX) * (K(CS) / R(CS))
     */

    return 0.0f;
    //return (float)adc_value * TPS4XXXX_ADC_VREF_V /  TPS4XXXX_ADC_MAX_COUNT * TPS4H160_ / r_cs;
}
