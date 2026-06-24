#include "can_svc_can.h"
#include "can_config.h"
#include "tcu_data.h"

//TODO should this be called logging or can tx or something similar?

void tcu_svc_can_tx_apps_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = APPS_CAN_ID;
    msg.dlc = UCR_03_APPS_LENGTH;

    tcu_apps_data_t apps_data;
    tcu_data_get_apps(&apps_data);

    struct ucr_03_apps_t apps_can_msg;
    apps_can_msg.apps1_percent_travel = apps_data.sensor1.percent;
    apps_can_msg.apps2_percent_travel = apps_data.sensor2.percent;
    apps_can_msg.torque_percent = apps_data.pedal_percent;
    apps_can_msg.apps_fault = (uint8_t)(apps_data.sensor1.validity | apps_data.sensor2.validity);
    ucr_03_apps_pack(msg.data, &apps_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void tcu_svc_can_tx_bse_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = FRONT_BSE_CAN_ID;
    msg.dlc = UCR_03_FRONT_BSE_LENGTH;

    tcu_bse_data_t bse_data;
    tcu_data_get_bse(&bse_data);

    struct ucr_03_front_bse_t front_bse_data_can_msg;
    front_bse_data_can_msg.braking_active = (uint8_t)bse_data.is_braking;
    front_bse_data_can_msg.hard_braking_active = (uint8_t)bse_data.is_hard_braking;
    front_bse_data_can_msg.front_bse_pressure_psi = bse_data.pressure_psi;
    ucr_03_front_bse_pack(msg.data, &front_bse_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void tcu_svc_can_tx_motor_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = MOTOR_DATA_CAN_ID;
    msg.dlc = UCR_03_MOTOR_DATA_LENGTH;

    tcu_motor_data_t motor_data;
    tcu_data_get_motor(&motor_data);

    struct ucr_03_motor_data_t motor_data_can_msg;
    motor_data_can_msg.motor_rpm = motor_data.rpm;
    motor_data_can_msg.active_current = motor_data.active_current_a;
    motor_data_can_msg.reactive_current = motor_data.reactive_current_a;
    motor_data_can_msg.motor_temp_c = motor_data.motor_temp_c;
    motor_data_can_msg.inverter_temp_c = motor_data.inverter_temp_c;
    motor_data_can_msg.ac_current = motor_data.ac_current_a;
    ucr_03_motor_data_pack(msg.data, &motor_data_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void tcu_svc_can_tx_motor_status_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = MOTOR_STATUS_CAN_ID;
    msg.dlc = UCR_03_MOTOR_STATUS_LENGTH;

    tcu_motor_status_t motor_status;
    tcu_data_get_motor_status(&motor_status);

    struct ucr_03_motor_status_t motor_status_can_msg;
    motor_status_can_msg.ena = (uint8_t)motor_status.status.ena;
    motor_status_can_msg.nc_r0 = (uint8_t)motor_status.status.nc_r0;
    motor_status_can_msg.lim_plus = (uint8_t)motor_status.status.lim_plus;
    motor_status_can_msg.lim_minus = (uint8_t)motor_status.status.lim_minus;
    motor_status_can_msg.ok = (uint8_t)motor_status.status.ok;
    motor_status_can_msg.icns = (uint8_t)motor_status.status.icns;
    motor_status_can_msg.t_nlim = (uint8_t)motor_status.status.t_nlim;
    motor_status_can_msg.pn = (uint8_t)motor_status.status.pn;
    motor_status_can_msg.ni = (uint8_t)motor_status.status.ni;
    motor_status_can_msg.n0 = (uint8_t)motor_status.status.n0;
    motor_status_can_msg.rsw = (uint8_t)motor_status.status.rsw;
    motor_status_can_msg.cal0 = (uint8_t)motor_status.status.cal0;
    motor_status_can_msg.cal = (uint8_t)motor_status.status.cal;
    motor_status_can_msg.tol = (uint8_t)motor_status.status.tol;
    motor_status_can_msg.rdy = (uint8_t)motor_status.status.rdy;
    motor_status_can_msg.brk0 = (uint8_t)motor_status.status.brk0;
    motor_status_can_msg.sign_mag = (uint8_t)motor_status.status.sign_mag;
    motor_status_can_msg.nclip = (uint8_t)motor_status.status.nclip;
    motor_status_can_msg.nclip_plus = (uint8_t)motor_status.status.nclip_plus;
    motor_status_can_msg.nclip_minus = (uint8_t)motor_status.status.nclip_minus;
    motor_status_can_msg.lrd_dig = (uint8_t)motor_status.status.lrd_dig;
    motor_status_can_msg.luse_rchd = (uint8_t)motor_status.status.luse_rchd;
    motor_status_can_msg.lrd_n = (uint8_t)motor_status.status.lrd_n;
    motor_status_can_msg.ird_ti = (uint8_t)motor_status.status.ird_ti;
    motor_status_can_msg.ird_tir = (uint8_t)motor_status.status.ird_tir;
    motor_status_can_msg.ird10_hz = (uint8_t)motor_status.status.ird10_hz;
    motor_status_can_msg.ird_tm = (uint8_t)motor_status.status.ird_tm;
    motor_status_can_msg.ird_ana = (uint8_t)motor_status.status.ird_ana;
    motor_status_can_msg.lwcns = (uint8_t)motor_status.status.lwcns;
    motor_status_can_msg.rfe_pulse = (uint8_t)motor_status.status.rfe_pulse;
    motor_status_can_msg.fiwe = (uint8_t)motor_status.status.fiwe;
    motor_status_can_msg.hnd_whl = (uint8_t)motor_status.status.hnd_whl;
    motor_status_can_msg.bad_paras = (uint8_t)motor_status.error.bad_paras;
    motor_status_can_msg.power_fault = (uint8_t)motor_status.error.power_fault;
    motor_status_can_msg.rfe_fault = (uint8_t)motor_status.error.rfe_fault;
    motor_status_can_msg.bus_timeout = (uint8_t)motor_status.error.bus_timeout;
    motor_status_can_msg.feedback = (uint8_t)motor_status.error.feedback;
    motor_status_can_msg.power_voltage = (uint8_t)motor_status.error.power_voltage;
    motor_status_can_msg.motor_temp_fault = (uint8_t)motor_status.error.motor_temp_fault;
    motor_status_can_msg.device_temp_fault = (uint8_t)motor_status.error.device_temp_fault;
    motor_status_can_msg.over_voltage = (uint8_t)motor_status.error.over_voltage;
    motor_status_can_msg.i_peak_fault = (uint8_t)motor_status.error.i_peak_fault;
    motor_status_can_msg.raceaway = (uint8_t)motor_status.error.raceaway;
    motor_status_can_msg.user = (uint8_t)motor_status.error.user;
    motor_status_can_msg.hw_err = (uint8_t)motor_status.error.hw_err;
    motor_status_can_msg.ballast_fault = (uint8_t)motor_status.error.ballast_fault;
    motor_status_can_msg.warning0 = (uint8_t)motor_status.error.warning0;
    motor_status_can_msg.illegal_status = (uint8_t)motor_status.error.illegal_status;
    motor_status_can_msg.safe_in = (uint8_t)motor_status.error.safe_in;
    motor_status_can_msg.motor_temp_warn = (uint8_t)motor_status.error.motor_temp_warn;
    motor_status_can_msg.device_temp_warn = (uint8_t)motor_status.error.device_temp_warn;
    motor_status_can_msg.vout_sat = (uint8_t)motor_status.error.vout_sat;
    motor_status_can_msg.i_peak_warn = (uint8_t)motor_status.error.i_peak_warn;
    motor_status_can_msg.raceway = (uint8_t)motor_status.error.raceway;
    motor_status_can_msg.i2_r = (uint8_t)motor_status.error.i2_r;
    motor_status_can_msg.ballast_warn = (uint8_t)motor_status.error.ballast_warn;
    ucr_03_motor_status_pack(msg.data, &motor_status_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}

void tcu_svc_can_tx_tcu_faults_data()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = TCU_FAULTS_CAN_ID;
    msg.dlc = UCR_03_TCU_FAULTS_LENGTH;

    tcu_fault_status_t fault_status;
    tcu_data_get_fault_status(&fault_status);

    struct ucr_03_tcu_faults_t tcu_faults_can_msg;
    tcu_faults_can_msg.apps_implausibility = (uint8_t)fault_status.apps_implausibility;
    tcu_faults_can_msg.apps_sensor1_range = (uint8_t)fault_status.apps_sensor1_range;
    tcu_faults_can_msg.apps_sensor2_range = (uint8_t)fault_status.apps_sensor2_range;
    tcu_faults_can_msg.bse_range = (uint8_t)fault_status.bse_range;
    tcu_faults_can_msg.appsbse_implausibility = (uint8_t)fault_status.apps_bse_plausibility;
    ucr_03_tcu_faults_pack(msg.data, &tcu_faults_can_msg, msg.dlc);

    com_svc_can_transmit(&msg);
}


void tcu_svc_can_tx_front_tcu_state()
{
    can_msg_t msg;
    msg.channel = CAN1;
    msg.id = FRONT_CONTROLLER_STATE_CAN_ID;
    msg.dlc = UCR_03_FRONT_CONTROLLER_STATE_LENGTH;

    struct ucr_03_front_controller_state_t front_controller_state;
    front_controller_state.state = (uint8_t)tcu_data_get_state();
    ucr_03_front_controller_state_pack(msg.data, &front_controller_state, msg.dlc);

    com_svc_can_transmit(&msg);
}
