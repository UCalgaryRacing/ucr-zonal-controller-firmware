#ifndef CONFIG_GPS_CONFIG_PINOUT_H_
#define CONFIG_GPS_CONFIG_PINOUT_H_

#include "stm32h7xx_hal.h"

/*============================================================================*/
/* UART3 Interface - Novatel OEM7600                                          */
/*============================================================================*/

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef  hdma_usart3_rx;

#define GPS_UART_HANDLE  (&huart3)
#define GPS_DMA_HANDLE   (&hdma_usart3_rx)

/*============================================================================*/
/* OEM7600 nRESET (active-low, driven by MCU GPIO)                            */
/*============================================================================*/

/* The OEM7600's nRESET line is wired to PA6.  CubeMX initialises this pin as
 * a push-pull output with ODR=0, holding the receiver in reset; the driver
 * releases it (drives HIGH) during init so the receiver boots and begins
 * accepting commands on UART3.  (The EPSON IMU's own nRESET is pulled up on
 * the board and is not under MCU control.) */
#define GPS_OEM7_NRESET_PORT  GPIOJ
#define GPS_OEM7_NRESET_PIN   GPIO_PIN_1

#endif /* CONFIG_GPS_CONFIG_PINOUT_H_ */
