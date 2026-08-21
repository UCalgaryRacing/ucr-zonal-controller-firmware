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

/*============================================================================*/
/* ADC Configuration                                                          */
/*============================================================================*/

ads124s08_hw_t ins_adc_array[INS_TOTAL_NUM_ADC] = {
    [INS_ADC_1] = {
        .spi_handle = INS_SPI_HANDLE,
        .cs_port = INS_CS_0_PORT,
        .cs_pin = INS_CS_0_PIN,
        .shadow = &ads124s08_0_shadow,
        .is_en = true
    },
 
    [INS_ADC_2] = {
        .spi_handle = INS_SPI_HANDLE,
        .cs_port = INS_CS_1_PORT,
        .cs_pin = INS_CS_1_PIN,
        .shadow = &ads124s08_1_shadow,
        .is_en = false
    }
};

/*============================================================================*/
/*Channel Configuration                                                       */
/*============================================================================*/

const ins_channel_config_t ins_default_config[INS_TOTAL_NUM_CHANNEL] = 
{
    [INS_SING_0] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN1,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = false
    },

    [INS_SING_1] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN2,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = false
    },

    [INS_SING_2] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN0,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = false
    },

    [INS_SING_3] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN3,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = false
    },

    [INS_SING_4] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN8,
        .input_neg_pin = ADS124S08_AINCOM,
        .input_gpio_pin = GPIO0,
        .is_en = false
    },

    [INS_SING_5] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN9,
        .input_neg_pin = ADS124S08_AINCOM,
        .input_gpio_pin = GPIO1,
        .is_en = false
    },

    [INS_SING_6] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN10,
        .input_neg_pin = ADS124S08_AINCOM,
        .input_gpio_pin = GPIO2,
        .is_en = false
    },

    [INS_SING_7] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN11,
        .input_neg_pin = ADS124S08_AINCOM,
        .input_gpio_pin = GPIO3,
        .is_en = false
    },

    [INS_SING_8] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN7,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = true
    },

    [INS_SING_9] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN5,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = true
    },

    [INS_SING_10] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN6,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = true
    },

    [INS_SING_11] = {
        .hw = &ins_adc_array[INS_ADC_1],
        .input_pos_pin = ADS124S08_AIN4,
        .input_neg_pin = ADS124S08_AINCOM,
        .is_en = true
    },

    [INS_DIFF_1] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN6,
        .input_neg_pin = ADS124S08_AIN7,
        .is_en = true
    },

    [INS_DIFF_2] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN5,
        .input_neg_pin = ADS124S08_AIN4,
        .is_en = true
    },

    [INS_DIFF_3] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN2,
        .input_neg_pin = ADS124S08_AIN3,
        .is_en = true
    },

    [INS_DIFF_4] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN1,
        .input_neg_pin = ADS124S08_AIN0,
        .is_en = true
    },

    [INS_DIFF_5] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN9,
        .input_neg_pin = ADS124S08_AIN8,
        .is_en = true
    },

    [INS_DIFF_6] = {
        .hw = &ins_adc_array[INS_ADC_2],
        .input_pos_pin = ADS124S08_AIN10,
        .input_neg_pin = ADS124S08_AIN11,
        .is_en = true
    },

};

/*============================================================================*/
/* Sensor Mapping and Config                                                  */
/*============================================================================*/

const ins_sensor_config_t ins_sensor_config[INS_TOTAL_NUM_SENSORS] = 
{
    [FL_WHEEL_SPEED] = {
        .channel_id = INS_SING_11,
    },

    [FR_WHEEL_SPEED] = {
        .channel_id = INS_SING_8,
    },

    [FL_SUSPENSION] = {
        .channel_id = INS_SING_10,
    },

    [FR_SUSPENSION] = {
        .channel_id = INS_SING_9,
    }
};
