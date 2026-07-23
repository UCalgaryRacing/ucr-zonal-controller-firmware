#ifndef CONFIG_MCU_CONFIG_H_
#define CONFIG_MCU_CONFIG_H_

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#include "mcu_config_hsd.h"

#include "mcu_config_analog_channels.h"
#include "mcu_config_analog.h"

/*============================================================================*/
/* HSD Mapping                                                                */
/*============================================================================*/

#define TS_FAN_PWM     HSD_5V_FAST_2_CTL
#define ACCU_FAN_PWM   HSD_5V_FAST_1_CTL
#define BAMOCAR_RFE    HSD_12V_1_CTL
#define BAMOCAR_FRG    HSD_12V_2_CTL 
#define DRS_PULSE      HSD_5V_SLOW_CTL

extern const mcu_channel_config_t mcu_hsd_default_config[MCU_HSD_TOTAL_CHANNELS];

/*============================================================================*/
/* Rear Controller ADC Mapping                                                */
/*============================================================================*/

#define BATT_HIGHEST_TEMP      RCO_ADC3_INP1 
#define BATT_LOWEST_VOLTAGE    RCO_ADC3_INP0

/*============================================================================*/
/* ADC Configuration                                                          */
/*============================================================================*/
#define MCU_ADC_3_VREF_V              2.5f


extern const mcu_analog_config_t mcu_analog_config[MCU_TOTAL_ADC_INPUT_COUNT];

#endif /* CONFIG_MCU_CONFIG_H_ */
