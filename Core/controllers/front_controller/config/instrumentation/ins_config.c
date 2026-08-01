/**
 * ins_config.c
 * 
 * Configuration for the instrumentation module V2.1
 * 
 * Created on: March 9, 2026
 *      Author: f-dunnwolbaum
 * 
 */

#include "ins_drv_ads124s08_regs.h"
#include "ins_config.h"
#include "ins_config_pinout.h"


// Definition of the shadow registers for each ADC, initialized to 0
// These will be updated when the driver layer does the default configuations
// and during operation to reflect the current state of the devices
static ads124s08_shawdow_t ads124s08_0_shadow = {0};
static ads124s08_shawdow_t ads124s08_1_shadow = {0};

ads124s08_hw_t ins_adc_array[INS_TOTAL_NUM_ADC] = {
    [SINGLE_ENDED_ADC] = {
        .spi_handle = INS_SPI_HANDLE,
        .cs_port = INS_CS_0_PORT,
        .cs_pin = INS_CS_0_PIN,
        .shadow = &ads124s08_0_shadow
    },
 
    [DIFFERENTIAL_ADC] = {
        .spi_handle = INS_SPI_HANDLE,
        .cs_port = INS_CS_1_PORT,
        .cs_pin = INS_CS_1_PIN,
        .shadow = &ads124s08_1_shadow
    }
};

/*============================================================================*/
/*Channel Configuration                                                       */
/*============================================================================*/

const ins_channel_t ins_default_config[INS_TOTAL_NUM_CHANNEL] = 
{
    [SINGLE_ENDED_0] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_1,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_1] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_2,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_2] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_0,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_3] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_3,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_4] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_8,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_5] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_9,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_6] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_10,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_7] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_11,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = false
    },

    [SINGLE_ENDED_8] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_7,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = true
    },

    [SINGLE_ENDED_9] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_5,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = true
    },

    [SINGLE_ENDED_10] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_6,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = true
    },

    [SINGLE_ENDED_11] = {
        .hw = &ins_adc_array[SINGLE_ENDED_ADC],
        .input_pos_pin = ADS124S08_INPUT_4,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
        .is_en = true
    },

    [DIFFERENTIAL_1] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_6,
        .input_neg_pin = ADS124S08_INPUT_7,
        .is_en = true
    },

    [DIFFERENTIAL_2] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_5,
        .input_neg_pin = ADS124S08_INPUT_4,
        .is_en = true
    },

    [DIFFERENTIAL_3] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_2,
        .input_neg_pin = ADS124S08_INPUT_3,
        .is_en = true
    },

    [DIFFERENTIAL_4] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_1,
        .input_neg_pin = ADS124S08_INPUT_0,
        .is_en = true
    },

    [DIFFERENTIAL_5] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_9,
        .input_neg_pin = ADS124S08_INPUT_8,
        .is_en = true
    },

    [DIFFERENTIAL_6] = {
        .hw = &ins_adc_array[DIFFERENTIAL_ADC],
        .input_pos_pin = ADS124S08_INPUT_10,
        .input_neg_pin = ADS124S08_INPUT_11,
        .is_en = false
    },

};
