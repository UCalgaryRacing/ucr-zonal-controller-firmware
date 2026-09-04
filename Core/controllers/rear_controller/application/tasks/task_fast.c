#include "task_fast.h"
#include "cmsis_os2.h"

#include "tcu_data.h"
#include "tcu_svc_can_route.h"
#include "mcu_config.h"
#include "mcu_svc_analog.h"
#include "glv_svc_status.h"

#include "pdm_svc_channel.h"
#include "pdm_config.h"

#include "prm_svc_channel.h"
#include "prm_channel_types.h"

#include "acu_svc_can_route.h"

#include "rco_data.h"
#include "rco_svc_logging.h"

#include "rco_svc_brake_light.h"
#include "rco_svc_buzzer.h"
#include "rco_svc_tssi.h"
#include "rco_svc_reset_button.h"
#include "rco_svc_drs.h"
#include "rco_svc_glv.h"
#include "rco_svc_fan.h"

#include "rco_svc_bamocar.h"

#include "acu_data.h"

#include "mcu_svc_hsd.h"
#include "mcu_config.h"

#include "whl_svc_can_route.h"
#include "whl_data.h"

#include "ins_svc_can_route.h"
#include "ins_svc_logging.h"
#include "ins_svc_pot.h"
#include "ins_svc_ads124s08.h"

static const uint32_t period = 10;
static uint32_t nextWakeTime;

void task_fast_init(void)
{
	nextWakeTime = osKernelGetTickCount();

	//---------------- PDM ----------------//
	//CRITICAL

	//BAMOCAR
	 pdm_svc_channel_init(BRAKE_LIGHT);
	 pdm_svc_channel_enable(BRAKE_LIGHT, true);

	 pdm_svc_channel_init(TSSI_RED);
	 pdm_svc_channel_enable(TSSI_RED, false);

	 pdm_svc_channel_init(TSSI_GREEN);
	 pdm_svc_channel_enable(TSSI_GREEN, false);

	 pdm_svc_channel_init(RTD_BUZZER);
	 pdm_svc_channel_enable(RTD_BUZZER, false);

	pdm_svc_channel_init(FRONT_CONTROLLER);
	pdm_svc_channel_enable(FRONT_CONTROLLER, true);
	pdm_svc_channel_set_duty(FRONT_CONTROLLER, 100);

	//BRAKE_LIGHT
	pdm_svc_channel_init(VBATT_SW_2);
	pdm_svc_channel_enable(VBATT_SW_2, true);
	pdm_svc_channel_set_duty(VBATT_SW_2, 100);


	pdm_svc_channel_init(ACULV);
	pdm_svc_channel_enable(ACULV, true);

	pdm_svc_channel_init(IMD);
	pdm_svc_channel_enable(IMD, true);

	pdm_svc_channel_enable(TRACTIVE_PUMP_1, false);
	pdm_svc_channel_set_duty(TRACTIVE_PUMP_1, 100);
	pdm_svc_channel_init(TRACTIVE_PUMP_1);

	pdm_svc_channel_enable(TRACTIVE_PUMP_2, false);
	pdm_svc_channel_set_duty(TRACTIVE_PUMP_2, 100);
	pdm_svc_channel_init(TRACTIVE_PUMP_2);

	//DRS / ACCUMULATOR FAN 1
	pdm_svc_channel_init(VBATT_SW_7);
	pdm_svc_channel_enable(VBATT_SW_7, true);
	pdm_svc_channel_set_duty(VBATT_SW_7, 100);

	pdm_svc_channel_init(ACCUMULATOR_FAN_1);
	pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
	pdm_svc_channel_set_duty(ACCUMULATOR_FAN_1, 100);


	//BAMOCAR
	pdm_svc_channel_init(ACCUMULATOR_FAN_2);
	pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
	pdm_svc_channel_set_duty(ACCUMULATOR_FAN_2, 100);


	pdm_svc_channel_init(TRACTIVE_FAN_1);
	pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
	pdm_svc_channel_set_duty(TRACTIVE_FAN_1, 100);

	pdm_svc_channel_init(TRACTIVE_FAN_2);
	pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
	pdm_svc_channel_set_duty(TRACTIVE_FAN_2, 100);

	// from MCU V1.0 pins tied high
	pdm_svc_channel_init(VBATT_SW_10);
	pdm_svc_channel_enable(VBATT_SW_10, true);

	pdm_svc_channel_init(VBATT_SW_11);
	pdm_svc_channel_enable(VBATT_SW_11, true);

	pdm_svc_channel_init(VBATT_SW_12);
	pdm_svc_channel_enable(VBATT_SW_12, true);

	pdm_svc_channel_init(VBATT_REG_SW_1);
	pdm_svc_channel_enable(VBATT_REG_SW_1, true);

	pdm_svc_channel_init(VBATT_REG_SW_2);
	pdm_svc_channel_enable(VBATT_REG_SW_2, true);

	pdm_svc_channel_init(VBATT_REG_SW_3);
	pdm_svc_channel_enable(VBATT_REG_SW_3, true);

	//---------------- PRM ----------------//
	prm_svc_channel_init(INTERNAL_3V3_2);
	prm_svc_channel_init(INTERNAL_12V);
	prm_svc_channel_init(INTERNAL_5V_2);

	prm_svc_channel_init(EXTERNAL_12V_1);
	prm_svc_channel_init(EXTERNAL_12V_2);
	prm_svc_channel_init(EXTERNAL_5V_1);
	prm_svc_channel_init(EXTERNAL_5V_2);

	//---------------- MCU ----------------//
	mcu_svc_hsd_channel_enable(BAMOCAR_RFE, true);

	mcu_svc_hsd_channel_enable(TS_FAN_PWM, true);



	//---------------- TCU ----------------//
	mcu_svc_analog_init(BATT_HIGHEST_TEMP);
	mcu_svc_analog_init(BATT_LOWEST_VOLTAGE);

	mcu_svc_analog_start(BATT_HIGHEST_TEMP);
	mcu_svc_analog_start(BATT_LOWEST_VOLTAGE);
	HAL_Delay(5); // small delay to ensure DMA can get the ADC readings correctly.
	pdm_svc_glv_readings_init();
	glv_svc_init_soc();


	tcu_data_init();

	tcu_svc_can_route_init();


	//---------------- ACCUMULATOR ----------------//
	acu_data_init();






	//---------------- ACU ----------------//
	 acu_svc_can_route_init();


	//---------------- REAR CONTROL  ----------------//
	rco_data_init();
	rco_svc_tssi_init();
	rco_svc_reset_init();
	rco_svc_drs_init();

	//---------------- WHEEL ----------------//
	whl_data_init();
	whl_svc_can_route_init();

	//---------------- Instrumentation ----------------//
	ins_svc_ads124s08_init();
	ins_svc_can_route_init();


	// FDCAN_FilterTypeDef sFilterConfig0;

    // sFilterConfig0.IdType = FDCAN_STANDARD_ID;
    // sFilterConfig0.FilterIndex = 0;
    // sFilterConfig0.FilterType = FDCAN_FILTER_MASK;
    // sFilterConfig0.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    // sFilterConfig0.FilterID1 = 200;
    // sFilterConfig0.FilterID2 = 0x7FF;
    // sFilterConfig0.RxBufferIndex = 0;
    // HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig0);

    // FDCAN_FilterTypeDef sFilterConfig1;

    // sFilterConfig1.IdType = FDCAN_STANDARD_ID;
    // sFilterConfig1.FilterIndex = 1;
    // sFilterConfig1.FilterType = FDCAN_FILTER_MASK;
    // sFilterConfig1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    // sFilterConfig1.FilterID1 = 203;
    // sFilterConfig1.FilterID2 = 0x7FF;
    // sFilterConfig1.RxBufferIndex = 0;
    // HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig1);

    // HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

}

void task_fast_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);

	//---------------- PDM ----------------//
	//only for updating vnf watchdog for now
	//TODO is there a better way to do this?
	pdm_svc_channel_update(BRAKE_LIGHT);
	pdm_svc_channel_update(TSSI_RED);
	pdm_svc_channel_update(TSSI_GREEN);
	pdm_svc_channel_update(RTD_BUZZER);
	pdm_svc_channel_update(ACULV);
	pdm_svc_channel_update(FRONT_CONTROLLER);
	pdm_svc_channel_update(IMD);
	pdm_svc_channel_update(TRACTIVE_FAN_1);
	pdm_svc_channel_update(ACCUMULATOR_FAN_2);
	pdm_svc_channel_update(ACCUMULATOR_FAN_1);
	pdm_svc_channel_update(TRACTIVE_FAN_2);
	pdm_svc_channel_update(TRACTIVE_FAN_1);
	pdm_svc_channel_update(VBATT_SW_7);
	pdm_svc_channel_update(TRACTIVE_PUMP_2);


	//---------------- REAR CONTROL ----------------//
	rco_svc_brake_light_update();
	rco_svc_buzzer_update();
	rco_svc_tssi_update();
	rco_svc_reset_button_update();
	rco_svc_can_tx_rco_data();
	rco_svc_update_drs();
	rco_svc_can_tx_rco_cooling_data();

	rco_svc_fan_update();


	//---------------- INSTRUMENTATION ----------------//
	ins_svc_update_pots(RL_SUSPENSION, RR_SUSPENSION);
	ins_svc_can_tx_rear_suspension_data();

	// ins_svc_can_tx_rear_wheel_speed_data(); //TODO: rear wheel speed not implemented yet

	//---------------- ACCUMULATOR ----------------//
	//acu_svc_set_acu_fault_timeout();



	//---------------- GLV ----------------//
	glv_svc_update_glv_status();
	pdm_svc_update_glv_data();
	glv_svc_update_soc();
	rco_svc_can_tx_glv_data();
	glv_svc_apply_glv_plausability();



	
}
