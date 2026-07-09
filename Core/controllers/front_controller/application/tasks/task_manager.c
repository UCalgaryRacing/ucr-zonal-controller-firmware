#include "task_manager.h"

#include <stdint.h>

#include "tcu_app_state_manager.h"
#include "can_typ_common.h"
#include "can_config.h"
#include "ucr_03.h"
#include "can_svc_can.h"
#include "tcu_svc_logging.h"
#include "tcu_data.h"
#include "acu_data.h"
#include "mco_svc_bamocar.h"
#include "mco_svc_traction_control.h"
#include "mco_svc_power_limit.h"
#include "mco_svc_can_tx.h"
#include "tcu_svc_fault.h"
#include "cmsis_os2.h"
#include "mco_svc_bamocar_regs.h"

//Remove later
#include "ins_data.h"
#include "whl_data.h"


#include "tcu_app_state_manager.h"

static float g_requested_torque = 0;

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_manager_init(void)
{
	nextWakeTime = osKernelGetTickCount();
	tcu_app_state_machine_init();
	mco_svc_power_limit_set_limit_w(60000);
	com_svc_can_init();
}

void task_manager_loop(void)
{
	nextWakeTime += period;



	//---------------- TCU ----------------//
	tcu_svc_fault_check_apps_plausibility();
	tcu_svc_fault_check_apps_bse_plausibility();
	tcu_svc_fault_check_apps_sensor_range();
	tcu_svc_fault_check_bse_sensor_range();

	tcu_app_state_machine_step();


	tcu_svc_can_tx_front_tcu_state();

	//---------------- BAMOCAR ----------------//

	//TODO move all this into proper services 
	g_requested_torque = tcu_data_get_apps_percent();
	bool regen_requested = whl_data_get_button_2();
	float wheel_rpm = ins_data_get_wheel_rpm(INS_WHEEL_SPEED_SENSOR_FRONT_LEFT);
	bool regen_active = (regen_requested && (wheel_rpm > 100.0f));
	//tcu_bse_data_t bse_data;
    //tcu_data_get_bse(&bse_data);

	//#define REGEN_TORQUE_SCALING_FACTOR       10.0f

	if (regen_active)
	{
		g_requested_torque = -10.0f;
		//g_requested_torque = bse_data.sensor.percent * REGEN_TORQUE_SCALING_FACTOR / 100.0f;

	}

	if (tcu_data_get_fault_active())
	{
		g_requested_torque = 0.0f;
	}
	if (acu_data_get_sdc_reserve_voltage() < ACU_SDC_RESERVE_MIN_VOLTAGE_V)
	{
		g_requested_torque = 0.0f;
	}
	if (-1.0f < g_requested_torque && g_requested_torque < 3.0f)
	{
		g_requested_torque = 0.0f;
	}

	if (g_requested_torque > 100.0f)
	{
		g_requested_torque = 100.0f;
	}
	if (tcu_app_state_machine_get_state() != TCU_APP_STATE_READY_TO_DRIVE)
	{
		g_requested_torque = 0.0f;
	}

	bool traction_control_active = false;
	bool power_limit_active = false;

	if (!regen_active)
	{
	//---------------- TRACTION CONTROL ----------------//
	// Enable exactly ONE method below before reflashing (comment out the other).
	mco_svc_traction_control_calculate_motor_speed_slip_ratio();

//	  float torque_before_tc = g_requested_torque;
//	  g_requested_torque = mco_svc_traction_control_limit_torque_percent(g_requested_torque);
//	  traction_control_active = (g_requested_torque < (torque_before_tc - 0.01f));


	//---------------- POWER LIMIT ----------------//
	float torque_before_pl = g_requested_torque;
	g_requested_torque = mco_svc_power_limit_limit_torque_percent(g_requested_torque);
	power_limit_active = (g_requested_torque < (torque_before_pl - 0.01f));
	}


	mco_svc_can_tx_motor_request_data(g_requested_torque, power_limit_active, traction_control_active);



	mco_svc_bamocar_set_torque_percent(g_requested_torque);





	osDelayUntil(nextWakeTime);
}
