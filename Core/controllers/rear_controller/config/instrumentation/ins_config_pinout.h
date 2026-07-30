#ifndef CONFIG_INS_CONFIG_PINOUT_H_
#define CONFIG_INS_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

extern TIM_HandleTypeDef htim2;

#define INS_FRONT_WHEEL_TIMER_HANDLE           (&htim2)
#define INS_FRONT_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1

extern SPI_HandleTypeDef hspi1;

#define INS_SPI_HANDLE (&hspi1)

#define INS_CS_0_PORT GPIOI
#define INS_CS_0_PIN GPIO_PIN_4

#define INS_CS_1_PORT GPIOG
#define INS_CS_1_PIN GPIO_PIN_10

#endif