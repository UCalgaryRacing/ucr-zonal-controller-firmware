#include "task_medium.h"
#include "rco_svc_brake_light.h"
#include "rco_svc_buzzer.h"
#include "cmsis_os2.h"


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
