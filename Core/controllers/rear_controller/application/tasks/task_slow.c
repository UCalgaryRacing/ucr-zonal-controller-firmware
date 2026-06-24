#include "task_slow.h"
#include "cmsis_os2.h"

// #include "tcu_svc_logging.h"
// #include "pdm_svc_logging.h"

static const uint32_t period = 100;
static uint32_t nextWakeTime;

void task_slow_init(void)
{
	nextWakeTime = osKernelGetTickCount();
}

void task_slow_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);	
}
