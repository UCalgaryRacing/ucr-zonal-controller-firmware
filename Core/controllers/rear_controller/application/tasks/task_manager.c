#include "task_manager.h"
#include "tcu_app_state_manager.h"
#include "can_typ_common.h"
#include "can_config.h"
#include "ucr_03.h"
#include "can_svc_can.h"
#include "cmsis_os2.h"

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_manager_init(void)
{
	nextWakeTime = osKernelGetTickCount();
	tcu_app_state_machine_init();
	com_svc_can_init();
}

void task_manager_loop(void)
{
	nextWakeTime += period;

	tcu_app_state_machine_step();

	osDelayUntil(nextWakeTime);
}
