#include "task_medium.h"
#include "cmsis_os2.h"

#include "tcu_svc_pedals.h"

#include "ins_svc_logging.h"
#include "ins_svc_pot.h"
#include "ins_svc_ads124s08.h"


static const uint32_t period = 50;
static uint32_t nextWakeTime;

void task_medium_init(void)
{
	nextWakeTime = osKernelGetTickCount();
	//---------------- Instrumentation ----------------//
	ins_svc_ads124s08_init();
}

void task_medium_loop(void)
{
	nextWakeTime += period;

	//---------------- INSTRUMENTATION ----------------//
	//	ins_svc_update_front_pots(FL_SUSPENSION, FR_SUSPENSION);
	ins_svc_update_ads124s08();

	osDelayUntil(nextWakeTime);
}
