#include "task_medium.h"
#include "cmsis_os2.h"

#include "tcu_svc_pedals.h"


static const uint32_t period = 50;
static uint32_t nextWakeTime;

void task_medium_init(void)
{
	nextWakeTime = osKernelGetTickCount();
}

void task_medium_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);
}
