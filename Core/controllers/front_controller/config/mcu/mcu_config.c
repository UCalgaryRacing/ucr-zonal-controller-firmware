#include "mcu_config_pinout.h"
#include "mcu_config.h"
#include "tcu_config.h"
#include "mcu_svc_hsd.h"

#include "pdm_config.h"

/*============================================================================*/
/* HSD Configuration Table                                                    */
/*============================================================================*/
// currently not used in front controller 

/*============================================================================*/
/* ADC Context Configuration                                                  */
/*============================================================================*/

static mcu_adc_context_t adc_2_context = {
    .adc_handle = FCO_MCU_ADC_2_HANDLE,
    .adc_vref = MCU_ADC_2_VREF_V,
    .adc_max = MCU_ADC_2_MAX_COUNT,
    .calibrated = false,
    .dma_started = false,
    .adc_channels = 1
};

static mcu_adc_context_t adc_3_context = {
    .adc_handle = FCO_MCU_ADC_3_HANDLE,
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
const mcu_analog_config_t mcu_analog_config[MCU_TOTAL_ADC_INPUT_COUNT] =
{
    /*------------------------------------------------------------------------*/
    /* APPS Sensor 1 (Primary)                                                */
    /*------------------------------------------------------------------------*/
    [APPS1] =
    {
        .settings =
        {
            .valid_min_v    = TCU_APPS_1_VOLTAGE_MIN_V,
            .valid_max_v    = TCU_APPS_1_VOLTAGE_MAX_V,
            .active_min_v   = TCU_APPS_1_ACTIVE_MIN_V,
            .active_max_v   = TCU_APPS_1_ACTIVE_MAX_V,
            .scaling_factor = TCU_APPS_1_SCALING_FACTOR,
            .positive_slope = TCU_APPS_1_POSITIVE_SLOPE,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = ADC_3_1_BUFFER_NUMBER,
            .adc_buffer_index = ADC_3_1_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* APPS Sensor 2 (Secondary)                                              */
    /* Per FSAE T.4.2.3: Different transfer function from APPS1               */
    /*------------------------------------------------------------------------*/
    [APPS2] =
    {
        .settings =
        {
            .valid_min_v    = TCU_APPS_2_VOLTAGE_MIN_V,
            .valid_max_v    = TCU_APPS_2_VOLTAGE_MAX_V,
            .active_min_v   = TCU_APPS_2_ACTIVE_MIN_V,
            .active_max_v   = TCU_APPS_2_ACTIVE_MAX_V,
            .scaling_factor = TCU_APPS_2_SCALING_FACTOR,
            .positive_slope = TCU_APPS_2_POSITIVE_SLOPE,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = ADC_3_0_BUFFER_NUMBER,
            .adc_buffer_index = ADC_3_0_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* BSE Sensor (Brake System Encoder)                                      */
    /*------------------------------------------------------------------------*/
    [BSE1] =
    {
        .settings =
        {
            .valid_min_v    = TCU_BSE_1_VOLTAGE_MIN_V,
            .valid_max_v    = TCU_BSE_1_VOLTAGE_MAX_V,
            .active_min_v   = TCU_BSE_1_ACTIVE_MIN_V,
            .active_max_v   = TCU_BSE_1_ACTIVE_MAX_V,
            .scaling_factor = TCU_BSE_1_SCALING_FACTOR,
            .positive_slope = true,
        },
        .hw = {
            .adc_context = &adc_2_context,
            .adc_buffer_number = ADC_2_0_BUFFER_NUMBER,
            .adc_buffer_index = ADC_2_0_BUFFER_INDEX,
        },
        .enabled = true,
    },
};
