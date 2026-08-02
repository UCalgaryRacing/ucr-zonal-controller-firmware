/*
 * ins_drv_ads124s08.c
 *
 *  Created on: Apr 29, 2026
 *      Author: f-dunnwolbaum
 */

#include "ins_drv_ads124s08_regs.h"
#include "ins_drv_ads124s08.h"
#include "ins_config.h"
#include "ins_config_pinout.h"
#include "cmsis_os2.h"

// static function prototypes
static void write_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t data);
static void read_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t *data);

void ins_drv_ads124s08_init()
{
    ads124s08_hw_t hw;
    // pull chip select pins high
    for(uint8_t i = 0; i < INS_TOTAL_NUM_ADC; i++)
    {
        hw = ins_adc_array[i];
        HAL_GPIO_WritePin(hw.cs_port, hw.cs_pin, GPIO_PIN_SET);
    }

    // toggle reset pin low, this resets the entire module.
    HAL_GPIO_WritePin(INS_RESET_PORT,INS_RESET_PIN,GPIO_PIN_RESET);
    osDelay(1);
    HAL_GPIO_WritePin(INS_RESET_PORT,INS_RESET_PIN,GPIO_PIN_SET);
}

void ins_drv_ads124s08_write_shadow(const ads124s08_hw_t *hw)
{
    // write the shadow values to the device registers
    write_register(hw,ADS124S08_STATUS_ADDRESS, hw->shadow->status);
    write_register(hw, ADS124S08_INPUT_MUX_ADDRESS, hw->shadow->input_mux);
    write_register(hw, ADS124S08_PGA_ADDRESS, hw->shadow->pga);
    write_register(hw, ADS124S08_DATA_RATE_ADDRESS, hw->shadow->data_rate);
    write_register(hw, ADS124S08_REF_ADDRESS, hw->shadow->ref);
    write_register(hw, ADS124S08_IDAC_MAG_ADDRESS, hw->shadow->i_dac_mag);
    write_register(hw, ADS124S08_IDAC_MUX_ADDRESS, hw->shadow->i_dac_mux);
    write_register(hw, ADS124S08_VBIAS_ADDRESS, hw->shadow->v_bias);

    // skip writting to the gain and offset registers

    write_register(hw, ADS124S08_GPIO_DATA_ADDRESS, hw->shadow->gpio_data);
    write_register(hw, ADS124S08_GPIO_CONFIG_ADDRESS, hw->shadow->gpio_control);
}

void ins_drv_ads124s08_read_shadow(ads124s08_hw_t *hw)
{
    // read the shadow values from the device registers
    read_register(hw,ADS124S08_ID_ADDRESS, &hw->shadow->device_id);
    read_register(hw,ADS124S08_STATUS_ADDRESS, &hw->shadow->status);
    read_register(hw, ADS124S08_INPUT_MUX_ADDRESS, &hw->shadow->input_mux);
    read_register(hw, ADS124S08_PGA_ADDRESS, &hw->shadow->pga);
    read_register(hw, ADS124S08_DATA_RATE_ADDRESS, &hw->shadow->data_rate);
    read_register(hw, ADS124S08_REF_ADDRESS, &hw->shadow->ref);
    read_register(hw, ADS124S08_IDAC_MAG_ADDRESS, &hw->shadow->i_dac_mag);
    read_register(hw, ADS124S08_IDAC_MUX_ADDRESS, &hw->shadow->i_dac_mux);
    read_register(hw, ADS124S08_VBIAS_ADDRESS, &hw->shadow->v_bias);
    read_register(hw, ADS124S08_SYS_ADDRESS, &hw->shadow->sys);
    read_register(hw, ADS124S08_OFFSET_CALIBRATION_0_ADDRESS, &hw->shadow->off_cal_0);
    read_register(hw, ADS124S08_OFFSET_CALIBRATION_1_ADDRESS, &hw->shadow->off_cal_1);
    read_register(hw, ADS124S08_OFFSET_CALIBRATION_2_ADDRESS, &hw->shadow->off_cal_2);
    read_register(hw, ADS124S08_GAIN_CALIBRATION_0_ADDRESS, &hw->shadow->fs_cal_0);
    read_register(hw, ADS124S08_GAIN_CALIBRATION_1_ADDRESS, &hw->shadow->fs_cal_1);
    read_register(hw, ADS124S08_GAIN_CALIBRATION_2_ADDRESS, &hw->shadow->fs_cal_2);
    read_register(hw, ADS124S08_GPIO_DATA_ADDRESS, &hw->shadow->gpio_data);
    read_register(hw, ADS124S08_GPIO_CONFIG_ADDRESS, &hw->shadow->gpio_control);
}

void ins_drv_shadow_init_default(ads124s08_hw_t * hw)
{   
    ads124s08_shawdow_t *shadow = hw->shadow;

    //ID register should not be set

    //reset the por flag
    ads124s08_reset_POR_flag(&shadow->status);

    // input mux register default configuration
    ads124s08_set_input_bit(&shadow->input_mux,ADS124S08_INPUT_0,ADS124S08_INPUT_AINCOM);

    // pga register default configuration
    ads124s08_set_pga_delay_bit(&shadow->pga,ADS124S08_PGA_DELAY_14_CYCLES);
    ads124s08_set_pga_en_bit(&shadow->pga,ADS124S08_PGA_DISABLED);
    ads124s08_set_pga_gain_bit(&shadow->pga,ADS124S08_PGA_GAIN_1);

    //data rate register default configuration
    ads124s08_set_global_chop_en_bit(&shadow->data_rate,ADS124S08_CHOP_DISABLED);
    ads124s08_set_clock_source_bit(&shadow->data_rate,ADS124S08_CLK_INTERNAL);
    ads124s08_set_conversion_mode_bit(&shadow->data_rate,ADS124S08_CONTINUOUS_CONVERSION_MODE);
    ads124s08_set_digital_filter_bit(&shadow->data_rate,ADS124S08_DIGITAL_FILTER_SINC3);
    ads124s08_set_data_rate_bits(&shadow->data_rate,ADS124S08_4000_SPS);

    // reference control register default configuration
    ads124s08_set_ref_monitor_bits(&shadow->ref,ADS124S08_REFERENCE_MONITER_DISABLED);
    ads124s08_set_pos_ref_bypass_bit(&shadow->ref,ADS124S08_REFERENCE_BYPASS_DISABLED);
    ads124s08_set_neg_ref_bypass_bit(&shadow->ref,ADS124S08_REFERENCE_BYPASS_DISABLED);
    ads124s08_set_ref_input_bits(&shadow->ref,ADS124S08_REFERENCE_0);
    ads124s08_set_internal_ref_config_bits(&shadow->ref,ADS124S08_INTERNAL_REF_OFF);

    //excitation current register default configuration
    ads124s08_set_idac_pga_monitor_bit(&shadow->i_dac_mag,ADS124S08_PGA_RAIL_MONITOR_DISABLED);
    ads124s08_set_idac_low_side_switch_bit(&shadow->i_dac_mag,ADS124S08_LOW_SIDE_SWITCH_OPEN);
    ads124s08_set_idac_magnitude_bits(&shadow->i_dac_mag,ADS124S08_IDAC_OFF);

    ads124s08_set_idac1_output_bits(&shadow->i_dac_mux,ADS124S08_IDAC_OUTPUT_OFF);
    ads124s08_set_idac2_output_bits(&shadow->i_dac_mux,ADS124S08_IDAC_OUTPUT_OFF);

    //vbias register default configuration
    shadow->v_bias = 0x00; // all bias off

    // sys register default configuration
    ads124s08_set_system_monitor_bits(&shadow->sys,ADS124S08_SYSTEM_MONITOR_DISABLED);
    ads124s08_set_calibration_sample_size_bits(&shadow->sys,ADS124S08_16_SAMPLE_CAL);
    ads124s08_set_spi_timeout_bit(&shadow->sys,ADS124S08_SPI_TIMEOUT_DISABLED);
    ads124s08_set_crc_enable_bit(&shadow->sys,ADS124S08_CRC_DISABLED);
    ads124s08_set_send_status_bit(&shadow->sys,ADS124S08_STATUS_BYTE_DISABLED);

    shadow->gpio_data = 0x00;
    shadow->gpio_control = 0x00;
}

void ins_drv_ads124s08_start_conversion(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw)
{   
    // set the input mux to the desired pins
    ads124s08_set_input_bit(&hw->shadow->input_mux, pos_pin, neg_pin);
    write_register(hw, ADS124S08_INPUT_MUX_ADDRESS, hw->shadow->input_mux);

    // send command to start single conversion
    uint8_t command = ADS124S08_START_COMMAND;
    ins_drv_ads124s08_send_command(hw, &command, 1);
}

void ins_drv_ads124s08_start_internal_calibration(ads124s08_hw_t *hw)
{
    uint8_t command;

    command  = ADS124S08_START_COMMAND;
    ins_drv_ads124s08_send_command(hw, &command, 1);

    command = ADS124S08_SELF_OFFSET_CALIBRATION;
    ins_drv_ads124s08_send_command(hw, &command, 1);
    osDelay(100);
    ins_drv_ads124s08_read_shadow(hw);

    // command  = ADS124S08_STOP_COMMAND;
    // ins_drv_ads124s08_send_command(hw, &command, 1);
}

static void write_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t data)
{
    uint8_t tx_buffer[3];

    // first two bytes are command bytes, all bytes after are the register values
    tx_buffer[0] = ADS124S08_WRITE_REGISTER_COMMAND | reg_address;
    tx_buffer[1] = 0x00; // only writing to one register

    // data to write to the register
    tx_buffer[2] = data;

    ins_drv_ads124s08_send_command(hw, tx_buffer, 3);
}

static void read_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t *data)
{
    uint8_t tx_buffer[2];
    uint8_t rx_buffer;

    // first two bytes are command bytes
    tx_buffer[0] = ADS124S08_READ_REGISTER_COMMAND | reg_address;
    tx_buffer[1] = 0x00; // only reading one register

    ins_drv_ads124s08_send_command(hw, tx_buffer, 2);

    ins_drv_ads124s08_read_data(hw, &rx_buffer, 1);

    *data = rx_buffer;
}

void ins_drv_ads124s08_send_command(const ads124s08_hw_t *hw, uint8_t *command, uint16_t length)
{
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET); 
    HAL_SPI_Transmit(hw->spi_handle, command, length, 10);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);
}

void ins_drv_ads124s08_read_data(const ads124s08_hw_t *hw, uint8_t *data_dest, uint16_t length)
{
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET); 
    HAL_SPI_Receive(hw->spi_handle, data_dest, length, 10);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);
}

int32_t ins_drv_ads124s08_format_data(uint8_t * data_buffer)
{   
    int32_t raw_data;
    
    // data is recieved in three bytes and is 24 bit 2's compliment, must convert to 32 bit int from buffer
    // if the data is negative, then front pad with 1 so that the data can be cast to an int 
    if((data_buffer[0] & 0x80))
    {
        raw_data = (int32_t) ((0xFF<<24) | (data_buffer[0] << 16) | (data_buffer[1] << 8) | data_buffer[2]);
    }
    else
    {
        raw_data = (int32_t) ((data_buffer[0] << 16) | (data_buffer[1] << 8) | data_buffer[2]);
    }

    return raw_data;
}
