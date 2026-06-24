#include "task_very_fast.h"
#include "cmsis_os2.h"

#include "gps_svc.h"

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_very_fast_init(void)
{
	nextWakeTime = osKernelGetTickCount();

	//---------------- GPS ----------------//
	gps_svc_init();
	gps_svc_start();
}

void task_very_fast_loop(void)
{
	nextWakeTime += period;

	//---------------- GPS ----------------//
	gps_svc_update();

	osDelayUntil(nextWakeTime);
}
