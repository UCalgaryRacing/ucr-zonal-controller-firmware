#include "mcu_config_pinout.h"
#include "mcu_config.h"
#include "glv_config.h"
#include "mcu_svc_hsd.h"

#include "pdm_config.h"



const mcu_channel_config_t mcu_hsd_default_config[MCU_HSD_TOTAL_CHANNELS] =
{
    [ACCU_FAN_PWM] = {

        .enabled     = true,
        .hw = {
                .input_port   = HSD_5V_FAST_1_CTL_GPIO_Port,
                .input_pin    = HSD_5V_FAST_1_CTL_GPIO_Pin,
            }
        },
    [TS_FAN_PWM] = {

        .enabled     = true,
        .hw = {
                .input_port   = HSD_5V_FAST_2_CTL_GPIO_Port,
                .input_pin    = HSD_5V_FAST_2_CTL_GPIO_Pin,
            }
        },
    [BAMOCAR_RFE] = {

        .enabled     = true,
        .hw = {
                .input_port   = HSD_12V_1_CTL_GPIO_Port,
                .input_pin    = HSD_12V_1_CTL_GPIO_Pin,
            }
        },
    [BAMOCAR_FRG] = {

        .enabled     = true,
        .hw = {
                .input_port   = HSD_12V_2_CTL_GPIO_Port,
                .input_pin    = HSD_12V_2_CTL_GPIO_Pin,
            }
        },
    [DRS_PULSE] = {

        .enabled     = true,
        .hw = {
                .input_port   = HSD_5V_SLOW_CTL_GPIO_Port,
                .input_pin    = HSD_5V_SLOW_CTL_GPIO_Pin,
            }
        },
};

/*============================================================================*/
/* ADC Context                                                                */
/*============================================================================*/
//TODO: should this be shared? also should buffer number/index be tied to the context, so that you can't accidentally put the wrong number?
// adc_channels should probably be a macro
static mcu_adc_context_t adc_3_context = {
    .adc_handle = RCO_ADC_3_HANDLE,
    .adc_vref = MCU_ADC_3_VREF_V,
    .adc_max = MCU_ADC_3_MAX_COUNT,
    .calibrated = false,
    .dma_started = false,
    .adc_channels = 2
};

/*============================================================================*/
/* Analog Sensor Configuration Table                                          */
/*============================================================================*/

const mcu_analog_config_t mcu_analog_config[MCU_TOTAL_ADC_INPUT_COUNT] =
{
    /*------------------------------------------------------------------------*/
    /* Battery Highest Temperature (GLV Board Logic)                          */
    /*------------------------------------------------------------------------*/
    [BATT_HIGHEST_TEMP] =
    {
        .settings =
        {
            .valid_min_v    = GLV_CELL_TEMP_VOLTAGE_MIN_V,
            .valid_max_v    = GLV_CELL_TEMP_VOLTAGE_MAX_V,
            .active_min_v   = GLV_CELL_TEMP_ACTIVE_MIN_V,
            .active_max_v   = GLV_CELL_TEMP_ACTIVE_MAX_V,
            .scaling_factor = GLV_CELL_TEMP_SCALING_FACTOR,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = ADC_3_1_BUFFER_NUMBER,
            .adc_buffer_index = ADC_3_1_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* Battery Lowest Voltage (GLV Board Logic)                               */
    /*------------------------------------------------------------------------*/
    [BATT_LOWEST_VOLTAGE] =
    {
        .settings =
        {
            .valid_min_v    = GLV_CELL_VOLTAGE_MIN_V,
            .valid_max_v    = GLV_CELL_VOLTAGE_MAX_V,
            .active_min_v   = GLV_CELL_VOLTAGE_ACTIVE_MIN_V,
            .active_max_v   = GLV_CELL_VOLTAGE_ACTIVE_MAX_V,
            .scaling_factor = GLV_CELL_VOLTAGE_SCALING_FACTOR,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = ADC_3_0_BUFFER_NUMBER,
            .adc_buffer_index = ADC_3_0_BUFFER_INDEX,
        },
        .enabled = true,
    },
};
