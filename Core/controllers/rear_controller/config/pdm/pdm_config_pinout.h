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
// not used currently 

// extern ADC_HandleTypeDef                        hadc2;
// extern ADC_HandleTypeDef                        hadc3;

// #define PDM_EFUSE_34_ADC_HANDLE                 (&hadc3)
// #define PDM_EFUSE_REG_ADC_HANDLE                (&hadc2)

/*============================================================================*/
/* TPS4XXXX Shared PINs                                                        */
/*============================================================================*/
// DIAG_EN (diagnostics) pin not currently initialized in CubeMX
#define PDM_TPS4XXXX_DIAG_EN_Port               GPIOD
#define PDM_TPS4XXXX_DIAG_EN_Pin                GPIO_PIN_15

#define PDM_TPS4XXXX_SEH_GPIO_Port              GPIOB
#define PDM_TPS4XXXX_SEH_GPIO_Pin               GPIO_PIN_12

#define PDM_TPS4XXXX_SEL_GPIO_Port              GPIOD
#define PDM_TPS4XXXX_SEL_GPIO_Pin               GPIO_PIN_11

/*============================================================================*/
/* VNF9Q20F Device 0 - Channels 1-4 - EFUSE 1                                 */
/*============================================================================*/

#define PDM_VNF9Q20F_0_SHADOW_REGISTER

#define PDM_VNF9Q20F_0_CSN_GPIO_Port                GPIOD
#define PDM_VNF9Q20F_0_CSN_Pin                      GPIO_PIN_14

/* Active low: HIGH = active, LOW = standby */
// not currently initialized in CubeMX
#define PDM_VNF9Q20F_0_STDBY_NOT_GPIO_Port          GPIOJ
#define PDM_VNF9Q20F_0_STDBY_NOT_Pin                GPIO_PIN_3

/*============================================================================*/
/* VNF9Q20F Device 1 - Channels 5-8 - EFUSE 2                                 */
/*============================================================================*/

#define PDM_VNF9Q20F_1_CSN_GPIO_Port                GPIOH
#define PDM_VNF9Q20F_1_CSN_Pin                      GPIO_PIN_11

/* Active low: HIGH = active, LOW = standby */
// not currently initialized in CubeMX
#define PDM_VNF9Q20F_1_STDBY_NOT_GPIO_Port          GPIOJ
#define PDM_VNF9Q20F_1_STDBY_NOT_Pin                GPIO_PIN_13

/*============================================================================*/
/* TPS4H160 Device 0 - Channels 9-12 - EFUSE 3                                */
/*============================================================================*/

#define PDM_TPS4XXXX_0_IN1_GPIO_Port                GPIOK
#define PDM_TPS4XXXX_0_IN1_Pin                      GPIO_PIN_1

#define PDM_TPS4XXXX_0_IN2_GPIO_Port                GPIOG
#define PDM_TPS4XXXX_0_IN2_Pin                      GPIO_PIN_7

#define PDM_TPS4XXXX_0_IN3_GPIO_Port                GPIOG
#define PDM_TPS4XXXX_0_IN3_Pin                      GPIO_PIN_6

#define PDM_TPS4XXXX_0_IN4_GPIO_Port                GPIOD
#define PDM_TPS4XXXX_0_IN4_Pin                      GPIO_PIN_1

/* Open-drain from IC, active LOW */
// not currently initialized in CubeMX
#define PDM_TPS4XXXX_0_FAULT_GPIO_Port              GPIOA
#define PDM_TPS4XXXX_0_FAULT_Pin                    GPIO_PIN_8

// #define PDM_TPS4XXXX_0_CS_ADC_HANDLE                PDM_EFUSE_34_ADC_HANDLE
// #define PDM_TPS4XXXX_0_CS_ADC_CHANNEL               ADC_CHANNEL_2

/*============================================================================*/
/* TPS4H160 Device 1 - Channels 13-16 - EFUSE 4                               */
/*============================================================================*/

#define PDM_TPS4XXXX_1_IN1_GPIO_Port  GPIOC
#define PDM_TPS4XXXX_1_IN1_Pin        GPIO_PIN_13

#define PDM_TPS4XXXX_1_IN2_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_1_IN2_Pin        GPIO_PIN_12

#define PDM_TPS4XXXX_1_IN3_GPIO_Port  GPIOD
#define PDM_TPS4XXXX_1_IN3_Pin        GPIO_PIN_5

#define PDM_TPS4XXXX_1_IN4_GPIO_Port  GPIOE
#define PDM_TPS4XXXX_1_IN4_Pin        GPIO_PIN_6

/* Open-drain from IC, active LOW */
// not currently initialized in CubeMX
#define PDM_TPS4XXXX_1_FAULT_GPIO_Port GPIOI
#define PDM_TPS4XXXX_1_FAULT_Pin       GPIO_PIN_0

// #define PDM_TPS4XXXX_1_CS_ADC_HANDLE   PDM_EFUSE_34_ADC_HANDLE
// #define PDM_TPS4XXXX_1_CS_ADC_CHANNEL  ADC_CHANNEL_3

/*============================================================================*/
/* TPS4H160 Device 2 - Regulated 12V Channels                                 */
/*============================================================================*/

#define PDM_TPS4XXXX_2_IN1_GPIO_Port  GPIOG
#define PDM_TPS4XXXX_2_IN1_Pin        GPIO_PIN_5

#define PDM_TPS4XXXX_2_IN2_GPIO_Port  GPIOJ
#define PDM_TPS4XXXX_2_IN2_Pin        GPIO_PIN_8

#define PDM_TPS4XXXX_2_IN3_GPIO_Port  GPIOH
#define PDM_TPS4XXXX_2_IN3_Pin        GPIO_PIN_10

#define PDM_TPS4XXXX_2_IN4_GPIO_Port  GPIOB
#define PDM_TPS4XXXX_2_IN4_Pin        GPIO_PIN_10

/* Open-drain from IC, active LOW */
// not currently initialized in CubeMX
#define PDM_TPS4XXXX_2_FAULT_GPIO_Port GPIOE
#define PDM_TPS4XXXX_2_FAULT_Pin       GPIO_PIN_12

// #define PDM_TPS4XXXX_2_CS_ADC_HANDLE   PDM_EFUSE_REG_ADC_HANDLE
// #define PDM_TPS4XXXX_2_CS_ADC_CHANNEL  ADC_CHANNEL_3

/*============================================================================*/
/* Input Protection - INA229 and LM5060                                       */
/*============================================================================*/
#define PDM_INA229_GLV_CS_GPIO_Port    GPIOF
#define PDM_INA229_GLV_CS_Pin          GPIO_PIN_13

#define PDM_LM5060_GLV_TIMER_GPIO_PORT GPIOH // PDM VBATT_LATCH_OFF
#define PDM_LM5060_GLV_TIMER_PIN       GPIO_PIN_15

#endif /* CONFIG_PDM_HW_V1_H_ */
