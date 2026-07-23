#ifndef CONFIG_MCU_CONFIG_H_
#define CONFIG_MCU_CONFIG_H_

#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#include "pdm_config.h"
#include "mcu_config_analog_channels.h"
#include "mcu_config_analog.h"

#include "mcu_config_hsd.h"

/*============================================================================*/
/* HSD Mapping                                                                */
/*============================================================================*/
// currently not used in front controller 
extern const mcu_channel_config_t mcu_hsd_default_config[MCU_HSD_TOTAL_CHANNELS];

/*============================================================================*/
/* Front Controller ADC Mapping                                               */
/*============================================================================*/

#define APPS1    FCO_ADC3_INP1 
#define APPS2    FCO_ADC3_INP0
#define BSE1     FCO_ADC2_INP0

/*============================================================================*/
/* ADC Configuration                                                          */
/*============================================================================*/
#define MCU_ADC_2_VREF_V              2.5f
#define MCU_ADC_3_VREF_V              2.5f


extern const mcu_analog_config_t mcu_analog_config[MCU_TOTAL_ADC_INPUT_COUNT];

#endif /* CONFIG_MCU_CONFIG_H_ */
