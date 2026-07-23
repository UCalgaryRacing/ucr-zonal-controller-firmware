#ifndef CONFIG_MCU_CONFIG_PINOUT_H_
#define CONFIG_MCU_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"


/*============================================================================*/
/* High Side Drivers - HSD                                                    */
/*============================================================================*/

#define HSD_5V_FAST_1_CTL_GPIO_Port     GPIOI
#define HSD_5V_FAST_1_CTL_GPIO_Pin      GPIO_PIN_6

#define HSD_5V_FAST_2_CTL_GPIO_Port     GPIOI
#define HSD_5V_FAST_2_CTL_GPIO_Pin      GPIO_PIN_5

#define HSD_12V_1_CTL_GPIO_Port         GPIOI
#define HSD_12V_1_CTL_GPIO_Pin          GPIO_PIN_3

#define HSD_12V_2_CTL_GPIO_Port         GPIOI
#define HSD_12V_2_CTL_GPIO_Pin          GPIO_PIN_1

#define HSD_5V_SLOW_CTL_GPIO_Port       GPIOB
#define HSD_5V_SLOW_CTL_GPIO_Pin        GPIO_PIN_7


/*============================================================================*/
/* ADC Interface                                                              */
/*============================================================================*/

extern ADC_HandleTypeDef hadc3;

extern DMA_HandleTypeDef hdma_adc3;

// Rear Controller ADC Mapping
#define RCO_ADC_3_HANDLE (&hadc3)

#endif /* CONFIG_MCU_CONFIG_PINOUT_H_ */
