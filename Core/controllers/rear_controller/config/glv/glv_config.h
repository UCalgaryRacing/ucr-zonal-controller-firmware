#ifndef CONFIG_GLV_CONFIG_H_
#define CONFIG_GLV_CONFIG_H_

/*============================================================================*/
/* BATT_HIGHEST_TEMP                                                          */
/*============================================================================*/

#define GLV_CELL_TEMP_VOLTAGE_MIN_V             1.448f
#define GLV_CELL_TEMP_VOLTAGE_MAX_V             2.1f

#define GLV_CELL_TEMP_ACTIVE_MIN_V              1.448f
#define GLV_CELL_TEMP_ACTIVE_MAX_V              2.1f

/* Multiplier to compensate for voltage dividers on the input */
#define GLV_CELL_TEMP_SCALING_FACTOR            4.407f

#define GLV_TEMP_SENSOR_SLOPE                   -92.02f
#define GLV_TEMP_SENSOR_INTERCEPT               193.15f

/*============================================================================*/
/* BATT_LOWEST_VOLTAGE                                                        */
/*============================================================================*/

#define GLV_CELL_VOLTAGE_MIN_V                   2.5f
#define GLV_CELL_VOLTAGE_MAX_V                   4.2f

#define GLV_CELL_VOLTAGE_ACTIVE_MIN_V            2.5f
#define GLV_CELL_VOLTAGE_ACTIVE_MAX_V            4.2f
#define GLV_CELL_MAXIMUM_TEMPERATURE_C           60.0f

/* Multiplier to compensate for voltage dividers on the input */
#define GLV_CELL_VOLTAGE_SCALING_FACTOR          4.407f

#endif /* CONFIG_GLV_CONFIG_H_ */
