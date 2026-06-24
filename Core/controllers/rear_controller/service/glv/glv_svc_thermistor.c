#include "glv_config.h"

float glv_svc_convert_voltage_to_temperature(float temp_sensor_voltage)
{

    float cell_temp;
    // this works assuming that the temperature sensor is a linear sensor with a known slope and intercept, which is true for an analog IC but not necessairly true for a thermistor.
    
    cell_temp = (temp_sensor_voltage) * GLV_TEMP_SENSOR_SLOPE + GLV_TEMP_SENSOR_INTERCEPT;

    return cell_temp;
}