#ifndef CONFIG_INS_CONFIG_H_
#define CONFIG_INS_CONFIG_H_


#include "stm32h7xx.h"
#include "stm32h753xx.h"
#include "stm32h7xx_hal.h"

#include "ins_config_sensor_id.h"
#include "ins_drv_ads124s08_regs.h"

/*============================================================================*/
/* Wheel Speed: used for MCU V1.0 wheel speed implementation                  */
/*============================================================================*/

/* Maps to ins_wheel_speed_sensor_t; validated at runtime in service init. */
#define INS_FRONT_WHEEL_SENSOR_ID                     0U
/* TIM2 counter tick frequency = TIM2CLK (240 MHz) / (Prescaler 239 + 1) = 1 MHz. */
#define INS_FRONT_WHEEL_TIMER_FREQUENCY_HZ            1000000U
#define INS_FRONT_WHEEL_PULSES_PER_ROTATION           30U
#define INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS            2000U
/* Number of 10 ms fast-task samples averaged to smooth brief RPM drops. */
#define INS_FRONT_WHEEL_RPM_MOVING_AVERAGE_SAMPLES    10U

/*============================================================================*/
/* Instrumentation Module Config                                              */
/*============================================================================*/
#define INS_EXTERNAL_VREF_V             5.0f    // external 5V reference provided to instrumentation module by PRM
#define INS_SCALING_FACTOR              2.4f    // from voltage divider on instrumentation module (all read inputs), set for 12V input

/*============================================================================*/
/* ADS124S08 Hardware Mapping                                                 */
/*============================================================================*/
typedef enum
{
    INS_ADC_1 = 0U,
    INS_ADC_2,
    INS_TOTAL_NUM_ADC
} ins_adc_id_t;

/**
 * Different channel types of the instrumentation module
 * There 12 individual analog inputs on each ADC
 * On V2 of instrumentation there are two ADC, one has the input conditioning for single ended measurements, the other has the conditioning for differential measurements
 */
typedef enum
{   
    /* Single ended channels are all on one ADC. All negatv*/
    INS_SING_0 = 0U, /* Single ended channel with positive input on AIN1 and negative input on AINCOM */
    INS_SING_1,      /* Single ended channel with positive input on AIN2 and negative input on AINCOM */
    INS_SING_2,      /* Single ended channel with positive input on AIN0 and negative input on AINCOM */
    INS_SING_3,      /* Single ended channel with positive input on AIN3 and negative input on AINCOM */
    INS_SING_4,      /* Single ended channel with positive input on AIN8 and negative input on AINCOM */
    INS_SING_5,      /* Single ended channel with positive input on AIN9 and negative input on AINCOM */
    INS_SING_6,      /* Single ended channel with positive input on AIN10 and negative input on AINCOM */
    INS_SING_7,      /* Single ended channel with positive input on AIN11 and negative input on AINCOM */
    INS_SING_8,      /* Single ended channel with positive input on AIN7 and negative input on AINCOM */
    INS_SING_9,      /* Single ended channel with positive input on AIN5 and negative input on AINCOM */
    INS_SING_10,     /* Single ended channel with positive input on AIN6 and negative input on AINCOM */
    INS_SING_11,     /* Single ended channel with positive input on AIN4 and negative input on AINCOM */

    /* Differential channels are all on the other ADC*/
    INS_DIFF_1,      /* Differential channel with positive input on AIN6 and negative input on AIN7 */
    INS_DIFF_2,      /* Differential channel with positive input on AIN5 and negative input on AIN4 */
    INS_DIFF_3,      /* Differential channel with positive input on AIN2 and negative input on AIN3 */
    INS_DIFF_4,      /* Differential channel with positive input on AIN1 and negative input on AIN0 */
    INS_DIFF_5,      /* Differential channel with positive input on AIN9 and negative input on AIN8 */
    INS_DIFF_6,      /* Differential channel with positive input on AIN10 and negative input on AIN11 */

    INS_TOTAL_NUM_CHANNEL
} ins_channel_id_t;

typedef struct
{
    ins_channel_id_t channel_id;
} ins_sensor_config_t;

/*============================================================================*/
/* Channel Hardware Mapping                                                   */
/*============================================================================*/

/* Hardware Mapping for ADS124S08 */

typedef struct 
{   
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *cs_port;               // Chip Select GPIO Port. Not supported on MCU V1.0
    uint16_t      cs_pin;                // Chip Select GPIO Pin. Not supported on MCU V1.0
    ads124s08_shawdow_t *shadow;
    bool is_en;
} ads124s08_hw_t;

typedef struct 
{
    const ads124s08_hw_t *hw;
    ads124s08_input_mux_t input_pos_pin; // Positive input pin for the channel
    ads124s08_input_mux_t input_neg_pin; // Negative input pin for the channel
    ads124s08_input_mux_t input_gpio_pin;
    bool is_en;
} ins_channel_config_t;

/*============================================================================*/
/* ADS124S08 ADC Configuration                                                */
/*============================================================================*/

/* given values from datasheet */
#define ADS124S08_INTERNAL_VREF_V           2.5f        // internal reference on ADCs
#define ADS124S08_ADC_RESOLUTION_BITS       24U         // 24-bit data recording

#define ADS124S08_MAX_VALUE                 ((1U << (ADS124S08_ADC_RESOLUTION_BITS - 1)) - 1U)  // max ADC raw value given for given resolution


/*============================================================================*/
/* Extern                                                                     */
/*============================================================================*/

extern const ins_channel_config_t ins_default_config[INS_TOTAL_NUM_CHANNEL];
extern ads124s08_hw_t ins_adc_array[INS_TOTAL_NUM_ADC];

extern const ins_sensor_config_t ins_sensor_config[INS_TOTAL_NUM_SENSORS];

#endif /*CONFIG_INS_CONFIG_H_*/
