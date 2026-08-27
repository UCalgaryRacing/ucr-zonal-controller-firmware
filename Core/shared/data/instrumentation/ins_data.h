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
    float wheel_rpm;
    bool wheel_valid;
    uint32_t wheel_period_ticks;
} ins_wheel_speed_calc_data_t;

typedef struct
{
    ins_wheel_speed_calc_data_t front_left;
    ins_wheel_speed_calc_data_t front_right;
    ins_wheel_speed_calc_data_t rear_left;
    ins_wheel_speed_calc_data_t rear_right;
} ins_wheel_speed_data_t;

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

status_t ins_data_set_wheel_speed_rpm(ins_sensor_id_t wheel_sensor, float wheel_rpm);
status_t ins_data_set_suspension_travel(ins_sensor_id_t suspension_sensor, float susp_travel);

status_t ins_data_set_wheel_period_ticks(ins_sensor_id_t wheel_sensor_id, uint32_t period_ticks); // no getter for now
status_t ins_data_set_wheel_speed_valid(ins_sensor_id_t wheel_sensor_id, bool valid); // no getter for now

void ins_data_set_steering_angle(float steering_angle);

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

float ins_data_get_wheel_speed_rpm(ins_sensor_id_t wheel_sensor);
float ins_data_get_suspension_travel(ins_sensor_id_t suspension_sensor);

float ins_data_get_steering_angle(void);


#endif /* DATA_INS_DATA_H_*/
