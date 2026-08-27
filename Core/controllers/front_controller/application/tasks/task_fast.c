#include "task_fast.h"
#include "cmsis_os2.h"

#include "tcu_data.h"
#include "mcu_svc_analog.h"
#include "tcu_svc_pedals.h"
#include "tcu_svc_fault.h"
#include "fco_svc_rtd.h"
#include "tcu_svc_can_route.h"
#include "tcu_svc_logging.h"
#include "fco_svc_dash_lights.h"

#include "ins_svc_logging.h"
#include "ins_svc_wheel_speed.h"
#include "ins_data.h"
#include "ins_svc_pot.h"

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

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_fast_init(void)
{
	nextWakeTime = osKernelGetTickCount();

	//---------------- PDM ----------------//
	//CRITICAL
	pdm_svc_channel_init(SDC_START);
	pdm_svc_channel_enable(SDC_START, true);
	pdm_svc_channel_set_duty(SDC_START, 100);

	// energ meter power
	pdm_svc_channel_init(VBATT_SW_2);
	pdm_svc_channel_enable(VBATT_SW_2, true);
	pdm_svc_channel_set_duty(VBATT_SW_2, 100);

	pdm_svc_channel_init(IMD_LIGHT);
	pdm_svc_channel_enable(IMD_LIGHT, false);

	pdm_svc_channel_init(BMS_LIGHT);
	pdm_svc_channel_enable(BMS_LIGHT, false);

	pdm_svc_channel_init(RTD_LIGHT);
	pdm_svc_channel_enable(RTD_LIGHT, false);


	//---------------- TCU ----------------//

	tcu_svc_can_route_init();

	mcu_svc_analog_init(APPS1);
	mcu_svc_analog_init(APPS2);
	mcu_svc_analog_init(BSE1);

	mcu_svc_analog_start(APPS1);
	mcu_svc_analog_start(APPS2);
	mcu_svc_analog_start(BSE1);

	fco_svc_ptt_init();	
	fco_svc_dash_lights_init();

	ins_svc_wheel_speed_init();

	//---------------- ACCUMULATOR ----------------//
	acu_svc_can_route_init();
	acu_data_init();

	//---------------- BAMOCAR ----------------//
	
	mco_svc_bamocar_init();
	mco_config_apply(&mco_default_config);
	mco_svc_bamocar_request_data(BAMOCAR_REG_INVERTER_TEMP, BAMOCAR_REPEAT_100MS);
	mco_svc_bamocar_request_data(BAMOCAR_REG_MOTOR_TEMP, BAMOCAR_REPEAT_100MS);
	mco_svc_bamocar_request_data(BAMOCAR_REG_RPM_ACTUAL, BAMOCAR_REPEAT_10MS);
	mco_svc_bamocar_request_data(BAMOCAR_REG_ACTIVE_CURRENT, BAMOCAR_REPEAT_10MS);
	mco_svc_bamocar_request_data(BAMOCAR_REG_REACTIVE_CURRENT, BAMOCAR_REPEAT_10MS);
	mco_svc_bamocar_request_data(BAMOCAR_REG_AC_CURRENT, BAMOCAR_REPEAT_10MS);
	mco_svc_bamocar_request_data(BAMOCAR_STATUS_INFO, BAMOCAR_REPEAT_100MS);
	mco_svc_bamocar_request_data(BAMOCAR_ERROR_INFO, BAMOCAR_REPEAT_100MS);

	//---------------- RCO ----------------//
	rco_svc_can_route_init();

	//---------------- Wheel ----------------//
	whl_data_init();
	whl_svc_can_route_init();

	//---------------- Instrumentation ----------------//
	mcu_svc_analog_init(FL_SUSP);
	mcu_svc_analog_init(FR_SUSP);

	mcu_svc_analog_start(FL_SUSP);
	mcu_svc_analog_start(FR_SUSP);

}

void task_fast_loop(void)
{
	nextWakeTime += period;


	//---------------- PDM ----------------//
	//only for updating vnf watchdog for now
	//TODO is there a better way to do this?
	pdm_svc_channel_update(SDC_START);
	pdm_svc_channel_update(IMD_LIGHT);
	pdm_svc_channel_update(BMS_LIGHT);
	pdm_svc_channel_update(RTD_LIGHT);

	//---------------- TCU ----------------//
	tcu_svc_apps_update(APPS1, APPS2);
	tcu_svc_bse_update(BSE1);

	fco_svc_ptt_update();
	fco_svc_rtd_button_update();

	tcu_svc_can_tx_bse_data();
	tcu_svc_can_tx_apps_data();
	tcu_svc_can_tx_tcu_faults_data();
	tcu_svc_can_tx_motor_data();
	tcu_svc_can_tx_motor_status_data();

	fco_svc_dash_lights_update();

	//---------------- INSTRUMENTATION ----------------//
	ins_svc_wheel_speed_update(FL_WHEEL_SPEED);
	ins_svc_wheel_speed_update(FR_WHEEL_SPEED);

	ins_svc_front_pots_update(FL_SUSP, FR_SUSP);

	ins_svc_can_tx_front_suspension_data();
	ins_svc_can_tx_front_wheel_speed_data();


	//---------------- ACCUMULATOR ----------------//
	//acu_svc_set_acu_fault_timeout();


	osDelayUntil(nextWakeTime);
}
