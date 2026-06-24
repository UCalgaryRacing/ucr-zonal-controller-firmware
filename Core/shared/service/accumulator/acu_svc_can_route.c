#include "can_config.h"
#include "com_typ_common.h"
#include "ucr_03.h"
#include "can_typ_common.h"
#include "can_svc_can.h"
#include "acu_data.h"
#include "acu_svc_can_route.h"


static bool g_initialized = false;

void acu_svc_can_rx_accumulator_data(can_msg_t *msg);
static void acu_svc_can_rx_accumulator_fault_data(can_msg_t *msg);
static void acu_svc_can_rx_accumulator_sdc_data(can_msg_t *msg);

status_t acu_svc_can_route_init(void)
{
    //TODO start CAN here
    com_svc_can_register(ACCUMULATOR_MEASUREMENTS_CAN_ID, acu_svc_can_rx_accumulator_data);
    com_svc_can_register(ACCUMULATOR_FAULT_CAN_ID, acu_svc_can_rx_accumulator_fault_data);
    com_svc_can_register(ACCUMULATOR_SDC_CAN_ID, acu_svc_can_rx_accumulator_sdc_data);
        
    g_initialized = true;
    return OK;
}


void acu_svc_can_rx_accumulator_data(can_msg_t *msg)
{
    //received by front controller
    struct ucr_03_accumulator_measurements_t accumulator_measurements_msg;
    ucr_03_accumulator_measurements_unpack(&accumulator_measurements_msg, msg->data, msg->dlc);

    acu_aculv_data_t aculv_data;  
    
    aculv_data.battery_current = accumulator_measurements_msg.battery_current;
    aculv_data.battery_power = accumulator_measurements_msg.battery_power;
    aculv_data.battery_voltage = accumulator_measurements_msg.battery_voltage;
    aculv_data.ts_active = accumulator_measurements_msg.ts_active;
    aculv_data.ts_voltage = accumulator_measurements_msg.ts_voltage;
    


    acu_data_set_aculv(&aculv_data);
}

// receive IMD and BMS faults from accumulator 
static void acu_svc_can_rx_accumulator_fault_data(can_msg_t *msg)
{
    struct ucr_03_accumulator_fault_t accumulator_fault_msg;
    ucr_03_accumulator_fault_unpack(&accumulator_fault_msg, msg->data, sizeof(msg->data));

    acu_fault_t acu_fault;

    acu_fault.bms_fault = accumulator_fault_msg.bms_fault;
    acu_fault.imd_fault = accumulator_fault_msg.imd_fault;
    acu_fault.last_acu_msg = HAL_GetTick();

    acu_data_set_fault_status(&acu_fault);
}

// receive SDCVoltages data from accumulator
static void acu_svc_can_rx_accumulator_sdc_data(can_msg_t *msg)
{
    struct ucr_03_sdc_voltages_t sdc_voltages_msg;
    ucr_03_sdc_voltages_unpack(&sdc_voltages_msg, msg->data, msg->dlc);

    acu_sdc_voltages_t sdc_voltages;

    sdc_voltages.sdc_mid1 = sdc_voltages_msg.sdc_mid1;
    sdc_voltages.sdc_mid2 = sdc_voltages_msg.sdc_mid2;
    sdc_voltages.sdc7 = sdc_voltages_msg.sdc7;
    sdc_voltages.sdc8 = sdc_voltages_msg.sdc8;
    sdc_voltages.sdc_end = sdc_voltages_msg.sdc_end;
    sdc_voltages.sdc_reserve = sdc_voltages_msg.sdc_reserve;

    acu_data_set_sdc_voltages(&sdc_voltages);
}

void acu_svc_set_acu_fault_timeout()
{
    if (HAL_GetTick() - acu_data_get_last_acu_msg() >= 500)
    {

        acu_fault_t acu_fault;

        acu_fault.bms_fault = true;
        acu_fault.imd_fault = true;
        acu_fault.last_acu_msg = acu_data_get_last_acu_msg();

        acu_data_set_fault_status(&acu_fault);
    }
}


