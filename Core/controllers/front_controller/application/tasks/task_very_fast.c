#include "task_very_fast.h"
#include "cmsis_os2.h"

#include "tcu_data.h"
#include "mcu_svc_analog.h"
#include "tcu_svc_pedals.h"
#include "tcu_svc_fault.h"
#include "fco_svc_rtd.h"
#include "tcu_svc_can_route.h"
#include "tcu_svc_logging.h"
#include "fco_svc_dash_lights.h"
#include "fco_svc_ptt.h"

#include "acu_svc_can_route.h"

#include "pdm_svc_channel.h"
#include "pdm_config.h"

#include "acu_data.h"

#include "rco_data.h"
#include "rco_svc_can_route.h"

#include "mco_svc_bamocar.h"
#include "mco_config.h"

#include "whl_data.h"
#include "whl_svc_can_route.h"

#include "prm_svc_channel.h"
#include "prm_channel_types.h"

#include "ins_svc_ads124s08.h"
#include "ins_svc_pot.h"
#include "ins_svc_logging.h"
#include "ins_svc_wheel_speed.h"

#include "task_timing_svc.h"

#include "gps_svc.h"

static const uint32_t period = 10;
static uint32_t nextWakeTime;



void task_very_fast_init(void)
{
	nextWakeTime = osKernelGetTickCount();

	// //---------------- GPS ----------------//
	gps_svc_init();
	gps_svc_start();

	//---------------- INSTRUMENTATION :() ----------------//
	//ins_svc_ads124s08_init();
}

void task_very_fast_loop(void)
{
	nextWakeTime += period;

	tcu_svc_apps_update(APPS1, APPS2);
	//tcu_svc_can_tx_apps_data();

	//ins_svc_wheel_speed_update(FL_WHEEL_SPEED);
	//ins_svc_wheel_speed_update(FR_WHEEL_SPEED);
	//ins_svc_can_tx_front_wheel_speed_data();

	//tcu_svc_can_tx_motor_data();
	// //---------------- GPS ----------------//
	//gps_svc_update();

	osDelayUntil(nextWakeTime);
}
