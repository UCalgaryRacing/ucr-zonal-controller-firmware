#include "mcu_svc_analog.h"
#include "tcu_data.h"
#include "mcu_config.h"
#include "tcu_config.h"
#include "tcu_svc_pedals.h"



/*============================================================================*/
/* Private Helpers                                                            */
/*============================================================================*/



static inline const mcu_analog_config_t* pedal_get_config(mcu_analog_channel_id_t channel_id)
{
    const mcu_analog_config_t *config = mcu_svc_get_channel_config(channel_id);

    // if (config == NULL || (config->type != TCU_ANALOG_TYPE_APPS && config->type != TCU_ANALOG_TYPE_BSE))
    //     return NULL;

    return config;
}

static inline void pedal_read_sensor(mcu_analog_channel_id_t channel_id, float *voltage)
{
    mcu_svc_get_analog_voltage(channel_id, voltage);
}

//TODO, possible implement 100ms delay here as allowed by rules
static inline void pedal_validate_range(const mcu_analog_config_t *config, float voltage, tcu_analog_sensor_t *sensor)
{
    if (check_float_limits(voltage, config->settings.valid_max_v, config->settings.valid_min_v))
    {
        sensor->validity = TCU_SENSOR_VALID;
    }
    else
    {
        //sensor->validity = TCU_SENSOR_VALID;
        sensor->validity = TCU_SENSOR_OUT_OF_RANGE;
    }

    sensor->timestamp_ms = HAL_GetTick();
}

static inline void pedal_calculate_percent(const mcu_analog_config_t *config, float voltage, float *percent, tcu_analog_sensor_t *sensor)
{
    mcu_svc_voltage_to_percent(config, voltage, percent);
    sensor->percent = *percent;
}


//TODO update config type to non pointer and remove pointer to poitner from getter
void tcu_svc_apps_update(mcu_analog_channel_id_t apps_1_channel_id, mcu_analog_channel_id_t apps_2_channel_id)
{
    tcu_apps_data_t apps_data;
//    tcu_data_get_apps(&apps_data);

    const mcu_analog_config_t *apps_1_config = pedal_get_config(apps_1_channel_id);
    const mcu_analog_config_t *apps_2_config = pedal_get_config(apps_2_channel_id);

    if (apps_1_config != NULL && apps_2_config != NULL)
    {
        float apps_1_voltage = 0;
        pedal_read_sensor(apps_1_channel_id, &apps_1_voltage);
        apps_data.sensor1.voltage_v = apps_1_voltage;

        float apps_2_voltage = 0;
        pedal_read_sensor(apps_2_channel_id, &apps_2_voltage);
        apps_data.sensor2.voltage_v = apps_2_voltage;

        pedal_validate_range(apps_1_config, apps_1_voltage, &apps_data.sensor1);
        pedal_validate_range(apps_2_config, apps_2_voltage, &apps_data.sensor2);

        float apps_1_percent = 0;
        pedal_calculate_percent(apps_1_config, apps_1_voltage, &apps_1_percent, &apps_data.sensor1);

        float apps_2_percent = 0;
        pedal_calculate_percent(apps_2_config, apps_2_voltage, &apps_2_percent, &apps_data.sensor2);

        apps_data.pedal_percent = apps_1_percent;
    }

    tcu_data_set_apps(&apps_data);
}

void tcu_svc_bse_update(mcu_analog_channel_id_t bse_1_channel_id)
{
    tcu_bse_data_t bse_data;
//    tcu_data_get_bse(&bse_data);

    const mcu_analog_config_t *bse_1_config = pedal_get_config(bse_1_channel_id);

    if (bse_1_config != NULL)
    {
        float bse_1_voltage = 0;
        pedal_read_sensor(bse_1_channel_id, &bse_1_voltage);
        bse_data.sensor.voltage_v = bse_1_voltage;

        pedal_validate_range(bse_1_config, bse_1_voltage, &bse_data.sensor);

        //TODO change to pressure
        float bse_1_percent = 0;
        pedal_calculate_percent(bse_1_config, bse_1_voltage, &bse_1_percent, &bse_data.sensor);

        float bse_1_pressure = bse_1_percent * TCU_BSE_1_PSI_MAX / 100;

        bse_data.pressure_psi = bse_1_pressure;

        bse_data.is_braking = (bse_1_percent > TCU_BSE_1_BRAKING_THRESHOLD_PCT);
        bse_data.is_hard_braking = (bse_1_percent > TCU_BSE_1_HARD_BRAKING_THRESHOLD_PCT);
    }

    tcu_data_set_bse(&bse_data);
}
