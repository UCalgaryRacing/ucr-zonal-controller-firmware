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


static ads124s08_hw_t adc_0 = 
{
    .spi_handle = INS_SPI_HANDLE,
    .shadow = &ads124s08_0_shadow
};


/*============================================================================*/
/*Channel Configuration                                                       */
/*============================================================================*/

const instrumentation_channel_t instrumentation_default_config[INSTRUMENTATION_NUM_CHANNEL] = 
{
    [SINGLE_ENDED_0] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_1,
        .input_neg_pin = ADS124S08_INPUT_AINCOM,
    },

    [SINGLE_ENDED_1] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_2,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_2] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_0,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_3] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_3,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_4] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_8,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_5] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_9,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_6] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_10,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_7] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_11,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_8] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_7,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_9] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_5,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_10] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_6,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    },

    [SINGLE_ENDED_11] = {
        .hw = &adc_0,
        .input_pos_pin = ADS124S08_INPUT_4,
        .input_neg_pin = ADS124S08_INPUT_AINCOM
    }
};
