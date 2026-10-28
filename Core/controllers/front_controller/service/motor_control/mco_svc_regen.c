
#include "mco_config.h"
#include "utilities.h"
#include "mco_svc_regen.h"
#include "tcu_data.h"
#include "ins_data.h"
#include "tcu_config.h"


bool mco_svc_regen_is_active()
{
    tcu_apps_data_t apps_data;
    tcu_data_get_apps(&apps_data);

    tcu_bse_data_t bse_data;
    tcu_data_get_bse(&bse_data);

    float wheel_rpm = ins_data_get_wheel_speed_rpm(FL_WHEEL_SPEED);

    bool regen_active = (apps_data.pedal_percent < TCU_APPS_REGEN_THRESHOLD_PCT) &&
                      (bse_data.sensor.percent > TCU_BSE_REGEN_THRESHOLD_PCT) &&
                      (wheel_rpm > 100.0f);


    return regen_active;
}

float mco_svc_regen_calculate_torque()
{
    tcu_bse_data_t bse_data;
    tcu_data_get_bse(&bse_data);

    float regen_torque = -1.0f * bse_data.pressure_psi * 0.0227f;

    return regen_torque;
}
