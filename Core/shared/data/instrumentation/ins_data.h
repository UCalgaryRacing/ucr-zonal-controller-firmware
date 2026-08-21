#ifndef DATA_INS_DATA_H_
#define DATA_INS_DATA_H_

#include <stdbool.h>
#include <stdint.h>
#include "com_typ_common.h"

#include "ins_config_sensor_id.h"

/*============================================================================*/
/* Data Structure Types                                                       */
/*============================================================================*/
typedef struct
{
    float front_left_wheel_rpm;
    float front_right_wheel_rpm;
    float rear_left_wheel_rpm;
    float rear_right_wheel_rpm;
} ins_whl_spd_data_t;

typedef struct
{
    float front_left_suspension;
    float front_right_suspension;
    float rear_left_suspension;
    float rear_right_suspension;
} ins_suspension_data_t;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t ins_data_init(void);

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

status_t ins_data_set_wheel_speed(ins_sensor_id_t wheel_sensor, float wheel_rpm);
status_t ins_data_set_suspension_travel(ins_sensor_id_t suspension_sensor, float susp_travel);

void ins_data_set_steering_angle(float steering_angle);

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

float ins_data_get_wheel_speed_rpm(ins_sensor_id_t wheel_sensor);
float ins_data_get_suspension_travel(ins_sensor_id_t suspension_sensor);

float ins_data_get_steering_angle(void);


#endif /* DATA_INS_DATA_H_ */
