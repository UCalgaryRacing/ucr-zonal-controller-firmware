#ifndef MCO_SVC_BAMOCAR_REGS_H_
#define MCO_SVC_BAMOCAR_REGS_H_

#include "mco_svc_bamocar.h"

/*============================================================================*/
/* Register Addresses (REGID)                                                  */
/*============================================================================*/

//TODO maybe change these to enums?
/* Status/Feedback Registers (Read from servo) */
#define BAMOCAR_REG_ACTIVE_CURRENT          0x27U
#define BAMOCAR_REG_REACTIVE_CURRENT        0x28U
#define BAMOCAR_REG_RPM_ACTUAL              0x30U
#define BAMOCAR_REG_MOTOR_TEMP              0x49U
#define BAMOCAR_REG_INVERTER_TEMP           0x4AU
#define BAMOCAR_REG_AC_CURRENT              0x20U
#define BAMOCAR_REG_CURRENT_LIMIT           0x48U
#define BAMOCAR_REG_VOUT                    0x8AU
#define BAMOCAR_REG_ERROR_CODE              0x8FU
#define BAMOCAR_REG_TORQUE_ACTUAL           0xA0U
#define BAMOCAR_REG_DC_VOLTAGE              0xEBU
#define BAMOCAR_STATUS_INFO					0x40
#define BAMOCAR_ERROR_INFO					0x8F

/* Command Registers (Write to servo) */
#define BAMOCAR_REG_RPM_CMD                 0x31U
#define BAMOCAR_REG_N_LIM                   0x34U
#define BAMOCAR_REG_REQUEST_DATA            0x3DU
#define BAMOCAR_REG_TORQUE_CMD              0x90U

/*============================================================================*/
/* Data Request Intervals                                                      */
/*============================================================================*/

#define BAMOCAR_REPEAT_ONCE                 0x00U
#define BAMOCAR_REPEAT_10MS                 0x0AU
#define BAMOCAR_REPEAT_20MS                 0x14U
#define BAMOCAR_REPEAT_50MS                 0x32U
#define BAMOCAR_REPEAT_100MS                0x64U
#define BAMOCAR_REPEAT_STOP                 0xFFU



#endif /* MCO_SVC_BAMOCAR_REGS_H_ */
