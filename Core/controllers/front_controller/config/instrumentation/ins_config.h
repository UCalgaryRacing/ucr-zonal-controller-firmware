#ifndef CONFIG_INS_CONFIG_H_
#define CONFIG_INS_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "ins_drv_ads124s08_regs.h"

/*============================================================================*/
/* Wheel Speed                                                                */
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

#define MCU_V1_0

// Value of Internal Reference on the ADCs
#define ADS124S08_INTERNAL_REF     2.5f

// Number of bits the ADS124S08 can record data in
#define ADS124S08_ADC_RESOLUTION_BITS  24U

// External reference provided to instrumenation module
#define INSTRUMENTATION_EXTERNAL_REFERENCE 5.0f

// Maximum value that the ADS124S08 can record in ADC counts
#define ADS124S08_MAX_VALUE    ((1U << (ADS124S08_ADC_RESOLUTION_BITS - 1)) - 1U)

// Maximum number of inputs that a single ADS124S08 can record from when configured as all single ended channels
#define ADS124S08_NUM_SINGLE_ENDED_CHANNELS 12U

// Maximum number of inputs that a single ADS124S08 can record from when configured as all differential channels
#define ADS124S08_NUM_DIFFERENTIAL_CHANNELS 0U

// maximum number of channels that can be configured when one ADC is entirely single ended and the other is entirely differential
#define INSTRUMENTATION_NUM_CHANNEL (ADS124S08_NUM_DIFFERENTIAL_CHANNELS + ADS124S08_NUM_SINGLE_ENDED_CHANNELS)

// Number of ADCs in V2 of the instrumenation module
#define INSTRUMENTATION_NUM_ADCS 2U

// Scaling factor, this is from the dividers on the module. Set for 12V inputs on V2.1
#define INSTRUMENTATION_SCALING_FACTOR 2.4f


/*============================================================================*/
/* Channel Hardware Mapping                                                   */
/*============================================================================*/

/**
 * Hardware mapping for the ADS124S08
 * Contains the SPI handle, chip select GPIO port and pin, and shadow register for the device
 */

typedef struct 
{   
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *cs_port;               // Chip Select GPIO Port. Not supported on MCU V1.0
    uint16_t      cs_pin;                // Chip Select GPIO Pin. Not supported on MCU V1.0
    ads124s08_shawdow_t *shadow;
} ads124s08_hw_t;


typedef struct 
{
    const ads124s08_hw_t *hw;                   /* Hardware map*/
    ads124s08_input_mux_t input_pos_pin; // Positive input pin for the channel
    ads124s08_input_mux_t input_neg_pin; // Negative input pin for the channel
} instrumentation_channel_t;

/**
 * Different channel types of the instrumentation module
 * There 12 individual analog inputs on each ADC
 * On V2 of instrumentation there are two ADC, one has the input conditioning for single ended measurements, the other has the conditioning for differential measurements
 */

typedef enum
{   
    /* Single ended channels are all on one ADC*/
    SINGLE_ENDED_0 = 0U, /* Single ended channel with positive input on AIN0 and negative input on AINCOM */
    SINGLE_ENDED_1,      /* Single ended channel with positive input on AIN1 and negative input on AINCOM */
    SINGLE_ENDED_2,      /* Single ended channel with positive input on AIN2 and negative input on AINCOM */
    SINGLE_ENDED_3,      /* Single ended channel with positive input on AIN3 and negative input on AINCOM */
    SINGLE_ENDED_4,      /* Single ended channel with positive input on AIN4 and negative input on AINCOM */
    SINGLE_ENDED_5,      /* Single ended channel with positive input on AIN5 and negative input on AINCOM */
    SINGLE_ENDED_6,      /* Single ended channel with positive input on AIN6 and negative input on AINCOM */
    SINGLE_ENDED_7,      /* Single ended channel with positive input on AIN7 and negative input on AINCOM */
    SINGLE_ENDED_8,      /* Single ended channel with positive input on AIN8 and negative input on AINCOM */
    SINGLE_ENDED_9,     /* Single ended channel with positive input on AIN9 and negative input on AINCOM */
    SINGLE_ENDED_10,     /* Single ended channel with positive input on AIN10 and negative input on AINCOM */
    SINGLE_ENDED_11,     /* Single ended channel with positive input on AIN11 and negative input on AINCOM */

    /* Differential channels are all on the other ADC*/
    DIFFERENTIAL_1,      /* Differential channel with positive input on AIN11 and negative input on AIN10 */
    DIFFERENTIAL_2,      /* Differential channel with positive input on AIN9 and negative input on AIN8 */
    DIFFERENTIAL_3,      /* Differential channel with positive input on AIN4 and negative input on AIN5 */
    DIFFERENTIAL_4,      /* Differential channel with positive input on AIN6 and negative input on AIN7 */
    DIFFERENTIAL_5,      /* Differential channel with positive input on AIN1 and negative input on AIN0 */
    DIFFERENTIAL_6       /* Differential channel with positive input on AIN3 and negative input on AIN2 */
} instrumentation_channel_id_t;

extern const instrumentation_channel_t ins_default_config[INSTRUMENTATION_NUM_CHANNEL];
extern ads124s08_hw_t ins_adc_array[INSTRUMENTATION_NUM_ADCS];

#endif