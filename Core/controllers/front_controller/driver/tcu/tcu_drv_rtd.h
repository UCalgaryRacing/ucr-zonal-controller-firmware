#ifndef DRIVER_TCU_DRV_RTD_H_
#define DRIVER_TCU_DRV_RTD_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"
#include "mcu_config.h"
#include "mcu_config_pinout.h"

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t tcu_drv_rtd_init(void);

/*============================================================================*/
/* Button State                                                               */
/*============================================================================*/

bool tcu_drv_rtd_is_button_pressed(void);

#endif /* DRIVER_TCU_DRV_RTD_H_ */
