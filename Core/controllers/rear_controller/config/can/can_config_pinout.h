#ifndef CONFIG_CAN_CONFIG_PINOUT_H_
#define CONFIG_CAN_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

/*============================================================================*/
/* CAN Interface                                                              */
/*============================================================================*/

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

#define CAN_1_HANDLE (&hfdcan1)
#define CAN_2_HANDLE (&hfdcan2)

#endif /* CONFIG_CAN_CONFIG_PINOUT_H_ */
