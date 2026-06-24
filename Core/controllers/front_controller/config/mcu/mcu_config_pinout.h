#ifndef CONFIG_MCU_CONFIG_PINOUT_H_
#define CONFIG_MCU_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"


/*============================================================================*/
/* TPS4H000 Device 1                                        */
/*============================================================================*/

#define MCU_TPS4XXXX_1_IN1_GPIO_Port  GPIOI
#define MCU_TPS4XXXX_1_IN1_Pin        GPIO_PIN_7

#define MCU_TPS4XXXX_1_IN2_GPIO_Port  GPIOI
#define MCU_TPS4XXXX_1_IN2_Pin        GPIO_PIN_6

#define MCU_TPS4XXXX_1_IN3_GPIO_Port  GPIOI
#define MCU_TPS4XXXX_1_IN3_Pin        GPIO_PIN_5

#define MCU_TPS4XXXX_1_IN4_GPIO_Port  GPIOI
#define MCU_TPS4XXXX_1_IN4_Pin        GPIO_PIN_4




/*============================================================================*/
/* ADC Interface                                                              */
/*============================================================================*/

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc1;

#define TCU_ADC_1_HANDLE (&hadc1)
#define TCU_ADC_3_HANDLE (&hadc3)

/*============================================================================*/
/*  Analog In 3: APPS1                                                        */
/*============================================================================*/

//TODO determine if these are determined by ADC rank or can be assigned
//Make sure they always match cubeMX, rank determines index in array
//will probably switch to adc polling / interrupt instead of dma to use across multiple files
//without having to extern the dma buffer from this file, but maybe not if this is only analog on MCU board....

#define TCU_AIN3_ADC_HANDLE                (&hadc3)
#define TCU_AIN3_ADC_BUFFER_NUMBER 2    //ADC3_INP12 
#define TCU_AIN3_ADC_BUFFER_INDEX  0
#define TCU_AIN_3_ADC_CHANNELS     2


/*============================================================================*/
/*  Analog In 4: APPS2                                                        */
/*============================================================================*/

#define TCU_AIN4_ADC_HANDLE                (&hadc3)
#define TCU_AIN4_ADC_BUFFER_NUMBER 2    //ADC3_INP13
#define TCU_AIN4_ADC_BUFFER_INDEX  1
#define TCU_AIN_4_ADC_CHANNELS     2

/*============================================================================*/
/*  Analog In 7: BSE                                                          */
/*============================================================================*/

#define TCU_AIN7_ADC_HANDLE                (&hadc1)
#define TCU_AIN7_ADC_BUFFER_NUMBER 0    //ADC1_INP15
#define TCU_AIN7_ADC_BUFFER_INDEX  0
#define TCU_AIN_7_ADC_CHANNELS     1


/*============================================================================*/
/* GPIO Configuration                                                         */
/*============================================================================*/

/* Active LOW input (button connects to GND when pressed) */
#define TCU_RTD_BUTTON_GPIO_PORT            GPIOA
#define TCU_RTD_BUTTON_GPIO_PIN             GPIO_PIN_5
#define TCU_RTD_BUTTON_ACTIVE_STATE         GPIO_PIN_RESET

#endif /* CONFIG_MCU_CONFIG_PINOUT_H_ */
