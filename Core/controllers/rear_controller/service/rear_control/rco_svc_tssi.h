#ifndef RCO_SVC_TSSI_H_
#define RCO_SVC_TSSI_H_

#include "com_typ_common.h"

#define TSSI_RED_FLASH_DURATION_MS 150 // period = 300ms = 3.3Hz

status_t rco_svc_tssi_init(void);
void rco_svc_tssi_update(void);

#endif /* RCO_SVC_TSSI_H_ */
