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

/*============================================================================*/
/* Static Function Prototypes                                                 */
/*============================================================================*/

static HAL_StatusTypeDef ins_drv_ads124s08_send_command(const ads124s08_hw_t *hw, uint8_t *command, uint16_t length);
static HAL_StatusTypeDef ins_drv_ads124s08_read_data(const ads124s08_hw_t *hw, uint8_t *data_dest, uint16_t length);

static status_t ads124s08_spi_write_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t data);
static status_t ads124s08_spi_read_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t *data);

/*============================================================================*/
/* Private Helper Functions                                                   */
/*============================================================================*/
static HAL_StatusTypeDef ins_drv_ads124s08_send_command(const ads124s08_hw_t *hw, uint8_t *command, uint16_t length)
{
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET); 
    HAL_StatusTypeDef hal_status = HAL_SPI_Transmit(hw->spi_handle, command, length, 10);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);

    return hal_status;
}

static HAL_StatusTypeDef ins_drv_ads124s08_read_data(const ads124s08_hw_t *hw, uint8_t *data_dest, uint16_t length)
{
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET); 
    HAL_StatusTypeDef hal_status = HAL_SPI_Receive(hw->spi_handle, data_dest, length, 10);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);

    return hal_status;
}

static status_t ads124s08_spi_write_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t data)
{
    uint8_t tx_buffer[3];

    // first two bytes are command bytes, all bytes after are the register values
    tx_buffer[0] = ADS124S08_WRITE_REGISTER_COMMAND | reg_address;
    tx_buffer[1] = 0x00; // only writing to one register

    // data to write to the register
    tx_buffer[2] = data;

    HAL_StatusTypeDef hal_status = ins_drv_ads124s08_send_command(hw, tx_buffer, 3);

    status_t status = (hal_status == HAL_OK) ? OK : ERROR;

    return status;
}

static status_t ads124s08_spi_read_register(const ads124s08_hw_t *hw, uint8_t reg_address, uint8_t *data)
{
    uint8_t tx_buffer[2];
    uint8_t rx_buffer;

    // first two bytes are command bytes
    tx_buffer[0] = ADS124S08_READ_REGISTER_COMMAND | reg_address;
    tx_buffer[1] = 0x00; // only reading one register

    // doesn't use send_command and read_data function here, because we need CS to stay low the entire time
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET);
    
    HAL_StatusTypeDef hal_status = HAL_SPI_Transmit(hw->spi_handle, tx_buffer, 2, 10);
    if (hal_status == HAL_OK)
    {
        hal_status = HAL_SPI_Receive(hw->spi_handle, &rx_buffer, 1, 10);
    }

    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);

    if (hal_status == HAL_OK)
    {
        *data = rx_buffer;
        return OK;
    }

    return ERROR;
}

/*============================================================================*/
/* Shadow Functions                                                           */
/*============================================================================*/
void ins_drv_ads124s08_shadow_init_default(const ads124s08_hw_t *hw)
{   
    ads124s08_shawdow_t *shadow = hw->shadow;

    //ID register should not be set

    //reset the por flag
    ads124s08_reset_POR_flag(&shadow->status);

    // input mux register default configuration
    ads124s08_set_input_bit(&shadow->input_mux,ADS124S08_AIN0,ADS124S08_AINCOM);

    // pga register default configuration
    ads124s08_set_pga_delay_bit(&shadow->pga,ADS124S08_PGA_DELAY_14_CYCLES);
    ads124s08_set_pga_en_bit(&shadow->pga,ADS124S08_PGA_ENABLED);
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

    ads124s08_set_gpio0_dir(&shadow->gpio_data,ADS124S08_GPIO_INPUT_CONFIG);
    ads124s08_set_gpio1_dir(&shadow->gpio_data,ADS124S08_GPIO_INPUT_CONFIG);
    ads124s08_set_gpio2_dir(&shadow->gpio_data,ADS124S08_GPIO_INPUT_CONFIG);
    ads124s08_set_gpio3_dir(&shadow->gpio_data,ADS124S08_GPIO_INPUT_CONFIG);

    ads124s08_set_gpio0_config(&shadow->gpio_control,ADS124S08_GPIO);
    ads124s08_set_gpio1_config(&shadow->gpio_control,ADS124S08_GPIO);
    ads124s08_set_gpio2_config(&shadow->gpio_control,ADS124S08_GPIO);
    ads124s08_set_gpio3_config(&shadow->gpio_control,ADS124S08_GPIO);
    
}

status_t ins_drv_ads124s08_write_shadow(const ads124s08_hw_t *hw)
{
    // write the shadow values to the device registers
    const struct {uint8_t address; uint8_t value;} regs[] =
    {
        {ADS124S08_STATUS_ADDRESS, hw->shadow->status},
        {ADS124S08_INPUT_MUX_ADDRESS, hw->shadow->input_mux},
        {ADS124S08_PGA_ADDRESS, hw->shadow->pga},
        {ADS124S08_DATA_RATE_ADDRESS, hw->shadow->data_rate},
        {ADS124S08_REF_ADDRESS, hw->shadow->ref},
        {ADS124S08_IDAC_MAG_ADDRESS, hw->shadow->i_dac_mag},
        {ADS124S08_IDAC_MUX_ADDRESS, hw->shadow->i_dac_mux},
        {ADS124S08_VBIAS_ADDRESS, hw->shadow->v_bias},

        // skip writing to the gain and offset registers

        {ADS124S08_GPIO_DATA_ADDRESS, hw->shadow->gpio_data},
        {ADS124S08_GPIO_CONFIG_ADDRESS, hw->shadow->gpio_control},

    };
    // error checking
    for (uint8_t i = 0; i < (sizeof(regs)/sizeof(regs[0])); i++)
    {
        status_t status = ads124s08_spi_write_register(hw, regs[i].address, regs[i].value);

        if (status != OK)
        {
            return status;
        }
    }

    return OK;
}

status_t ins_drv_ads124s08_read_shadow(const ads124s08_hw_t *hw)
{
    // read the shadow values from the device registers
    const struct {uint8_t address; uint8_t *value;} regs[] = 
    {
        {ADS124S08_ID_ADDRESS, &hw->shadow->device_id},
        {ADS124S08_STATUS_ADDRESS, &hw->shadow->status},
        {ADS124S08_INPUT_MUX_ADDRESS, &hw->shadow->input_mux},
        {ADS124S08_PGA_ADDRESS, &hw->shadow->pga},
        {ADS124S08_DATA_RATE_ADDRESS, &hw->shadow->data_rate},
        {ADS124S08_REF_ADDRESS, &hw->shadow->ref},
        {ADS124S08_IDAC_MAG_ADDRESS, &hw->shadow->i_dac_mag},
        {ADS124S08_IDAC_MUX_ADDRESS, &hw->shadow->i_dac_mux},
        {ADS124S08_VBIAS_ADDRESS, &hw->shadow->v_bias},
        {ADS124S08_SYS_ADDRESS, &hw->shadow->sys},
        {ADS124S08_OFFSET_CALIBRATION_0_ADDRESS, &hw->shadow->off_cal_0},
        {ADS124S08_OFFSET_CALIBRATION_1_ADDRESS, &hw->shadow->off_cal_1},
        {ADS124S08_OFFSET_CALIBRATION_2_ADDRESS, &hw->shadow->off_cal_2},
        {ADS124S08_GAIN_CALIBRATION_0_ADDRESS, &hw->shadow->fs_cal_0},
        {ADS124S08_GAIN_CALIBRATION_1_ADDRESS, &hw->shadow->fs_cal_1},
        {ADS124S08_GAIN_CALIBRATION_2_ADDRESS, &hw->shadow->fs_cal_2},
        {ADS124S08_GPIO_DATA_ADDRESS, &hw->shadow->gpio_data},
        {ADS124S08_GPIO_CONFIG_ADDRESS, &hw->shadow->gpio_control},
    };
    // error checking
    for (uint8_t i = 0; i < (sizeof(regs)/sizeof(regs[0])); i++)
    {
        status_t status = ads124s08_spi_read_register(hw, regs[i].address, regs[i].value);

        if (status != OK)
        {
            return status;
        }
    }
    
    return OK;
}

/*============================================================================*/
/* Calibration, Conversion, and Reading Channel                               */
/*============================================================================*/

status_t ins_drv_ads124s08_start_internal_calibration(const ads124s08_hw_t *hw)
{
    uint8_t command;

    command  = ADS124S08_START_COMMAND;
    if (ins_drv_ads124s08_send_command(hw, &command, 1) != HAL_OK)
    {
        return ERROR;
    }
    
    command = ADS124S08_SELF_OFFSET_CALIBRATION;
    if (ins_drv_ads124s08_send_command(hw, &command, 1) != HAL_OK)
    {
        return ERROR;
    }

    osDelay(ADS124S08_SFOCAL_DELAY_MS);

    if (ins_drv_ads124s08_read_shadow(hw) != OK)
    {
        return ERROR;
    }

    return OK;

    // command  = ADS124S08_STOP_COMMAND;
    // ins_drv_ads124s08_send_command(hw, &command, 1);
}

status_t ins_drv_ads124s08_start_conversion(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw)
{   
    // set the input mux to the desired pins
    ads124s08_set_input_bit(&hw->shadow->input_mux, pos_pin, neg_pin);
    if (ads124s08_spi_write_register(hw, ADS124S08_INPUT_MUX_ADDRESS, hw->shadow->input_mux) != OK)
    {
        return ERROR;
    }

    // send command to start single conversion
    uint8_t command = ADS124S08_START_COMMAND;
    if (ins_drv_ads124s08_send_command(hw, &command, 1) != HAL_OK)
    {
        return ERROR;
    }

    return OK;
}

status_t ins_drv_ads124s08_read_channel(ads124s08_input_mux_t pos_pin, ads124s08_input_mux_t neg_pin, const ads124s08_hw_t *hw, uint8_t *raw_data_buffer)
{

    //set the input mux and start the conversion
    status_t status = ins_drv_ads124s08_start_conversion(pos_pin, neg_pin, hw);
    if (status != OK)
    {
        return status;
    }

    osDelay(2);

    //send command to read data from the adc
    uint8_t command = ADS124S08_READ_DATA_COMMAND;
    
    // doesn't use send_command and read_data function here, because we need CS to stay low the entire time
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef hal_status = HAL_SPI_Transmit(hw->spi_handle, &command, 1, 10);
    if (hal_status == HAL_OK)
    {
        hal_status = HAL_SPI_Receive(hw->spi_handle, raw_data_buffer, 3, 10);
    }

    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);

    return (hal_status == HAL_OK) ? OK : ERROR;
}

status_t ins_drv_ads124s08_read_gpio(const ads124s08_hw_t *hw, const ads124s08_input_mux_t gpio_pin, bool *data)
{    
    uint8_t reg = 0;

    ads124s08_spi_read_register(hw, ADS124S08_GPIO_DATA_ADDRESS, &reg);

    switch (gpio_pin)
    {
        case GPIO0:
            *data = ((reg & ADS124S08_GPIO_0_CONFIG_MASK) >> ADS124S08_GPIO_0_CONFIG_LOCATION);
            break;
        case GPIO1:
            *data = ((reg & ADS124S08_GPIO_1_CONFIG_MASK) >> ADS124S08_GPIO_1_CONFIG_LOCATION);
            break;
        case GPIO2:
            *data = ((reg & ADS124S08_GPIO_2_CONFIG_MASK) >> ADS124S08_GPIO_2_CONFIG_LOCATION);
            break;    
        case GPIO3:
            *data = ((reg & ADS124S08_GPIO_3_CONFIG_MASK) >> ADS124S08_GPIO_3_CONFIG_LOCATION);
            break;    
    }
    return OK;
}

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/
void ins_drv_ads124s08_cs_high(const ads124s08_hw_t *hw)
{
    // pull chip select pin high
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);
}

void ins_drv_ads124s08_reset_module(void)
{
    // 9.4.1.2: toggle reset pin low, this resets the entire module.
    HAL_GPIO_WritePin(INS_RESET_PORT,INS_RESET_PIN,GPIO_PIN_RESET);
    osDelay(1);
    HAL_GPIO_WritePin(INS_RESET_PORT,INS_RESET_PIN,GPIO_PIN_SET);
    osDelay(1);
}

status_t ins_drv_ads124s08_init_device(const ads124s08_hw_t *hw)
{
    // send RESET command
    uint8_t command = ADS124S08_RESET_COMMAND;
    status_t status = ins_drv_ads124s08_send_command(hw, &command, 1);
    if (status != OK)
    {
        return status;
    }
    
    osDelay(ADS124S08_TD_RSSC_SETTLING_TIME_MS);  // waits t_d(RSSC) as per datasheet after RESET command

    ins_drv_ads124s08_shadow_init_default(hw);
    status = ins_drv_ads124s08_write_shadow(hw);
    if (status != OK)
    {
        return status;
    }

    return ins_drv_ads124s08_start_internal_calibration(hw);

}
