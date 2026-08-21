#ifndef PRM_CONFIG_H_
#define PRM_CONFIG_H_

#define PRM_TOTAL_NUMBER_CHANNELS 7U

#include <stdbool.h>
#include "stm32h7xx_hal.h"

typedef struct
{
    GPIO_TypeDef* en_port;
    uint16_t en_pin;
} tlv767xx_hw_t;

typedef struct
{
    tlv767xx_hw_t hw; //currently only one chip used on PRM that MCU can enable
    bool enabled;
    // TODO: Add ADC handle for current measurement and thermistor measurement
} prm_channel_config_t;

extern const prm_channel_config_t prm_default_config[PRM_TOTAL_NUMBER_CHANNELS]; 

#endif /* PRM_CONFIG_H_ */