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
#include <string.h>
#include <stdbool.h>
#include "cmsis_os2.h"

static float convert_raw_to_voltage(uint8_t *data_buffer);

static float ins_array_values[INS_TOTAL_NUM_CHANNEL];

static const ins_channel_t * get_channel_config(ins_channel_id_t id)
{
    return &ins_default_config[id];
}

void ins_svc_ads124s08_init()
{
    // do driver level initialization
    ins_drv_ads124s08_init();

    // initialize each specific device
    for(uint8_t i = 0; i < INS_TOTAL_NUM_ADC; i++)
    {
        ins_svc_ads124s08_init_device(&ins_adc_array[i]);
    }

}

void ins_svc_ads124s08_init_device(ads124s08_hw_t *hw)
{   
    uint8_t command = ADS124S08_RESET_COMMAND;
    ins_drv_ads124s08_send_command(hw, &command, 1);
    osDelay(10);

    // write defaults to the shadow register
    ins_drv_shadow_init_default(hw);
    ins_drv_ads124s08_write_shadow(hw);

    ads124s08_shawdow_t readback_shadow;
    ads124s08_hw_t readback_hw = *hw;

    readback_hw.shadow = &readback_shadow;
    ins_drv_ads124s08_read_shadow(&readback_hw);
    // update the device ID in the shadow register with the value read back from the device   
    hw->shadow->device_id = readback_hw.shadow->device_id; 

    // update the calibration registers
    hw->shadow->fs_cal_0 = readback_hw.shadow->fs_cal_0;
    hw->shadow->fs_cal_1 = readback_hw.shadow->fs_cal_1;
    hw->shadow->fs_cal_2 = readback_hw.shadow->fs_cal_2;

    hw->shadow->off_cal_0 = readback_hw.shadow->off_cal_0;
    hw->shadow->off_cal_1 = readback_hw.shadow->off_cal_1;
    hw->shadow->off_cal_2 = readback_hw.shadow->off_cal_2;

    // start the calibration for the adc
    ins_drv_ads124s08_start_internal_calibration(hw);

}

void ins_svc_update_ads124s08()
{   
    ins_channel_t * channel;
    for(uint8_t id = 0; id < INS_TOTAL_NUM_CHANNEL; id++)
    {
        channel = get_channel_config(id);
        if(channel->is_en)
        {
            ins_array_values[id] =  ins_svc_ads124s08_read_channel(*channel);
        }
    }
}

float ins_svc_ads124s08_read_channel(ins_channel_t channel)
{   
    uint8_t command;
    float converted_value;

    //remove delay?
    osDelay(10);
    //set the input mux and start the conversion
    ins_drv_ads124s08_start_conversion(channel.input_pos_pin, channel.input_neg_pin, channel.hw);

    osDelay(5);

    //send command to read data from the adc
    command = ADS124S08_READ_DATA_COMMAND;
    ins_drv_ads124s08_send_command(channel.hw, &command, 1);

    //actually read the data
    uint8_t data_buffer[3];
    ins_drv_ads124s08_read_data(channel.hw, data_buffer, 3);

    //convert raw value to a float
    converted_value = convert_raw_to_voltage(data_buffer);
    
    return converted_value;
}

static float convert_raw_to_voltage(uint8_t *data_buffer)
{
    int32_t raw_data;
    float converted_value;

    raw_data = ins_drv_ads124s08_format_data(data_buffer);

    converted_value = (float) raw_data * INSTRUMENTATION_SCALING_FACTOR * INSTRUMENTATION_EXTERNAL_REFERENCE / ADS124S08_MAX_VALUE;
    
    return converted_value;
}