#include "task_test.h"
#include "cmsis_os2.h"
#include "can_svc_can.h"
#include "acu_data.h"

// static uint8_t bms_fault_test = 1;
// static uint8_t imd_fault_test = 1;

static const uint32_t period = 5000;
static uint32_t nextWakeTime;

void task_test_init(void)
{
	nextWakeTime = osKernelGetTickCount();
}

void task_test_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);

}
