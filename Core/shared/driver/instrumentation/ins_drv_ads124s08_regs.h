/**
 * @file ins_drv_ads124s08_regs.h
 * 
 * Holds the register definitions, addresses, bit masks, and helper functions for the ADS124S08 ADC
 * Used by the driver layer to read and write to the device registers
 * 
 * All the register values are done in enumerations
 * Register masks are done as #defines
 * 
 * Based on the datasheet for the ADS124S08, June 2017 revision, register definitions can be found in section 9.6.1
 * 
 *  author: f-dunnwolbaum
 */

#include <stdint.h>
#include <stdbool.h>
 
#ifndef INSTRUMENTATION_DRV_ADS124S08_REGS_H
#define INSTRUMENTATION_DRV_ADS124S08_REGS_H

/*============================================================================*/
/* Commands                                                                   */
/*============================================================================*/

#define ADS124S08_WAKE_UP_COMMAND 0x02
#define ADS124S08_POWER_DOWN_COMMAND 0x04
#define ADS124S08_RESET_COMMAND 0x06
#define ADS124S08_START_COMMAND 0x08
#define ADS124S08_STOP_COMMAND 0x0A
#define ADS124S08_READ_DATA_COMMAND 0x12
#define ADS124S08_NOP_COMMAND 0x00
#define ADS124S08_OFFSET_CALIBRATION_COMMAND 0x16
#define ADS124S08_GAIN_CALIBRATION_COMMAND 0x17
#define ADS124S08_SELF_OFFSET_CALIBRATION 0x19
#define ADS124S08_READ_REGISTER_COMMAND 0x20
#define ADS124S08_WRITE_REGISTER_COMMAND 0x40

/*============================================================================*/
/* Register Addresses                                                         */
/*============================================================================*/

#define ADS124S08_ID_ADDRESS 0x00U
#define ADS124S08_STATUS_ADDRESS 0x01U
#define ADS124S08_INPUT_MUX_ADDRESS 0x02U
#define ADS124S08_PGA_ADDRESS 0x03U
#define ADS124S08_DATA_RATE_ADDRESS 0x04U
#define ADS124S08_REF_ADDRESS 0x05U
#define ADS124S08_IDAC_MAG_ADDRESS 0x06U
#define ADS124S08_IDAC_MUX_ADDRESS 0x07U
#define ADS124S08_VBIAS_ADDRESS 0x08U
#define ADS124S08_SYS_ADDRESS 0x09U
#define ADS124S08_OFFSET_CALIBRATION_0_ADDRESS 0x0AU
#define ADS124S08_OFFSET_CALIBRATION_1_ADDRESS 0x0BU
#define ADS124S08_OFFSET_CALIBRATION_2_ADDRESS 0x0CU
#define ADS124S08_GAIN_CALIBRATION_0_ADDRESS 0x0DU
#define ADS124S08_GAIN_CALIBRATION_1_ADDRESS 0x0EU
#define ADS124S08_GAIN_CALIBRATION_2_ADDRESS 0x0FU
#define ADS124S08_GPIO_DATA_ADDRESS 0x10U
#define ADS124S08_GPIO_CONFIG_ADDRESS 0x11U

/*============================================================================*/
/* Register Bit Masks and enumerations                                        */
/*============================================================================*/

/**
 * Status register bit values and masks
 * 
 * Each bit is a flag in the status register, so the values in the enum are the bit positions
 * and can also be used for masking, under normal operation the status register should be 0
 * 
 * bit 7 is POR flag
 * bit 6 is nRDY flag
 * bit 5 is the positive PGA input at positive rail flag
 * bit 4 is the positive PGA input at negative rail flag
 * bit 3 is the negative PGA input at positive rail flag
 * bit 2 is the negative PGA input at negative rail flag
 * bit 1 is the reference voltage flag 1, indicates reference voltage is below 1
 * bit 0 is the reference voltage flag 0, indicates reference voltage is below 0.3V
 * 
 * Note that only the POR flag can be writted to, all others are read only
 */

typedef enum
{
    ADS124S08_NO_ERROR = 0U,
    ADS124S08_ERROR = 1U,
} ads124s08_status_reg_t;

#define ADS124S08_STATUS_POR_MASK 0x80U
#define ADS124S08_STATUS_POR_LOCATION 7U

#define ADS124S08_STATUS_nRDY_MASK 0x40U
#define ADS124S08_STATUS_nRDY_LOCATION 6U

#define ADS124S08_STATUS_POS_PGA_POS_RAIL_MASK 0x20U
#define ADS124S08_STATUS_POS_PGA_POS_RAIL_LOCATION 5U

#define ADS124S08_STATUS_POS_PGA_NEG_RAIL_MASK 0x10U
#define ADS124S08_STATUS_POS_PGA_NEG_RAIL_LOCATION 4U

#define ADS124S08_STATUS_NEG_PGA_POS_RAIL_MASK 0x08U
#define ADS124S08_STATUS_NEG_PGA_POS_RAIL_LOCATION 3U

#define ADS124S08_STATUS_NEG_PGA_NEG_RAIL_MASK 0x04U
#define ADS124S08_STATUS_NEG_PGA_NEG_RAIL_LOCATION 2U

#define ADS124S08_STATUS_REF_VOLTAGE_FLAG_1_MASK 0x02U
#define ADS124S08_STATUS_REF_VOLTAGE_FLAG_1_LOCATION 1U

#define ADS124S08_STATUS_REF_VOLTAGE_FLAG_0_MASK 0x01U
#define ADS124S08_STATUS_REF_VOLTAGE_FLAG_0_LOCATION 0U

/**
 * Input mux register values are defined in the enumeration
 * The input mux mask is done
 * 
 * bits [7:4] are the positive input selection
 * bits [3:0] are the negative input selection
 */
typedef enum
{
    ADS124S08_INPUT_0 = 0U,
    ADS124S08_INPUT_1 = 1U,
    ADS124S08_INPUT_2 = 2U,
    ADS124S08_INPUT_3 = 3U,
    ADS124S08_INPUT_4 = 4U,
    ADS124S08_INPUT_5 = 5U,
    ADS124S08_INPUT_6 = 6U,
    ADS124S08_INPUT_7 = 7U,
    ADS124S08_INPUT_8 = 8U,
    ADS124S08_INPUT_9 = 9U,
    ADS124S08_INPUT_10 = 10U,
    ADS124S08_INPUT_11 = 11U,
    ADS124S08_INPUT_AINCOM = 12U
} ads124s08_input_mux_t;

#define ADS124S08_POSITIVE_INPUT_MASK 0XF0U
#define ADS124S08_POSITIVE_INPUT_LOCATION 4U

#define ADS124S08_NEGATIVE_INPUT_MASK 0x0FU
#define ADS124S08_NEGATIVE_INPUT_LOCATION 0U

/**
 * PGA register bit values and masks
 * 
 * The PGA register has multiple bit fields
 * one enumeration is used per bit field
 * 
 * bits [7:5] are the pga delay
 * bits [4:3] are the pga enable
 * bits [2:0] are the pga gain
 */

 // Delay is counted in clock cycles, tmod = 4.096MHz
typedef enum
{
    ADS124S08_PGA_DELAY_14_CYCLES = 0U,
    ADS124S08_PGA_DELAY_25_CYCLES = 1U,
    ADS124S08_PGA_DELAY_64_CYCLES = 2U,
    ADS124S08_PGA_DELAY_256_CYCLES = 3U,
    ADS124S08_PGA_DELAY_1024_CYCLES = 4U,
    ADS124S08_PGA_DELAY_2048_CYCLES = 5U,
    ADS124S08_PGA_DELAY_4096_CYCLES = 6U,
    ADS124S08_PGA_DELAY_1_CYCLES = 7U
} ads124s08_pga_delay_t;

typedef enum
{
    ADS124S08_PGA_DISABLED = 0U,
    ADS124S08_PGA_ENABLED = 1U
} ads124s08_pga_enable_t;

typedef enum
{
    ADS124S08_PGA_GAIN_1 = 0U,
    ADS124S08_PGA_GAIN_2 = 1U,
    ADS124S08_PGA_GAIN_4 = 2U,
    ADS124S08_PGA_GAIN_8 = 3U,
    ADS124S08_PGA_GAIN_16 = 4U,
    ADS124S08_PGA_GAIN_32 = 5U,
    ADS124S08_PGA_GAIN_64 = 6U,
    ADS124S08_PGA_GAIN_128 = 7U
} ads124s08_pga_gain_t;

#define ADS124S08_PGA_DELAY_MASK 0xE0U
#define ADS124S08_PGA_DELAY_LOCATION 5U

#define ADS124S08_PGA_ENABLE_MASK 0x18U
#define ADS124S08_PGA_ENABLE_LOCATION 3U

#define ADS124S08_PGA_GAIN_MASK 0x07U
#define ADS124S08_PGA_GAIN_LOCATION 0U

/**
 * Data register bit values and masks
 * 
 * bit 7 is the global chop enable
 * bit 6 is the chop clock source
 * bit 5 is the conversion mode
 * bit 4 is the digital filter selection
 * bits [3:0] are the data rate or effective sample rate selection
 */

typedef enum
{   
    ADS124S08_CHOP_DISABLED = 0U,
    ADS124S08_CHOP_ENABLED = 1U
} ads124s08_chop_enable_t;

typedef enum
{
    ADS124S08_CLK_INTERNAL = 0U,
    ADS124S08_CLK_EXTERNAL = 1U
} ads124s08_chop_clock_source_t;

typedef enum
{
    ADS124S08_CONTINUOUS_CONVERSION_MODE = 0U,
    ADS124S08_SINGLE_SHOT_MODE = 1U
} ads124s08_conversion_mode_t;

typedef enum
{
    ADS124S08_DIGITAL_FILTER_SINC3 = 0U,
    ADS124S08_DIGITAL_FILTER_LOW_LATENCY = 1U
} ads124s08_digital_filter_t;

typedef enum
{
    ADS124S08_2_5_SPS = 0U,
    ADS124S08_5_SPS = 1U,
    ADS124S08_10_SPS = 2U,
    ADS124S08_16_6_SPS = 3U,
    ADS124S08_20_SPS = 4U,
    ADS124S08_50_SPS = 5U,
    ADS124S08_60_SPS = 6U,
    ADS124S08_100_SPS = 7U,
    ADS124S08_200_SPS = 8U,
    ADS124S08_400_SPS = 9U,
    ADS124S08_800_SPS = 10U,
    ADS124S08_1000_SPS = 11U,
    ADS124S08_2000_SPS = 12U,
    ADS124S08_4000_SPS = 13U
} ads124s08_data_rate_t;

#define ADS124S08_CHOP_ENABLE_MASK 0x80U
#define ADS124S08_CHOP_ENABLE_LOCATION 7U

#define ADS124S08_CHOP_CLOCK_SOURCE_MASK 0x40U
#define ADS124S08_CHOP_CLOCK_SOURCE_LOCATION 6U

#define ADS124S08_CONVERSION_MODE_MASK 0x20U
#define ADS124S08_CONVERSION_MODE_LOCATION 5U

#define ADS124S08_DIGITAL_FILTER_MASK 0x10U
#define ADS124S08_DIGITAL_FILTER_LOCATION 4U

#define ADS124S08_DATA_RATE_MASK 0x0FU
#define ADS124S08_DATA_RATE_LOCATION 0U

/**
 * Reference control register bit values and masks
 * 
 * bits[7:6] are the reference monitor selection
 * bit 5 is the positive reference bypass
 * bit 4 is the negative reference bypass
 * bits [3:2] are the reference selection
 * bits [1:0] are the reference configuration
 */

typedef enum
{
    ADS124S08_REFERENCE_MONITER_DISABLED = 0U,
    ADS124S08_REGERENCE_MONITER_LV0 = 1U,
    ADS124S08_REGERENCE_MONITER_LV1 = 2U,
} ads124s08_reference_moninter_config_t;

typedef enum
{
    ADS124S08_REFERENCE_BYPASS_ENABLED = 0U,
    ADS124S08_REFERENCE_BYPASS_DISABLED = 1U
    
} ads124s08_reference_bypass_t;


typedef enum
{
    ADS124S08_REFERENCE_0 = 0U,
    ADS124S08_REFERENCE_1 = 1U,
    ADS124S08_INTERNAL_REFERENCE = 2U,
} ads124s08_reference_input_selection_t;

typedef enum
{
    ADS124S08_INTERNAL_REF_OFF = 0U,         
    ADS124S08_INTERNAL_REF_OFF_SLEEP = 1U, /* internal reference is on unless in sleep or power-down mode*/
    ADS124S08_INTERNAL_REF_ALWAYS_ON = 2U, /* internal reference is always on */
} ads124s08_internal_reference_config_t;

#define ADS124S08_REFERENCE_MONITOR_MASK 0XC0U
#define ADS124S08_REFERENCE_MONITOR_LOCATION 6U

#define ADS124S08_POSITIVE_REFERENCE_BYPASS_MASK 0x20U
#define ADS124S08_POSITIVE_REFERENCE_BYPASS_LOCATION 5U

#define ADS124S08_NEGATIVE_REFERENCE_BYPASS_MASK 0x10U
#define ADS124S08_NEGATIVE_REFERENCE_BYPASS_LOCATION 4U

#define ADS124S08_REFERENCE_SELECTION_MASK 0x0CU
#define ADS124S08_REFERENCE_SELECTION_LOCATION 2U

#define ADS124S08_INTERNAL_REFERENCE_CONFIG_MASK 0x03U
#define ADS124S08_INTERNAL_REFERENCE_CONFIG_LOCATION 0U

/**
 * Excitation current register 1 bit values and masks
 * 
 * bit 7 is the PGA output rail monitor enable
 * bit 6 is the low side switch enable
 * bits [5:4] are reserved and should only have 0 written
 * bits [3:0] are the IDAC magnitude
 */

typedef enum
{
    ADS124S08_PGA_RAIL_MONITOR_DISABLED = 0U,
    ADS124S08_PGA_RAIL_MONITOR_ENABLED = 1U
} ads124x08_idac_pga_rail_moniter_t;

typedef enum
{
    ADS124S08_LOW_SIDE_SWITCH_OPEN = 0U,
    ADS124S08_LOW_SIDE_SWITCH_CLOSED = 1U
} ads124s08_idac_low_side_switch_t;

typedef enum
{
    ADS124S08_IDAC_OFF = 0U,
    ADS124S08_IDAC_MAGNITUDE_10uA = 1U,
    ADS124S08_IDAC_MAGNITUDE_50uA = 2U,
    ADS124S08_IDAC_MAGNITUDE_100uA = 3U,
    ADS124S08_IDAC_MAGNITUDE_250uA = 4U,
    ADS124S08_IDAC_MAGNITUDE_500uA = 5U,
    ADS124S08_IDAC_MAGNITUDE_750uA = 6U,
    ADS124S08_IDAC_MAGNITUDE_1000uA = 7U,
    ADS124S08_IDAC_MAGNITUDE_1500uA = 8U,
    ADS124S08_IDAC_MAGNITUDE_2000uA = 9U,
} ads124s08_idac_1_current_t;

#define ADS124S08_PGA_IDAC_RAIL_MONITOR_MASK 0x80U
#define ADS124S08_PGA_IDAC_RAIL_MONITOR_LOCATION 7U

#define ADS124S08_IDAC_LOW_SIDE_SWITCH_MASK 0x40U
#define ADS124S08_IDAC_LOW_SIDE_SWITCH_LOCATION 6U

#define ADS124S08_IDAC_MAGNITUDE_MASK 0X0FU
#define ADS124S08_IDAC_MAGNITUDE_LOCATION 0U

/**
 * Excitation current register 2 bit values and masks
 * 
 * 
 * bits [7:4] are the IDAC2 output mux
 * bits [3:0] are the IDAC1 output mux
 */

typedef enum
{
    ADS124S08_IDAC_OUTPUT_0 = 0U,
    ADS124S08_IDAC_OUTPUT_1 = 1U,
    ADS124S08_IDAC_OUTPUT_2 = 2U,
    ADS124S08_IDAC_OUTPUT_3 = 3U,
    ADS124S08_IDAC_OUTPUT_4 = 4U,
    ADS124S08_IDAC_OUTPUT_5 = 5U,
    ADS124S08_IDAC_OUTPUT_6 = 6U,
    ADS124S08_IDAC_OUTPUT_7 = 7U,
    ADS124S08_IDAC_OUTPUT_8 = 8U,
    ADS124S08_IDAC_OUTPUT_9 = 9U,
    ADS124S08_IDAC_OUTPUT_10 = 10U,
    ADS124S08_IDAC_OUTPUT_11 = 11U,
    ADS124S08_IDAC_OUTPUT_AINCOM = 12U,
    ADS124S08_IDAC_OUTPUT_OFF = 15U
} ads124s08_idac_output_mux_t;

#define ADS124S08_IDAC1_OUTPUT_MASK 0X0FU
#define ADS124S08_IDAC1_OUTPUT_LOCATION 0U

#define ADS124S08_IDAC2_OUTPUT_MASK 0XF0U
#define ADS124S08_IDAC2_OUTPUT_LOCATION 4U

/**
 * Sensor biasing register bit values and masks
 * 
 * bit 7 is the bias voltage level, either half of the supply or 1/12 of the supply
 * bit 6 is the connection of the bias voltage to AINCOM
 * bits [5:0] are the connection of the bias voltage to the different analog inputs
 */

typedef enum
{
    ADS124S08_VBIAS_HALF_VDD = 0U,
    ADS124S08_VBIAS_TWELFVE_VDD = 1U,
} ads124s08_vbias_level_config_t;

typedef enum
{
    ADS124S08_VBIAS_DISCONNECTED = 0U,
    ADS124S08_VBIAS_CONNECTED = 1U
} ads124s08_vbias_connection_t;

#define ADS124S08_VBIAS_LEVEL_MASK 0x80U
#define ADS124S08_VBIAS_LEVEL_LOCATION 7U

#define ADS124S08_AINCOM_VBIAS_CONNECTION_MASK 0x40U
#define ADS124S08_AINCOM_VBIAS_CONNECTION_LOCATION 6U

#define ADS124S08_VBIAS_ANALOG_INPUT_5_CONNECTION_MASK 0x20U
#define ADS124S08_VBIAS_ANALOG_INPUT_5_CONNECTION_LOCATION 5U

#define ADS124S08_VBIAS_ANALOG_INPUT_4_CONNECTION_MASK 0x10U
#define ADS124S08_VBIAS_ANALOG_INPUT_4_CONNECTION_LOCATION 4U

#define ADS124S08_VBIAS_ANALOG_INPUT_3_CONNECTION_MASK 0x08U
#define ADS124S08_VBIAS_ANALOG_INPUT_3_CONNECTION_LOCATION 3U

#define ADS124S08_VBIAS_ANALOG_INPUT_2_CONNECTION_MASK 0x04U
#define ADS124S08_VBIAS_ANALOG_INPUT_2_CONNECTION_LOCATION 2U

#define ADS124S08_VBIAS_ANALOG_INPUT_1_CONNECTION_MASK 0x02U
#define ADS124S08_VBIAS_ANALOG_INPUT_1_CONNECTION_LOCATION 1U

#define ADS124S08_VBIAS_ANALOG_INPUT_0_CONNECTION_MASK 0x01U
#define ADS124S08_VBIAS_ANALOG_INPUT_0_CONNECTION_LOCATION 0U

/**
 * System control register bit values and masks
 * 
 * bit[7:5] are the system monitor configuration
 * bit[4:3] are the calibration sample size
 * bit 2 is the SPI timeout enable
 * bit 1 is the CRC enable
 * bit 0 is the send status byte enable
 */

typedef enum
{   
    ADS124S08_SYSTEM_MONITOR_DISABLED = 0U,
    ADS124S08_GAIN_SET_BY_USER = 1U,        //shorts the pga to the inputs to allow the gain to be set manually
    ADS124S08_INTERNAL_TEMP_SENSOR_ENABLED = 2U,
    ADS124S08_AVDD_AVSS_FOUR_SAMPLE = 3U,
    ADS124S08_DVDD_FOUR_SAMPLE = 4U,
    ADS124S08_BURNOUT_ENABLED_0_2UA = 5U,   //0.2uA burnout currrent
    ADS124S08_BURNOUT_ENABLED_1UA = 6U,     //1uA burnout current
    ADS124S08_BURNOUT_ENABLED_10UA = 7U     //10uA burnout current
} ads124s08_system_monitor_t;

typedef enum
{
    ADS124S08_1_SAMPLE_CAL = 0U,
    ADS124S08_2_SAMPLE_CAL = 1U,
    ADS124S08_8_SAMPLE_CAL = 2U,
    ADS124S08_16_SAMPLE_CAL = 3U
} ads124s08_calibration_sample_size_t;

typedef enum
{
    ADS124S08_SPI_TIMEOUT_ENABLED = 0U,
    ADS124S08_SPI_TIMEOUT_DISABLED = 1U
} ads124s08_spi_timeout_t;

typedef enum 
{
    ADS124S08_CRC_DISABLED = 0U,
    ADS124S08_CRC_ENABLED = 1U
} ads124s08_crc_enable_t;

typedef enum
{
    ADS124S08_STATUS_BYTE_DISABLED = 0U,
    ADS124S08_STATUS_BYTE_ENABLED= 1U
} ads124s08_send_status_byte_t;

#define ADS124S08_SYSTEM_MONITOR_MASK 0xE0U
#define ADS124S08_SYSTEM_MONITOR_LOCATION 5U

#define ADS124S08_CALIBRATION_SAMPLE_SIZE_MASK 0x18U
#define ADS124S08_CALIBRATION_SAMPLE_SIZE_LOCATION 3U

#define ADS124S08_SPI_TIMEOUT_MASK 0x04U
#define ADS124S08_SPI_TIMEOUT_LOCATION 2U

#define ADS124S08_CRC_ENABLE_MASK 0x02U
#define ADS124S08_CRC_ENABLE_LOCATION 1U

#define ADS124S08_SEND_STATUS_BYTE_MASK 0x01U
#define ADS124S08_SEND_STATUS_BYTE_LOCATION 0U

/**
 * Gain calibration and offset calibration registers hold the 
 * gain and offset values. no need for enumerations or masks
 */

/**
 * GPIO data register bit values and masks
 * 
 * Chooses the data direction for each GPIO pin and the data for that GPIO
 * 
 * bits [7:4] are the data direction for each GPIO pin
 * bits [3:0] are the data for each GPIO
 */

typedef enum
{
    ADS124S08_GPIO_OUTPUT_CONFIG = 0U,
    ADS124S08_GPIO_INPUT_CONFIG = 1U
} ads124s08_gpio_data_direction_t;

// digital gpio value for the gpios
typedef enum
{
    ADS124S08_GPIO_LOW = 0U,
    ADS124S08_GPIO_HIGH = 1U
} ads124s08_gpio_value_t;


#define ADS124S08_GPIO_3_DIRECTION_MASK 0x80U
#define ADS124S08_GPIO_3_DIRECTION_LOCATION 7U

#define ADS124S08_GPIO_2_DIRECTION_MASK 0x40U
#define ADS124S08_GPIO_2_DIRECTION_LOCATION 6U

#define ADS124S08_GPIO_1_DIRECTION_MASK 0x20U
#define ADS124S08_GPIO_1_DIRECTION_LOCATION 5U

#define ADS124S08_GPIO_0_DIRECTION_MASK 0x10U
#define ADS124S08_GPIO_0_DIRECTION_LOCATION 4U

#define ADS124S08_GPIO_3_DATA_MASK 0x08U
#define ADS124S08_GPIO_3_DATA_LOCATION 3U

#define ADS124S08_GPIO_2_DATA_MASK 0x04U
#define ADS124S08_GPIO_2_DATA_LOCATION 2U

#define ADS124S08_GPIO_1_DATA_MASK 0x02U
#define ADS124S08_GPIO_1_DATA_LOCATION 1U

#define ADS124S08_GPIO_0_DATA_MASK 0x01U
#define ADS124S08_GPIO_0_DATA_LOCATION 0U

/**
 * GPIO configuration register bit values and masks
 * 
 * Chooses whether the pin is used as an analog input or GPIO
 */

typedef enum
{
    ADS124S08_ANALOG_INPUT = 0U,
    ADS124S08_GPIO = 1U
} ads124s08_gpio_configuration_t;

#define ADS124S08_GPIO_3_CONFIG_MASK 0x08U
#define ADS124S08_GPIO_3_CONFIG_LOCATION 3U

#define ADS124S08_GPIO_2_CONFIG_MASK 0x04U
#define ADS124S08_GPIO_2_CONFIG_LOCATION 2U

#define ADS124S08_GPIO_1_CONFIG_MASK 0x02U
#define ADS124S08_GPIO_1_CONFIG_LOCATION 1U

#define ADS124S08_GPIO_0_CONFIG_MASK 0x01U
#define ADS124S08_GPIO_0_CONFIG_LOCATION 0U

/*============================================================================*/
/* Shadow Registers                                                           */
/*============================================================================*/

/*
 Shadow register of ADS124S08
 Any changes to a registers must be reflected in the shadow register
 to ensure that MCU can know the status of the device

 Each register is 8-bits, the entire register space is 18 bytes
 
*/
typedef struct 
{
    /* Device ID Register 9.6.1.1
    *
    * Should never be written to, only read
    */
    uint8_t device_id;

    /* Device Status Register 9.6.1.2
    * 
    * Contains all status flags for ads124s08
    */
    uint8_t status;

    /* Input Mux Register 9.6.1.3
     * 
     * Controls what input is being used for positive and negative inputs
     */
    uint8_t input_mux;

    /* Programmable Gain Setting Register 9.6.1.4
     * 
     * Controls internal gain settings 
     */
    uint8_t pga;

    /* Data Rate Register 9.6.1.5
     * 
     * Contains sample rate, clock source,
     * conversion mode and digital filter selection
     */
    uint8_t data_rate;

    /* Reference Control Register 9.6.1.6
     *
     * Controls where the ADC reference comes from 
     * and reference monitoring/buffering 
     */
    uint8_t ref;

    /* Current Excitation Control Registers 9.6.1.7 and 9.6.1.8
     * 
     * Controls what pins are used for excitation and sets a current limit
     * Not used on UCR-03
     */
    uint8_t i_dac_mag;
    uint8_t i_dac_mux;

    /* Sensor Bias Register 9.6.1.9
     * 
     * Can be used to control the magnitude of bias
     * and what pins have bias enabled
     * Not used on UCR-03
     */
    uint8_t v_bias;

    /* System Control Register 9.6.1.10
     * 
     * Used to control system monitering, calibration sample size
     * CRC and SPI timeout
     */
    uint8_t sys;

    /* Offset Calibration Registers 9.6.1.11 to 9.6.1.13
     * 
     * Holds value from the calibration offset. 
     * Calibration value is 24-bits long
     */
    uint8_t off_cal_0; /* [7:0]   */
    uint8_t off_cal_1; /* [15:8]  */
    uint8_t off_cal_2; /* [23:16] */

    /* Gain Calibration Refisters 9.6.1.14 to 9.6.1.16
     * 
     * Holds value from gain calibration
     * Calibration value is 24-bits long
     */
    uint8_t fs_cal_0; /* [7:0]   */
    uint8_t fs_cal_1; /* [15:8]  */
    uint8_t fs_cal_2; /* [23:16] */

    /* GPIO Data and Control Registers 9.6.1.17 and 9.6.1.18
     * 
     * Controls GPIO direction and data on ADS1240S06
     * Not used on UCR-03
     */
    uint8_t gpio_data;
    uint8_t gpio_control;

} ads124s08_shawdow_t;

/*============================================================================*/
/* Helper Functions                                                           */
/*============================================================================*/

/**
 * The following are functions to be used for setting register values.
 * They operate on individual registers, not the entire shadow register
 */

// Status register helper
static inline void ads124s08_reset_POR_flag(uint8_t * reg)
{
    *reg = (uint8_t)(*reg & (~(1U << ADS124S08_STATUS_POR_LOCATION)));
}

// Input register helper
static inline void ads124s08_set_input_bit(uint8_t *reg,ads124s08_input_mux_t positive_input, ads124s08_input_mux_t negative_input)
{
    *reg = (uint8_t)(((positive_input << ADS124S08_POSITIVE_INPUT_LOCATION) & 0xF0U) | 
            (negative_input & 0x0FU));
}

// PGA register helper
static inline void ads124s08_set_pga_delay_bit(uint8_t *reg, ads124s08_pga_delay_t delay)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_PGA_DELAY_MASK)) | (delay << ADS124S08_PGA_DELAY_LOCATION);
}

static inline void ads124s08_set_pga_en_bit(uint8_t *reg, ads124s08_pga_enable_t enable)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_PGA_ENABLE_MASK)) | (enable << ADS124S08_PGA_ENABLE_LOCATION);
}

static inline void ads124s08_set_pga_gain_bit(uint8_t *reg, ads124s08_pga_gain_t gain)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_PGA_GAIN_MASK)) | (gain << ADS124S08_PGA_GAIN_LOCATION);
}

// Data register helper
static inline void ads124s08_set_global_chop_en_bit(uint8_t *reg, ads124s08_chop_enable_t enable)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_CHOP_ENABLE_MASK)) | (enable << ADS124S08_CHOP_ENABLE_LOCATION);
}

static inline void ads124s08_set_clock_source_bit(uint8_t *reg, ads124s08_chop_clock_source_t clock_source)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_CHOP_CLOCK_SOURCE_MASK)) | (clock_source << ADS124S08_CHOP_CLOCK_SOURCE_LOCATION);
}

static inline void ads124s08_set_conversion_mode_bit(uint8_t *reg, ads124s08_conversion_mode_t conversion_mode)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_CONVERSION_MODE_MASK)) | (conversion_mode << ADS124S08_CONVERSION_MODE_LOCATION);
}

static inline void ads124s08_set_digital_filter_bit(uint8_t *reg, ads124s08_digital_filter_t filter)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_DIGITAL_FILTER_MASK)) | (filter << ADS124S08_DIGITAL_FILTER_LOCATION);
}

static inline void ads124s08_set_data_rate_bits(uint8_t *reg, ads124s08_data_rate_t data_rate)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_DATA_RATE_MASK)) | (data_rate << ADS124S08_DATA_RATE_LOCATION);
}

// Reference control register helpers

static inline void ads124s08_set_ref_monitor_bits(uint8_t *reg, ads124s08_reference_moninter_config_t monitor_config)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_REFERENCE_MONITOR_MASK)) | (monitor_config << ADS124S08_REFERENCE_MONITOR_LOCATION);
}  

static inline void ads124s08_set_pos_ref_bypass_bit(uint8_t *reg, ads124s08_reference_bypass_t bypass)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_POSITIVE_REFERENCE_BYPASS_MASK)) | (bypass << ADS124S08_POSITIVE_REFERENCE_BYPASS_LOCATION);
}

static inline void ads124s08_set_neg_ref_bypass_bit(uint8_t *reg, ads124s08_reference_bypass_t bypass)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_NEGATIVE_REFERENCE_BYPASS_MASK)) | (bypass << ADS124S08_NEGATIVE_REFERENCE_BYPASS_LOCATION);
}

static inline void ads124s08_set_ref_input_bits(uint8_t *reg, ads124s08_reference_input_selection_t ref_input)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_REFERENCE_SELECTION_MASK)) | (ref_input << ADS124S08_REFERENCE_SELECTION_LOCATION);
}

static inline void ads124s08_set_internal_ref_config_bits(uint8_t *reg, ads124s08_internal_reference_config_t internal_ref_config)
{
    *reg = (uint8_t)(*reg & (~ADS124S08_INTERNAL_REFERENCE_CONFIG_MASK)) | (internal_ref_config << ADS124S08_INTERNAL_REFERENCE_CONFIG_LOCATION);
}

// Excitation current register 1

static inline void ads124s08_set_idac_pga_monitor_bit(uint8_t *reg, ads124x08_idac_pga_rail_moniter_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_PGA_IDAC_RAIL_MONITOR_MASK) | (enable << ADS124S08_PGA_IDAC_RAIL_MONITOR_LOCATION));
}

static inline void ads124s08_set_idac_low_side_switch_bit(uint8_t *reg, ads124s08_idac_low_side_switch_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_IDAC_LOW_SIDE_SWITCH_MASK) | (enable << ADS124S08_IDAC_LOW_SIDE_SWITCH_LOCATION));
}

static inline void ads124s08_set_idac_magnitude_bits(uint8_t *reg, ads124s08_idac_1_current_t magnitude)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_IDAC_MAGNITUDE_MASK) | (magnitude << ADS124S08_IDAC_MAGNITUDE_LOCATION));
}

// Excitation current register 2

static inline void ads124s08_set_idac1_output_bits(uint8_t *reg, ads124s08_idac_output_mux_t mux)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_IDAC1_OUTPUT_MASK) | (mux << ADS124S08_IDAC1_OUTPUT_LOCATION));
}

static inline void ads124s08_set_idac2_output_bits(uint8_t *reg, ads124s08_idac_output_mux_t mux)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_IDAC2_OUTPUT_MASK) | (mux << ADS124S08_IDAC2_OUTPUT_LOCATION));
}

// Vbias register
static inline void ads124s08_set_vbias_level_bit(uint8_t *reg, ads124s08_vbias_level_config_t level)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_LEVEL_MASK) | (level << ADS124S08_VBIAS_LEVEL_LOCATION));
}

static inline void ads124s08_set_vbias_aincom_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_AINCOM_VBIAS_CONNECTION_MASK) | (enable << ADS124S08_AINCOM_VBIAS_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain0_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_0_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_0_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain1_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_1_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_1_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain2_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_2_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_2_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain3_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_3_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_3_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain4_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_4_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_4_CONNECTION_LOCATION));
}

static inline void ads124s08_set_vbias_ain5_bit(uint8_t *reg, ads124s08_vbias_connection_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_VBIAS_ANALOG_INPUT_5_CONNECTION_MASK) | (enable << ADS124S08_VBIAS_ANALOG_INPUT_5_CONNECTION_LOCATION));
}

// system control register
static inline void ads124s08_set_system_monitor_bits(uint8_t *reg, ads124s08_system_monitor_t monitor)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_SYSTEM_MONITOR_MASK) |
                     (monitor << ADS124S08_SYSTEM_MONITOR_LOCATION));
}

static inline void ads124s08_set_calibration_sample_size_bits(uint8_t *reg, ads124s08_calibration_sample_size_t size)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_CALIBRATION_SAMPLE_SIZE_MASK) |
                     (size << ADS124S08_CALIBRATION_SAMPLE_SIZE_LOCATION));
}

static inline void ads124s08_set_spi_timeout_bit(uint8_t *reg, ads124s08_spi_timeout_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_SPI_TIMEOUT_MASK) |
                     (enable << ADS124S08_SPI_TIMEOUT_LOCATION));
}

static inline void ads124s08_set_crc_enable_bit(uint8_t *reg, ads124s08_crc_enable_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_CRC_ENABLE_MASK) |
                     (enable << ADS124S08_CRC_ENABLE_LOCATION));
}

static inline void ads124s08_set_send_status_bit(uint8_t *reg, ads124s08_send_status_byte_t enable)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_SEND_STATUS_BYTE_MASK) |
                     (enable << ADS124S08_SEND_STATUS_BYTE_LOCATION));
}

// GPIO data register helpers
static inline void ads124s08_set_gpio0_dir(uint8_t *reg, ads124s08_gpio_data_direction_t dir)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_0_DIRECTION_MASK) |
                     (dir << ADS124S08_GPIO_0_DIRECTION_LOCATION));
}

static inline void ads124s08_set_gpio1_dir(uint8_t *reg, ads124s08_gpio_data_direction_t dir)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_1_DIRECTION_MASK) |
                     (dir << ADS124S08_GPIO_1_DIRECTION_LOCATION));
}

static inline void ads124s08_set_gpio2_dir(uint8_t *reg, ads124s08_gpio_data_direction_t dir)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_2_DIRECTION_MASK) |
                     (dir << ADS124S08_GPIO_2_DIRECTION_LOCATION));
}

static inline void ads124s08_set_gpio3_dir(uint8_t *reg, ads124s08_gpio_data_direction_t dir)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_3_DIRECTION_MASK) |
                     (dir << ADS124S08_GPIO_3_DIRECTION_LOCATION));
}

static inline void ads124s08_set_gpio0_value(uint8_t *reg, ads124s08_gpio_value_t val)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_0_DATA_MASK) |
                     (val << ADS124S08_GPIO_0_DATA_LOCATION));
}

static inline void ads124s08_set_gpio1_value(uint8_t *reg, ads124s08_gpio_value_t val)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_1_DATA_MASK) |
                     (val << ADS124S08_GPIO_1_DATA_LOCATION));
}

static inline void ads124s08_set_gpio2_value(uint8_t *reg, ads124s08_gpio_value_t val)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_2_DATA_MASK) |
                     (val << ADS124S08_GPIO_2_DATA_LOCATION));
}

static inline void ads124s08_set_gpio3_value(uint8_t *reg, ads124s08_gpio_value_t val)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_3_DATA_MASK) |
                     (val << ADS124S08_GPIO_3_DATA_LOCATION));
}

// GPIO configuration register helpers
static inline void ads124s08_set_gpio0_config(uint8_t *reg, ads124s08_gpio_configuration_t cfg)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_0_CONFIG_MASK) |
                     (cfg << ADS124S08_GPIO_0_CONFIG_LOCATION));
}

static inline void ads124s08_set_gpio1_config(uint8_t *reg, ads124s08_gpio_configuration_t cfg)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_1_CONFIG_MASK) |
                     (cfg << ADS124S08_GPIO_1_CONFIG_LOCATION));
}

static inline void ads124s08_set_gpio2_config(uint8_t *reg, ads124s08_gpio_configuration_t cfg)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_2_CONFIG_MASK) |
                     (cfg << ADS124S08_GPIO_2_CONFIG_LOCATION));
}

static inline void ads124s08_set_gpio3_config(uint8_t *reg, ads124s08_gpio_configuration_t cfg)
{
    *reg = (uint8_t)((*reg & ~ADS124S08_GPIO_3_CONFIG_MASK) |
                     (cfg << ADS124S08_GPIO_3_CONFIG_LOCATION));
}

#endif /* INSTRUMENTATION_DRV_ADS124S08_REGS_H */