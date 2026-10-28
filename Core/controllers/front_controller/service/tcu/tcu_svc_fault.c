#include "tcu_svc_fault.h"
#include "tcu_data.h"
#include "tcu_config.h"
#include "mcu_config.h"
#include "stm32h7xx_hal.h"
#include <math.h>

void tcu_svc_fault_check_apps_plausibility(void)
{
    tcu_apps_data_t apps_data;
    tcu_data_get_apps(&apps_data);

    tcu_apps_plausibility_t plausibility;
    tcu_data_get_apps_plausibility(&plausibility);

    float diff = fabsf(apps_data.sensor1.percent - apps_data.sensor2.percent);

    if (diff > TCU_APPS_IMPLAUSIBILITY_THRESHOLD_PCT)
    {
        if (plausibility.is_plausible == true)
        {
            plausibility.implausibility_start_ms = HAL_GetTick();
        }

        plausibility.is_plausible = false;

        if ((HAL_GetTick() - plausibility.implausibility_start_ms) > TCU_APPS_IMPLAUSIBILITY_TIMEOUT_MS)
        {
            plausibility.implausibility_active = true;
        }
        else
        {
            plausibility.implausibility_active = false;
        }
    }
    else
    {
        plausibility.is_plausible = true;
        plausibility.implausibility_active = false;
    }

    tcu_data_set_apps_plausibility(&plausibility);

    tcu_fault_status_t fault;
    tcu_data_get_fault_status(&fault);
    fault.apps_implausibility = plausibility.implausibility_active;
    tcu_data_set_fault_status(&fault);
}

void tcu_svc_fault_check_apps_bse_plausibility(void)
{
    tcu_apps_data_t apps_data;
    tcu_data_get_apps(&apps_data);

    tcu_bse_data_t bse_data;
    tcu_data_get_bse(&bse_data);

    tcu_fault_status_t fault;
    tcu_data_get_fault_status(&fault);

    if (fault.apps_bse_plausibility == true)
    {
        if (apps_data.pedal_percent <= TCU_APPS_BSE_PLAUSIBILITY_RELEASE_PCT)
        {
            fault.apps_bse_plausibility = false;
        }
    }
    else
    {
        fault.apps_bse_plausibility = (apps_data.pedal_percent > TCU_APPS_BSE_PLAUSIBILITY_APPS_THRESHOLD_PCT && bse_data.is_hard_braking);
    }

    tcu_data_set_fault_status(&fault);
}

void tcu_svc_fault_check_apps_sensor_range(void)
{
    tcu_apps_data_t apps_data;
    tcu_data_get_apps(&apps_data);

    tcu_fault_status_t fault;
    tcu_data_get_fault_status(&fault);

    fault.apps_sensor1_range = (apps_data.sensor1.validity == TCU_SENSOR_OUT_OF_RANGE);
    fault.apps_sensor2_range = (apps_data.sensor2.validity == TCU_SENSOR_OUT_OF_RANGE);

    tcu_data_set_fault_status(&fault);
}

void tcu_svc_fault_check_bse_sensor_range(void)
{
    tcu_bse_data_t bse_data;
    tcu_data_get_bse(&bse_data);

    tcu_fault_status_t fault;
    tcu_data_get_fault_status(&fault);

    fault.bse_range = (bse_data.sensor.validity == TCU_SENSOR_OUT_OF_RANGE);

    tcu_data_set_fault_status(&fault);
}
