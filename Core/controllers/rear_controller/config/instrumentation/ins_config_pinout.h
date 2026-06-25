#ifndef CONFIG_INS_CONFIG_PINOUT_H_
#define CONFIG_INS_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

extern TIM_HandleTypeDef htim2;

#define INS_FRONT_WHEEL_TIMER_HANDLE           (&htim2)
#define INS_FRONT_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1

extern SPI_HandleTypeDef hspi1;

#define INS_SPI_HANDLE (&hspi1)
// mcu v1.0 does not support chip select pins for the instrumentation module

#endif