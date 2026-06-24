#ifndef DATA_TCU_DATA_H_
#define DATA_TCU_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"
#include "tcu_data.h"
#include "tcu_app_state_manager.h"

/*============================================================================*/
/* Sensor Validity                                                            */
/*============================================================================*/

typedef enum
{
    TCU_SENSOR_VALID = 0,
    TCU_SENSOR_OUT_OF_RANGE,
} tcu_sensor_validity_t;

/*============================================================================*/
/* Sensor Data                                                                */
/*============================================================================*/

typedef struct
{
    float voltage_v;
    float percent;
    tcu_sensor_validity_t validity;
    uint32_t timestamp_ms;
} tcu_analog_sensor_t;

/*============================================================================*/
/* APPS Data (sensor readings only, written by acquisition service)           */
/*============================================================================*/

typedef struct
{
    tcu_analog_sensor_t sensor1;
    tcu_analog_sensor_t sensor2;
    float pedal_percent;
} tcu_apps_data_t;

/*============================================================================*/
/* BSE Data                                                                   */
/*============================================================================*/

typedef struct
{
    tcu_analog_sensor_t sensor;
    float pressure_psi;
    bool is_braking;
    bool is_hard_braking;
} tcu_bse_data_t;

/*============================================================================*/
/* Fault Data Structures                                                      */
/*============================================================================*/

typedef struct
{
    bool apps_implausibility;
    bool apps_sensor1_range;
    bool apps_sensor2_range;
    bool bse_range;
    bool apps_bse_plausibility;
} tcu_fault_status_t;

/*============================================================================*/
/* APPS Plausibility State (written by fault service)                         */
/*============================================================================*/

typedef struct
{
    bool is_plausible;
    bool implausibility_active;
    uint32_t implausibility_start_ms;
} tcu_apps_plausibility_t;

/*============================================================================*/
/* Motor Controller Data Structures                                           */
/*============================================================================*/

typedef struct
{
    float rpm;
    float active_current_a;
    float reactive_current_a;
    float ac_current_a;
    float motor_temp_c;
    float inverter_temp_c;
} tcu_motor_data_t;

typedef struct
{
    float peak_ac_current_a;
    float continuous_ac_current_a;
    float torque_request_percent;
    bool enable;
    bool reverse;
    //TODO add speed request
} tcu_motor_request_t;

typedef struct
{
    bool ena;
    bool nc_r0;
    bool lim_plus;
    bool lim_minus;
    bool ok;
    bool icns;
    bool t_nlim;
    bool pn;
    bool ni;
    bool n0;
    bool rsw;
    bool cal0;
    bool cal;
    bool tol;
    bool rdy;
    bool brk0;
    bool sign_mag;
    bool nclip;
    bool nclip_plus;
    bool nclip_minus;
    bool lrd_dig;
    bool luse_rchd;
    bool lrd_n;
    bool ird_ti;
    bool ird_tir;
    bool ird10_hz;
    bool ird_tm;
    bool ird_ana;
    bool lwcns;
    bool rfe_pulse;
    bool fiwe;
    bool hnd_whl;
} tcu_bamocar_status_display_t;

typedef struct
{
    bool bad_paras;
    bool power_fault;
    bool rfe_fault;
    bool bus_timeout;
    bool feedback;
    bool power_voltage;
    bool motor_temp_fault;
    bool device_temp_fault;
    bool over_voltage;
    bool i_peak_fault;
    bool raceaway;
    bool user;
    bool hw_err;
    bool ballast_fault;
    bool warning0;
    bool illegal_status;
    bool safe_in;
    bool motor_temp_warn;
    bool device_temp_warn;
    bool vout_sat;
    bool i_peak_warn;
    bool raceway;
    bool i2_r;
    bool ballast_warn;
} tcu_bamocar_error_display_t;

typedef struct
{
    tcu_bamocar_status_display_t status;
    tcu_bamocar_error_display_t error;
} tcu_motor_status_t;

/*============================================================================*/
/* Traction Control Data (written by traction control service)                */
/*============================================================================*/

typedef struct
{
    float measured_slip_ratio;
    float desired_slip_ratio;
    float bamocar_n_lim_percent;
} tcu_traction_control_data_t;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t tcu_data_init(void);

/*============================================================================*/
/* APPS Data Access                                                           */
/*============================================================================*/

status_t tcu_data_set_apps(const tcu_apps_data_t *apps_data);

status_t tcu_data_get_apps(tcu_apps_data_t *apps_data);

float tcu_data_get_apps_percent(void);

bool tcu_data_get_apps_fault(void);

/*============================================================================*/
/* APPS Plausibility Access                                                   */
/*============================================================================*/

void tcu_data_set_apps_plausibility(const tcu_apps_plausibility_t *plausibility);

void tcu_data_get_apps_plausibility(tcu_apps_plausibility_t *plausibility);

/*============================================================================*/
/* BSE Data Access                                                            */
/*============================================================================*/

status_t tcu_data_set_bse(const tcu_bse_data_t *bse_data);

status_t tcu_data_get_bse(tcu_bse_data_t *bse_data);

bool tcu_data_get_brake_active(void);

bool tcu_data_get_hard_braking(void);

/*============================================================================*/
/* Motor Data Access                                                          */
/*============================================================================*/

status_t tcu_data_set_motor(const tcu_motor_data_t *motor_data);

status_t tcu_data_get_motor(tcu_motor_data_t *motor_data);

float tcu_data_get_motor_rpm(void);

void tcu_data_set_motor_rpm(float rpm);

void tcu_data_set_motor_active_current(float active_current_a);

void tcu_data_set_motor_reactive_current(float reactive_current_a);

void tcu_data_set_motor_ac_current(float ac_current_a);

float tcu_data_get_motor_temp(void);

void tcu_data_set_motor_temp(float temp);

float tcu_data_get_inverter_temp(void);

void tcu_data_set_inverter_temp(float temp);

status_t tcu_data_get_motor_status(tcu_motor_status_t *motor_status);

void tcu_data_set_bamocar_status_display(const tcu_bamocar_status_display_t *status);

void tcu_data_set_bamocar_error_display(const tcu_bamocar_error_display_t *error);

/*============================================================================*/
/* Traction Control Data Access                                               */
/*============================================================================*/

status_t tcu_data_set_traction_control(const tcu_traction_control_data_t *traction_control_data);

status_t tcu_data_get_traction_control(tcu_traction_control_data_t *traction_control_data);

float tcu_data_get_traction_control_measured_slip_ratio(void);

void tcu_data_set_traction_control_measured_slip_ratio(float measured_slip_ratio);

float tcu_data_get_traction_control_desired_slip_ratio(void);

void tcu_data_set_traction_control_desired_slip_ratio(float desired_slip_ratio);

float tcu_data_get_traction_control_bamocar_n_lim_percent(void);

void tcu_data_set_traction_control_bamocar_n_lim_percent(float bamocar_n_lim_percent);

/*============================================================================*/
/* Motor Request Data Access                                                  */
/*============================================================================*/

status_t tcu_data_set_motor_request(const tcu_motor_request_t *motor_request_data);

status_t tcu_data_get_motor_request(tcu_motor_request_t *motor_request_data);

bool tcu_data_get_motor_request_enabled(void);

bool tcu_data_get_motor_request_reverse(void);

/*============================================================================*/
/* RTD Status Access                                                          */
/*============================================================================*/

void tcu_data_set_rtd_pressed(bool rtd_pressed);

bool tcu_data_get_rtd_pressed(void);

/*============================================================================*/
/* Fault Status Access                                                        */
/*============================================================================*/

status_t tcu_data_set_fault_status(const tcu_fault_status_t *fault_status);

status_t tcu_data_get_fault_status(tcu_fault_status_t *fault_status);

bool tcu_data_get_fault_active(void);

bool tcu_data_is_motor_enabled(void);

/*============================================================================*/
/* State Data Access                                                          */
/*============================================================================*/

void tcu_data_set_state(tcu_app_state_t tcu_state);

tcu_app_state_t tcu_data_get_state();

#endif /* DATA_TCU_DATA_H_ */
