#ifndef CONFIG_INS_CONFIG_PINOUT_H_
#define CONFIG_INS_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

/*============================================================================*/
/* Pinout for Wheel Speed                                                */
/*============================================================================*/

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

#define INS_RL_WHEEL_TIMER_HANDLE           (&htim5)
#define INS_RL_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1

#define INS_RR_WHEEL_TIMER_HANDLE           (&htim2)
#define INS_RR_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1


/*============================================================================*/
/* Pinout for Instrumentation Board                                           */
/*============================================================================*/
extern SPI_HandleTypeDef hspi1;

#define INS_SPI_HANDLE (&hspi1)

#define INS_CS_0_PORT GPIOI
#define INS_CS_0_PIN GPIO_PIN_4

#define INS_CS_1_PORT GPIOG
#define INS_CS_1_PIN GPIO_PIN_10

#define INS_RESET_PORT GPIOJ
#define INS_RESET_PIN GPIO_PIN_15

#endif