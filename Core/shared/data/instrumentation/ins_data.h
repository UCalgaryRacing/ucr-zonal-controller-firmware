#ifndef DATA_INS_DATA_H_
#define DATA_INS_DATA_H_

#include <stdbool.h>
#include <stdint.h>
#include "com_typ_common.h"

/*============================================================================*/
/* Wheel Speed Data                                                           */
/*============================================================================*/

typedef enum
{
    INS_WHEEL_SPEED_SENSOR_FRONT_LEFT = 0,
    INS_WHEEL_SPEED_SENSOR_FRONT_RIGHT,
    INS_WHEEL_SPEED_SENSOR_REAR_LEFT,
    INS_WHEEL_SPEED_SENSOR_REAR_RIGHT,
    INS_WHEEL_SPEED_SENSOR_COUNT
} ins_wheel_speed_sensor_t;

status_t ins_data_init(void);

void ins_data_set_wheel_rpm(ins_wheel_speed_sensor_t sensor, float wheel_rpm);

float ins_data_get_wheel_rpm(ins_wheel_speed_sensor_t sensor);

void ins_data_set_wheel_valid(ins_wheel_speed_sensor_t sensor, bool valid);

bool ins_data_get_wheel_valid(ins_wheel_speed_sensor_t sensor);

void ins_data_set_wheel_period_ticks(ins_wheel_speed_sensor_t sensor, uint32_t period_ticks);

uint32_t ins_data_get_wheel_period_ticks(ins_wheel_speed_sensor_t sensor);

#endif /* DATA_INS_DATA_H_ */
