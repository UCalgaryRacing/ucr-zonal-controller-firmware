#ifndef DRIVER_RCO_DRV_RESET_H_
#define DRIVER_RCO_DRV_RESET_H_

#include "com_typ_common.h"
#include "rco_config_pinout.h"

// initialization
status_t rco_drv_reset_init(void);

bool rco_drv_reset_is_button_pressed(void);

void rco_drv_broadcast_reset_button(bool reset_state);

#endif /* DRIVER_RCO_DRV_RESET_H_ */