#include "task_very_fast.h"
#include "cmsis_os2.h"

/* The rear controller has no 100 Hz workload (GPS is front-only), so this
 * task is an empty placeholder that exists solely to satisfy the shared
 * main.c, which starts VeryFastTask for both build configurations. */

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_very_fast_init(void)
{
	nextWakeTime = osKernelGetTickCount();
}

void task_very_fast_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);
}
