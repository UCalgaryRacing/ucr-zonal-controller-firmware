/*
 * ins_svc_ads124s08.c
 *
 *  Created on: Apr 29, 2026
 *      Author: f-dunnwolbaum
 */

#include "ins_svc_ads124s08.h"
#include "ins_drv_ads124s08.h"
#include "ins_drv_ads124s08_regs.h"
#include "ins_config.h"
#include "ins_config_sensor_id.h"
#include <string.h>
#include <stdbool.h>
#include "cmsis_os2.h"

static float ins_array_values[INS_TOTAL_NUM_CHANNEL];

/*============================================================================*/
/* Private Function Prototype                                                 */
/*============================================================================*/
static int32_t ins_svc_raw_to_int(uint8_t *data_buffer);
static float ins_svc_int_to_scaled_voltage(int32_t raw_int);

/*============================================================================*/
/* Private Function Definitions                                               */
/*============================================================================*/

static int32_t ins_svc_raw_to_int(uint8_t *data_buffer)
{   
    int32_t raw_int;
    
    // data is recieved in three bytes and is 24 bit 2's compliment, must convert to 32 bit int from buffer
    // if the data is negative, then front pad with 1 so that the data can be cast to an int 
    if((data_buffer[0] & 0x80))
    {
        raw_int = (int32_t) ((0xFF<<24) | (data_buffer[0] << 16) | (data_buffer[1] << 8) | data_buffer[2]);
    }
    else
    {
        raw_int = (int32_t) ((data_buffer[0] << 16) | (data_buffer[1] << 8) | data_buffer[2]);
    }

    return raw_int;
}

static float ins_svc_int_to_scaled_voltage(int32_t raw_int)
{
    return ((float) raw_int * INS_SCALING_FACTOR * INS_EXTERNAL_VREF_V / ADS124S08_MAX_VALUE);
}

const ins_channel_config_t *ins_svc_get_channel_config(ins_channel_id_t id)
{
    return &ins_default_config[id];
}

const ins_channel_id_t *ins_svc_get_channel_id(ins_sensor_id_t id)
{
    return &ins_sensor_config[id];
}

/*============================================================================*/
/* Public Function Definitions                                                */
/*============================================================================*/
status_t ins_svc_ads124s08_init(void)
{
    for(uint8_t i = 0; i < INS_TOTAL_NUM_ADC; i++)
    {
        if(ins_adc_array[i].is_en == true) // if the ADC is being used 
        {
            ins_drv_ads124s08_cs_high(&ins_adc_array[i]); // initialize that ADC chip
        }
    }

    ins_drv_ads124s08_reset_module();

    for(uint8_t i = 0; i < INS_TOTAL_NUM_ADC; i++)
    {
        if(ins_adc_array[i].is_en == true) // if the ADC is being used 
        {
            status_t status = ins_drv_ads124s08_init_device(&ins_adc_array[i]); // initialize that ADC chip
            if(status != OK)
            {
                return status;
            }
        }
    }

    return OK;
}

status_t ins_svc_ads124s08_get_analog_voltage(ins_channel_id_t channel_id, float *voltage)
{
    if (channel_id >= INS_TOTAL_NUM_CHANNEL)
    {
        return ERROR_INVALID_PARAM;
    }

    const ins_channel_config_t *channel_config = ins_svc_get_channel_config(channel_id);

    uint8_t raw_data_buffer[3];
    status_t status = ins_drv_ads124s08_read_channel(channel_config->input_pos_pin, channel_config->input_neg_pin, channel_config->hw, raw_data_buffer);

    if (status != OK)
    {
        return status;
    }

    // convert 
    int32_t raw_int = ins_svc_raw_to_int(raw_data_buffer);
    *voltage = ins_svc_int_to_scaled_voltage(raw_int);

    return OK;
}

status_t ins_svc_ads124s08_read_gpio(ins_channel_id_t channel_id, bool *data)
{
    if (channel_id >= INS_TOTAL_NUM_CHANNEL)
    {
        return ERROR_INVALID_PARAM;
    } 

    const ins_channel_config_t *channel_config = ins_svc_get_channel_config(channel_id); 
    
    ins_drv_ads124s08_read_gpio(channel_config->hw, channel_config->input_gpio_pin, data);

    return OK;

}
