
#include "rco_svc_glv.h"
#include "can_svc_can.h"
#include "pdm_data.h"


void rco_svc_can_tx_glv_data(void){
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = GLV_STATUS_CAN_ID;
    msg.dlc = 32;

    pdm_full_system_data_t pdm_data;
    pdm_data_get_all(&pdm_data);
    pdm_glv_data_t glv_data = pdm_data.glv;
    struct ucr_03_glv_battery_t glv_status_data;

    glv_status_data.glv_voltage = glv_data.measurements.voltage.voltage_v;
    glv_status_data.glv_current = glv_data.measurements.current.current_a;
    glv_status_data.glv_lowest_cell_voltage = glv_data.lowest_cell_voltage_v;
    glv_status_data.glv_highest_cell_temp = glv_data.highest_cell_temperature_c;
    glv_status_data.glv_so_c = glv_data.soc_percent;
    glv_status_data.glv_power = glv_data.measurements.power.power_w;
    glv_status_data.glv_under_voltage_fault = glv_data.undervoltage_fault;
    glv_status_data.glv_over_temp_fault = glv_data.overtemperature_fault;

    ucr_03_glv_battery_pack(msg.data, &glv_status_data, msg.dlc);

    com_svc_can_transmit(&msg);

}

