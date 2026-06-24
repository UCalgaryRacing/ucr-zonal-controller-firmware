#ifndef RCO_DRV_DRS_H
#define RCO_DRV_DRS_H

#include <stdint.h>



#define DRS_MAX_TIMER_VALUE 65535U
void rco_drv_drs_set_duty(float duty_cycle);
void rco_drv_drs_init();
#endif //RCO_DRV_DRS_H