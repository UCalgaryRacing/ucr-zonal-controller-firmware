#include "mcu_config_pinout.h"
#include "mcu_config.h"
#include "glv_config.h"
#include "mcu_svc_hsd.h"
//TODO is this fine to couple configs with shared types like this or should move to seperate shared types?
#include "pdm_config.h"



const mcu_channel_config_t mcu_hsd_default_config[MCU_HSD_TOTAL_CHANNELS] =
{
    [DOUT_1] = {

        .enabled     = true,
        .hw = {
                .input_port   = MCU_TPS4XXXX_1_IN1_GPIO_Port,
                .input_pin    = MCU_TPS4XXXX_1_IN1_Pin,
            }
        },
    [DOUT_2] = {

        .enabled     = true,
        .hw = {
                .input_port   = MCU_TPS4XXXX_1_IN2_GPIO_Port,
                .input_pin    = MCU_TPS4XXXX_1_IN2_Pin,
            }
        },
    [DOUT_3] = {

        .enabled     = true,
        .hw = {
                .input_port   = MCU_TPS4XXXX_1_IN3_GPIO_Port,
                .input_pin    = MCU_TPS4XXXX_1_IN3_Pin,
            }
        },
    [DOUT_4] = {

        .enabled     = true,
        .hw = {
                .input_port   = MCU_TPS4XXXX_1_IN4_GPIO_Port,
                .input_pin    = MCU_TPS4XXXX_1_IN4_Pin,
            }
        },
};



// static mcu_adc_context_t adc_1_context = {
//     .adc_handle = TCU_ADC_1_HANDLE,
//     .adc_vref = MCU_ADC_1_VREF_V,
//     .adc_max = MCU_ADC_1_MAX_COUNT,
//     .calibrated = false,
//     .dma_started = false,
//     .adc_channels = 1
// };

static mcu_adc_context_t adc_3_context = {
    .adc_handle = TCU_ADC_3_HANDLE,
    .adc_vref = MCU_ADC_3_VREF_V,
    .adc_max = MCU_ADC_3_MAX_COUNT,
    .calibrated = false,
    .dma_started = false,
    .adc_channels = 2
};

/*============================================================================*/
/* Analog Sensor Configuration Table                                          */
/*============================================================================*/

//TODO maybe split hw and context / dma stuff into seperate structs
//TODO should these defines be named more generically?
const mcu_analog_config_t mcu_analog_config[MCU_ANALOG_SENSOR_COUNT] =
{
    /*------------------------------------------------------------------------*/
    /* APPS Sensor 1 (Primary)                                                */
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
            .adc_buffer_number = TCU_AIN3_ADC_BUFFER_NUMBER,
            .adc_buffer_index = TCU_AIN3_ADC_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* APPS Sensor 2 (Secondary)                                              */
    /* Per FSAE T.4.2.3: Different transfer function from APPS1               */
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
            .adc_buffer_number = TCU_AIN4_ADC_BUFFER_NUMBER,
            .adc_buffer_index = TCU_AIN4_ADC_BUFFER_INDEX,
        },
        .enabled = true,
    },
};
