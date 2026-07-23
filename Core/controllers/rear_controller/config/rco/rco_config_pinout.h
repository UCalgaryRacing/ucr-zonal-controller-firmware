#ifndef REAR_RCO_CONFIG_PINOUT_H_
#define REAR_RCO_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"
#include "stm32h753xx.h"

/*============================================================================*/
/* GPIO Configuration                                                         */
/*============================================================================*/

/* Active LOW input (button connects to GND when pressed) */
#define RCO_RST_BUTTON_GPIO_PORT            GPIOF
#define RCO_RST_BUTTON_GPIO_PIN             GPIO_PIN_14

#define RCO_BSPD_RESET_PORT                 GPIOJ
#define RCO_BSPD_RESET_PIN                  GPIO_PIN_1

extern TIM_HandleTypeDef htim4;

#define RCO_DRS_PWM_TIMER_REGISTER TIM4
#define RCO_DRW_PWM_TIMER (&htim4)

#endif /* REAR_RCO_CONFIG_PINOUT_H_ */
