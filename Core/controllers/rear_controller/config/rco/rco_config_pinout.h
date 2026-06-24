#ifndef REAR_RCO_CONFIG_PINOUT_H_
#define REAR_RCO_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"
#include "stm32h753xx.h"

/*============================================================================*/
/* GPIO Configuration                                                         */
/*============================================================================*/

/* Active LOW input (button connects to GND when pressed) */
#define RCO_RST_BUTTON_GPIO_PORT            GPIOA
#define RCO_RST_BUTTON_GPIO_PIN             GPIO_PIN_5

#define RCO_BSPD_RESET_PORT                     GPIOA
#define RCO_BSPD_RESET_PIN                      GPIO_PIN_6

extern TIM_HandleTypeDef htim8;

#define RCO_DRS_PWM_TIMER_REGISTER TIM8
#define RCO_DRW_PWM_TIMER (&htim8)

#endif /* REAR_RCO_CONFIG_PINOUT_H_ */
