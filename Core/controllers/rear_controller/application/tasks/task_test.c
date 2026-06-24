#include "task_test.h"
#include "cmsis_os2.h"
#include "task_test.h"
#include "cmsis_os2.h"
#include "can_svc_can.h"
#include "acu_data.h"

static const uint32_t period = 5000;
static uint32_t nextWakeTime;

// static uint8_t brake_test = 0;
// static float brake_psi = 0;

// static uint8_t state_timer = 0;



// static uint8_t bms_fault_test = 1;
// static uint8_t imd_fault_test = 1;

void task_test_init(void)
{
	nextWakeTime = osKernelGetTickCount();
}

void task_test_loop(void)
{
	nextWakeTime += period;
	osDelayUntil(nextWakeTime);

//	brake_test = !brake_test;
//	brake_psi++;
//	state_timer++;
//	if (state_timer > 4) state_timer = 0;
//
//
//	can_msg_t msg;
//    msg.channel = CAN1;
//	msg.id = FRONT_BSE_CAN_ID;
//	msg.dlc = 5;
//
//    struct ucr_03_front_bse_t front_bse_data;
//    front_bse_data.braking_active = brake_test;
//    front_bse_data.hard_braking_active = brake_test;
//    front_bse_data.front_bse_pressure_psi = brake_psi;
//    ucr_03_front_bse_pack(msg.data, &front_bse_data, msg.dlc);
//
//    com_svc_can_transmit(&msg);

//
//
//    msg.channel = CAN1;
//	msg.id = FRONT_CONTROLLER_STATE_CAN_ID;
//	msg.dlc = 1;
//
//    struct ucr_03_front_controller_state_t front_controller_state;
//
//	front_controller_state.state = state_timer;
//    ucr_03_front_controller_state_pack(msg.data, &front_controller_state, msg.dlc);
//
//    com_svc_can_transmit(&msg);
//
//
//	bms_fault_test = !bms_fault_test;
//	imd_fault_test = !imd_fault_test;
//
//	msg.channel = CAN1;
//	msg.id = ACCUMULATOR_FAULT_CAN_ID;
//	msg.dlc = 1;
//
//   struct ucr_03_accumulator_fault_t accumulator_fault_msg;
//
//	accumulator_fault_msg.bms_fault = bms_fault_test;
//	accumulator_fault_msg.imd_fault = imd_fault_test;
//	ucr_03_accumulator_fault_pack(msg.data, &accumulator_fault_msg, msg.dlc);
//
//   com_svc_can_transmit(&msg);
}
