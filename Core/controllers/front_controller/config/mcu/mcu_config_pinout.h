#ifndef CONFIG_MCU_CONFIG_PINOUT_H_
#define CONFIG_MCU_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"


/*============================================================================*/
/* High Side Drivers: HSD                                                     */
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

extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;

// Front Controller ADC Mapping
#define FCO_MCU_ADC_2_HANDLE (&hadc2)
#define FCO_MCU_ADC_3_HANDLE (&hadc3)

/*============================================================================*/
/* GPIO Configuration                                                         */
/*============================================================================*/

/* Active LOW input (button connects to GND when pressed) */
#define TCU_RTD_BUTTON_GPIO_PORT            GPIOF
#define TCU_RTD_BUTTON_GPIO_PIN             GPIO_PIN_14
#define TCU_RTD_BUTTON_ACTIVE_STATE         GPIO_PIN_RESET

#define PTT_CTL_GPIO_Port                   GPIOE
#define PTT_CTL_GPIO_Pin                    GPIO_PIN_11

#endif /* CONFIG_MCU_CONFIG_PINOUT_H_ */
