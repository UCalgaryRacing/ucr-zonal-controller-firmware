#ifndef CONFIG_INS_CONFIG_H_
#define CONFIG_INS_CONFIG_H_

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
#define INS_FRONT_WHEEL_RPM_MOVING_AVERAGE_SAMPLES    10U

#endif