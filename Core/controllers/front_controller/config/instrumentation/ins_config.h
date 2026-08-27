#ifndef CONFIG_INS_CONFIG_H_
#define CONFIG_INS_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"

#include "ins_config_sensor_id.h"

/*============================================================================*/
/* Wheel Speed                                                                */
/*============================================================================*/

/* Maps to ins_wheel_speed_sensor_t; validated at runtime in service init. */
#define INS_FRONT_WHEEL_SENSOR_ID                     0U
/* TIM2 counter tick frequency = TIM2CLK (240 MHz) / (Prescaler 239 + 1) = 1 MHz. */
#define INS_FRONT_WHEEL_TIMER_FREQUENCY_HZ            1000000U
#define INS_FRONT_WHEEL_PULSES_PER_ROTATION           30U
#define INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS            2000U
/* Number of 10 ms fast-task samples averaged to smooth brief RPM drops. */
#define INS_WHEEL_RPM_MOVING_AVERAGE_SAMPLES    10U

/*============================================================================*/
/* Hardware Mapping                                                           */
/*============================================================================*/

typedef struct 
{
    TIM_HandleTypeDef *timer_handle;
    uint8_t timer_channel;
} ins_wheel_speed_sensor_hw_t;

// TODO: clean up? maybe just one struct
typedef struct
{
    ins_wheel_speed_sensor_hw_t hw;
} ins_wheel_speed_sensor_config_t;

extern const ins_wheel_speed_sensor_config_t ins_wheel_speed_sensor_config[INS_TOTAL_NUM_SENSORS];


/*============================================================================*/
/* Front Left Suspension Configuration                                        */
/*============================================================================*/

#define INS_FL_SUSP_VOLTAGE_MIN_V             0.0f    // TODO: find real values of active and minmax voltage
#define INS_FL_SUSP_VOLTAGE_MAX_V             12.0f

#define INS_FL_SUSP_ACTIVE_MIN_V              0.0f
#define INS_FL_SUSP_ACTIVE_MAX_V              12.0f

#define INS_FL_SUSP_SCALING_FACTOR            4.85 // from resistor divider (input filter)

#define INS_FL_SUSP_POSITIVE_SLOPE            true

/*============================================================================*/
/* Front Right Suspension Configuration                                        */
/*============================================================================*/

#define INS_FR_SUSP_VOLTAGE_MIN_V             0.0f    // TODO: find real values of active and minmax voltage
#define INS_FR_SUSP_VOLTAGE_MAX_V             12.0f

#define INS_FR_SUSP_ACTIVE_MIN_V              0.0f
#define INS_FR_SUSP_ACTIVE_MAX_V              12.0f

#define INS_FR_SUSP_SCALING_FACTOR            4.85 // from resistor divider (input filter)

#define INS_FR_SUSP_POSITIVE_SLOPE            true

#endif /*CONFIG_INS_CONFIG_H_*/
