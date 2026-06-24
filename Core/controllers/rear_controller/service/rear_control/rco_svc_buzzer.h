#ifndef RCO_SVC_BUZZER_H_
#define RCO_SVC_BUZZER_H_

#include <stdint.h>

void rco_svc_buzzer_update(void);
void rco_svc_buzzer_enable(uint32_t duration_ms);

#endif /* RCO_SVC_BUZZER_H_ */
