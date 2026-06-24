#ifndef CONFIG_PDM_HW_V1_H_
#define CONFIG_PDM_HW_V1_H_

#include "stm32h7xx_hal.h"

/*============================================================================*/
/* SPI Interface                                                              */
/*============================================================================*/

extern SPI_HandleTypeDef hspi5;

#define PDM_SPI_HANDLE                (&hspi5)

/*============================================================================*/
/* ADC Interface (for TPS4H160 current sense)                                 */
/*============================================================================*/

extern ADC_HandleTypeDef hadc1;

#define PDM_ADC_HANDLE                (&hadc1)

/*============================================================================*/
/* VNF9Q20F Device 0 - Channels 1-4                                           */
/*============================================================================*/

#define PDM_VNF9Q20F_0_SHADOW_REGISTER

#define PDM_VNF9Q20F_0_CSN_GPIO_Port   GPIOF
#define PDM_VNF9Q20F_0_CSN_Pin         GPIO_PIN_11

/* Active low: HIGH = active, LOW = standby */
#define PDM_VNF9Q20F_0_STDBY_NOT_GPIO_Port   GPIOD
#define PDM_VNF9Q20F_0_STDBY_NOT_Pin         GPIO_PIN_15

/*============================================================================*/
/* VNF9Q20F Device 1 - Channels 5-8                                           */
/*============================================================================*/

#define PDM_VNF9Q20F_1_CSN_GPIO_Port   GPIOF
#define PDM_VNF9Q20F_1_CSN_Pin         GPIO_PIN_13

/* Active low: HIGH = active, LOW = standby */
#define PDM_VNF9Q20F_1_STDBY_NOT_GPIO_Port   GPIOA
#define PDM_VNF9Q20F_1_STDBY_NOT_Pin         GPIO_PIN_7

/*============================================================================*/
/* TPS4H160 Device 0 - Channels 9-12                                          */
/*============================================================================*/

#define PDM_TPS4XXXX_0_IN1_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_IN1_Pin        GPIO_PIN_11

#define PDM_TPS4XXXX_0_IN2_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_IN2_Pin        GPIO_PIN_11

#define PDM_TPS4XXXX_0_IN3_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_IN3_Pin        GPIO_PIN_2

#define PDM_TPS4XXXX_0_IN4_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_IN4_Pin        GPIO_PIN_3

#define PDM_TPS4XXXX_0_DIAG_EN_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_DIAG_EN_Pin        GPIO_PIN_4

#define PDM_TPS4XXXX_0_SEH_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_SEH_Pin        GPIO_PIN_6

#define PDM_TPS4XXXX_0_SEL_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_0_SEL_Pin        GPIO_PIN_7

/* Open-drain from IC, active LOW */
#define PDM_TPS4XXXX_0_FAULT_GPIO_Port GPIOE
#define PDM_TPS4XXXX_0_FAULT_Pin       GPIO_PIN_8

#define PDM_TPS4XXXX_0_CS_ADC_HANDLE   PDM_ADC_HANDLE
#define PDM_TPS4XXXX_0_CS_ADC_CHANNEL  ADC_CHANNEL_0

/*============================================================================*/
/* TPS4H160 Device 1 - Channels 13-16                                         */
/*============================================================================*/

#define PDM_TPS4XXXX_1_IN1_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_IN1_Pin        GPIO_PIN_10

#define PDM_TPS4XXXX_1_IN2_GPIO_Port  GPIOC
#define PDM_TPS4XXXX_1_IN2_Pin        GPIO_PIN_0

#define PDM_TPS4XXXX_1_IN3_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_IN3_Pin        GPIO_PIN_6

#define PDM_TPS4XXXX_1_IN4_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_IN4_Pin        GPIO_PIN_5

#define PDM_TPS4XXXX_1_DIAG_EN_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_DIAG_EN_Pin        GPIO_PIN_4

#define PDM_TPS4XXXX_1_SEH_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_SEH_Pin        GPIO_PIN_6

#define PDM_TPS4XXXX_1_SEL_GPIO_Port  GPIOF
#define PDM_TPS4XXXX_1_SEL_Pin        GPIO_PIN_7

/* Open-drain from IC, active LOW */
#define PDM_TPS4XXXX_1_FAULT_GPIO_Port GPIOF
#define PDM_TPS4XXXX_1_FAULT_Pin       GPIO_PIN_8

#define PDM_TPS4XXXX_1_CS_ADC_HANDLE   PDM_ADC_HANDLE
#define PDM_TPS4XXXX_1_CS_ADC_CHANNEL  ADC_CHANNEL_1

/*============================================================================*/
/* TPS4H160 Device 2 - Regulated 12V Channels 17-20                           */
/*============================================================================*/

#define PDM_TPS4XXXX_2_IN1_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_IN1_Pin        GPIO_PIN_0

#define PDM_TPS4XXXX_2_IN2_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_IN2_Pin        GPIO_PIN_1

#define PDM_TPS4XXXX_2_IN3_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_IN3_Pin        GPIO_PIN_2

#define PDM_TPS4XXXX_2_IN4_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_IN4_Pin        GPIO_PIN_3

#define PDM_TPS4XXXX_2_DIAG_EN_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_DIAG_EN_Pin        GPIO_PIN_4

#define PDM_TPS4XXXX_2_SEH_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_SEH_Pin        GPIO_PIN_6

#define PDM_TPS4XXXX_2_SEL_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_SEL_Pin        GPIO_PIN_7

/* Open-drain from IC, active LOW */
#define PDM_TPS4XXXX_2_FAULT_GPIO_Port GPIOG
#define PDM_TPS4XXXX_2_FAULT_Pin       GPIO_PIN_8

#define PDM_TPS4XXXX_2_CS_ADC_HANDLE   PDM_ADC_HANDLE
#define PDM_TPS4XXXX_2_CS_ADC_CHANNEL  ADC_CHANNEL_2

/*============================================================================*/
/* INA229 Chip Select Pins                                                    */
/*============================================================================*/

#define PDM_INA229_GLV_CS_GPIO_Port   GPIOH
#define PDM_INA229_GLV_CS_Pin         GPIO_PIN_4

#define PDM_LM5060_GLV_TIMER_GPIO_PORT   GPIOF
#define PDM_LM5060_GLV_TIMER_PIN        GPIO_PIN_4

#endif /* CONFIG_PDM_HW_V1_H_ */
