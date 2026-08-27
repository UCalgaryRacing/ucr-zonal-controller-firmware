#ifndef CONFIG_INS_CONFIG_PINOUT_H_
#define CONFIG_INS_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

#define INS_FL_WHEEL_TIMER_HANDLE           (&htim5)
#define INS_FL_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1

#define INS_FR_WHEEL_TIMER_HANDLE           (&htim2)
#define INS_FR_WHEEL_TIMER_CHANNEL          TIM_CHANNEL_1

#endif
