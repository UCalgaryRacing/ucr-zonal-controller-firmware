#include "rco_svc_drs.h"
#include "rco_drv_drs.h"
#include "rco_config.h"
#include "rco_data.h"
#include "stm32h7xx_hal.h"
#include "rco_data.h"
#include "tcu_data.h"
#include "ins_data.h"
#include "whl_data.h"

void rco_svc_update_drs()
{   
    bool drs_active = rco_data_get_drs();
    if (ins_data_get_wheel_speed_rpm(FL_WHEEL_SPEED) > VEHICLE_SPEED_DRS_ACTIVATION_LIMIT)
    {
        drs_active = true;
    }

    if(whl_data_get_button_1())
    {
        drs_active = true;
    }

    if(tcu_data_get_brake_active())
    {
        drs_active = false;
    }

    rco_svc_set_drs(drs_active);
}

void rco_svc_set_drs(bool drs_active)
{
    rco_data_set_drs(drs_active);
    if(drs_active)
    {
        //drs open
        rco_drv_drs_set_duty(DRS_OPEN_PWM_DUTY_CYCLE);

    }
    else
    {   
        //drs closed
        rco_drv_drs_set_duty(DRS_CLOSED_PWM_DUTY_CYCLE);
    }
}

void rco_svc_toggle_drs()
{   
    bool drs_active = rco_data_get_drs_active();
    if(drs_active)
    {   
        drs_active = false;
        rco_svc_set_drs(drs_active);
        
    }
    else
    {
        drs_active =  true;
        rco_svc_set_drs(drs_active);
    }
}

void rco_svc_drs_init()
{   
    bool drs_active;
    //drs closed
    rco_drv_drs_init();
    rco_data_set_drs(false);
    rco_drv_drs_set_duty(DRS_CLOSED_PWM_DUTY_CYCLE);
    drs_active = false;
    rco_data_set_drs_active(drs_active);
}
