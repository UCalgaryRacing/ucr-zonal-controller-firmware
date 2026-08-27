#ifndef COM_CONFIG_H
#define COM_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "can_config_pinout.h"
#include "com_typ_common.h"

/*============================================================================*/
/* Types                                                                      */
/*============================================================================*/

typedef struct {
    bool started;
} can_context_t;


/*============================================================================*/
/* Channel Hardware Mapping                                                   */
/*============================================================================*/

typedef struct
{
    FDCAN_HandleTypeDef *can_handle;
    can_context_t *context;
} can_hw_t;


typedef struct
{
    bool enabled;
    can_hw_t hw;
} can_config_t;


//Accumulator
#define ACCUMULATOR_MEASUREMENTS_CAN_ID         101
#define ACCUMULATOR_FAULT_CAN_ID                102
#define ACCUMULATOR_SDC_CAN_ID                  99


//Front Controller 
#define FRONT_CONTROLLER_STATE_CAN_ID           200
#define APPS_CAN_ID                             201
#define FRONT_BSE_CAN_ID                        202
#define MOTOR_DATA_CAN_ID                       203
#define MOTOR_REQUEST_CAN_ID                    204
#define MOTOR_STATUS_CAN_ID                     205
#define FRONT_WHEEL_SPEED_CAN_ID                206
#define FRONT_SUSPENSION_CAN_ID                 207
#define FRONT_INSTRUMENTATION_CAN_ID            208
#define TCU_FAULTS_CAN_ID                       213
#define TRACTION_CONTROL_DATA_CAN_ID            220

//Rear Controller
#define GLV_STATUS_CAN_ID                       401
#define REAR_WHEEL_SPEED_CAN_ID                 403
#define REAR_COOLING_CONTROL_CAN_ID             404
#define REAR_CONTROL_CAN_ID                     405
#define REAR_BSE_CAN_ID                         406
#define REAR_COOLANT_TEMP_CAN_ID                407
#define REAR_SUSPENSION_CAN_ID                  408
#define BSPD_CAN_ID                             409
#define REAR_INSTRUMENTATION_CAN_ID             410

//Wheel
#define STEERING_WHEEL_EVENT_CAN_ID             500
#define STEERING_WHEEL_STATE_CAN_ID             501

//Bamocar
#define BAMOCAR_CAN_TX_ID                       513 //to bamocar
#define BAMOCAR_CAN_RX_ID                       385 //from bamocar

//Brusa
#define CHARGER_COMMAND_CAN_ID                  1560
#define CHARGER_STATUS_CAN_ID                   1552
#define CHARGER_ACTUAL_1_CAN_ID                 1553
#define CHARGER_ACTUAL_2_CAN_ID                 1554
#define CHARGER_TEMP_CAN_ID                     1555
#define CHARGER_ERROR_CAN_ID                    1556



/*============================================================================*/
/* Extern Configuration                                                       */
/*============================================================================*/

extern const can_config_t can_config[];

#endif /* COM_CONFIG_H */
