#ifndef MCU_CONFIG_ANALOG_H_
#define MCU_CONFIG_ANALOG_H_

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/*============================================================================*/
/* System Topology                                                            */
/*============================================================================*/

typedef struct {
    ADC_HandleTypeDef *adc_handle;
    float adc_vref;
    uint16_t adc_max;  
    bool calibrated;
    bool dma_started;
    uint8_t adc_channels;
} mcu_adc_context_t;

/*============================================================================*/
/* Channel Hardware Mapping                                                   */
/*============================================================================*/

typedef struct
{
    mcu_adc_context_t *adc_context;
    uint8_t adc_buffer_number;
    uint8_t adc_buffer_index;
} analog_hw_t;

/*============================================================================*/
/* Analog calibration (one row in mcu_analog_config)                          */
/*============================================================================*/

typedef struct
{
    float valid_min_v;
    float valid_max_v;
    float active_min_v;
    float active_max_v;
    float scaling_factor;
    bool positive_slope;
} mcu_analog_settings_t;

typedef struct
{
    bool enabled;  
    mcu_analog_settings_t settings;
    analog_hw_t hw;           
} mcu_analog_config_t;

/*============================================================================*/
/* ADC Configuration                                                          */
/*============================================================================*/

#define MCU_ADC_2_RESOLUTION_BITS     16U
#define MCU_ADC_2_MAX_COUNT           ((1U << MCU_ADC_2_RESOLUTION_BITS) - 1U)

#define MCU_ADC_3_RESOLUTION_BITS     16U
#define MCU_ADC_3_MAX_COUNT           ((1U << MCU_ADC_3_RESOLUTION_BITS) - 1U)

#endif /*MCU_CONFIG_ANALOG_H_*/