#include "mcu_config_pinout.h"
#include "mcu_config.h"

#include "tcu_config.h"
#include "ins_config.h"

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



static mcu_adc_context_t adc_1_context = {
    .adc_handle = TCU_ADC_1_HANDLE,
    .adc_vref = TCU_ADC_1_VREF_V,
    .adc_max = TCU_ADC_1_MAX_COUNT,
    .calibrated = false,
    .dma_started = false,
    .adc_channels = 1
};

static mcu_adc_context_t adc_3_context = {
    .adc_handle = TCU_ADC_3_HANDLE,
    .adc_vref = TCU_ADC_3_VREF_V,
    .adc_max = TCU_ADC_3_MAX_COUNT,
    .calibrated = false,
    .dma_started = false,
    .adc_channels = 4
};

/*============================================================================*/
/* Analog Sensor Configuration Table                                          */
/*============================================================================*/

//TODO maybe split hw and context / dma stuff into seperate structs
const mcu_analog_config_t mcu_analog_config[MCU_ANALOG_SENSOR_COUNT] =
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
            .adc_buffer_number = TCU_AIN3_ADC_BUFFER_NUMBER,
            .adc_buffer_index = TCU_AIN3_ADC_BUFFER_INDEX,
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
            .adc_buffer_number = TCU_AIN4_ADC_BUFFER_NUMBER,
            .adc_buffer_index = TCU_AIN4_ADC_BUFFER_INDEX,
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
            .adc_context = &adc_1_context,
            .adc_buffer_number = TCU_AIN7_ADC_BUFFER_NUMBER,
            .adc_buffer_index = TCU_AIN7_ADC_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* Front Left Suspension Potentiometer                                    */
    /*------------------------------------------------------------------------*/
    [FL_SUSP] =
    {
        .settings =
        {
            .valid_min_v    = INS_FL_SUSP_VOLTAGE_MIN_V,
            .valid_max_v    = INS_FL_SUSP_VOLTAGE_MAX_V,
            .active_min_v   = INS_FL_SUSP_ACTIVE_MIN_V,
            .active_max_v   = INS_FL_SUSP_ACTIVE_MAX_V,
            .scaling_factor = INS_FL_SUSP_SCALING_FACTOR,
            .positive_slope = true,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = INS_AIN2_ADC_BUFFER_NUMBER,
            .adc_buffer_index = INS_AIN2_ADC_BUFFER_INDEX,
        },
        .enabled = true,
    },

    /*------------------------------------------------------------------------*/
    /* Front Left Suspension Potentiometer                                    */
    /*------------------------------------------------------------------------*/
    [FR_SUSP] =
    {
        .settings =
        {
            .valid_min_v    = INS_FR_SUSP_VOLTAGE_MIN_V,
            .valid_max_v    = INS_FR_SUSP_VOLTAGE_MAX_V,
            .active_min_v   = INS_FR_SUSP_ACTIVE_MIN_V,
            .active_max_v   = INS_FR_SUSP_ACTIVE_MAX_V,
            .scaling_factor = INS_FR_SUSP_SCALING_FACTOR,
            .positive_slope = true,
        },
        .hw = {
            .adc_context = &adc_3_context,
            .adc_buffer_number = INS_AIN1_ADC_BUFFER_NUMBER,
            .adc_buffer_index = INS_AIN1_ADC_BUFFER_INDEX,
        },
        .enabled = true,
    },
};
