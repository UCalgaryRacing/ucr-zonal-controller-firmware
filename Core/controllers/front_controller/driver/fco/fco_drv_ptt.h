#ifndef DRIVER_FCO_DRV_PTT_H_
#define DRIVER_FCO_DRV_PTT_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"
#include "mcu_config.h"
#include "mcu_config_pinout.h"

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t fco_drv_ptt_init(void);

/*============================================================================*/
/* Set PTT On                                                                 */
/*============================================================================*/

void fco_drv_ptt_set_on(bool on);

#endif /* DRIVER_FCO_DRV_PTT_H_ */