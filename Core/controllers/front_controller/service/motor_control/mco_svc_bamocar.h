#ifndef MCO_SVC_BAMOCAR_H_
#define MCO_SVC_BAMOCAR_H_

#include "com_typ_common.h"
#include "mco_svc_bamocar_regs.h"
#include "mco_config.h"
#include "can_config.h"
#include "can_typ_common.h"
#include "utilities.h"

//TODO make internal functions for common operations as public API, dont expose function to write directly to registers in this header

status_t mco_svc_bamocar_init(void);
status_t mco_svc_bamocar_start(void);
status_t mco_svc_bamocar_set_torque_percent(float torque_percent);
status_t mco_svc_bamocar_set_torque_nm(float torque_request_nm, bool rms);
status_t mco_svc_bamocar_set_rpm(float rpm);
status_t mco_svc_bamocar_set_n_lim_percent(float n_lim_percent);
status_t mco_svc_bamocar_request_data(uint8_t reg, uint8_t interval);
status_t mco_svc_bamocar_write_register(uint8_t reg, int16_t value);
void mco_svc_bamocar_can_rx(can_msg_t *msg);
float mco_svc_bamocar_calculate_theoretical_dc_power(float ac_current, float dc_voltage, float vout);



#define BAMOCAR_CMD_MAX_VALUE               32767
#define BAMOCAR_CMD_MIN_VALUE               -32767

/*============================================================================*/
/* Motor-Specific Parameters                                                   */
/*============================================================================*/

#define BAMOCAR_MOTOR_MAX_RPM                     6000U
#define BAMOCAR_MOTOR_MAX_NM                      230U
#define BAMOCAR_MOTOR_KT_CONSTANT                 0.61

#define BAMOCAR_MOTOR_TORQUE_MAX_RAW			  BAMOCAR_CMD_MAX_VALUE
#define BAMOCAR_MOTOR_TORQUE_MIN_RAW			  BAMOCAR_CMD_MIN_VALUE
#define BAMOCAR_MOTOR_RPM_MAX_RAW			      BAMOCAR_CMD_MAX_VALUE
#define BAMOCAR_MOTOR_RPM_MIN_RAW			      BAMOCAR_CMD_MIN_VALUE

/*============================================================================*/
/* Inverter-Specific Parameters                                               */
/*============================================================================*/

#define BAMOCAR_DC_VOLTAGE_GAIN_FACTOR     31.5848f
#define BAMOCAR_EFFICIENCY                 0.92
#define BAMOCAR_PEAK_CURRENT               424.4f
#define BAMOCAR_CONTINUOUS_CURRENT         200.0f

#endif /* MCO_SVC_BAMOCAR_H_ */
