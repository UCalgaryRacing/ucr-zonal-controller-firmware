#include "fco_svc_dash_lights.h"

#include "acu_data.h"
#include "com_typ_common.h"
#include "pdm_config.h"
#include "pdm_svc_channel.h"
#include "rco_data.h"
#include "tcu_data.h"

static bool g_bms_fault_latch = false;
static bool g_imd_fault_latch = false;
static bool g_initialized = false;

status_t fco_svc_dash_lights_init(void)
{
    pdm_svc_channel_enable(BMS_LIGHT, false);
    pdm_svc_channel_enable(IMD_LIGHT, false);
    pdm_svc_channel_enable(RTD_LIGHT, false);
    g_initialized = true;
    return OK;
}

void fco_svc_dash_lights_update(void)
{
    acu_fault_t accumulator_faults;
    acu_data_get_fault_status(&accumulator_faults);

    if (rco_data_get_reset_pressed() == true)
    {
        g_bms_fault_latch = false;
        g_imd_fault_latch = false;
    }

    if (accumulator_faults.bms_fault == true && (g_bms_fault_latch == false))
    {
        pdm_svc_channel_enable(BMS_LIGHT, true);
        g_bms_fault_latch = true;
    }
    else if (g_bms_fault_latch == true)
    {
        pdm_svc_channel_enable(BMS_LIGHT, true);
    }
    else
    {
        pdm_svc_channel_enable(BMS_LIGHT, false);
    }

    if (accumulator_faults.imd_fault == true && (g_imd_fault_latch == false))
    {
        pdm_svc_channel_enable(IMD_LIGHT, accumulator_faults.imd_fault);
        g_imd_fault_latch = true;
    }
    else if (g_imd_fault_latch == true)
    {
        pdm_svc_channel_enable(IMD_LIGHT, true);
    }
    else
    {
        pdm_svc_channel_enable(IMD_LIGHT, false);
    }

    tcu_app_state_t tcu_state = tcu_data_get_state();
    bool rtd_active = (tcu_state == TCU_APP_STATE_READY_TO_DRIVE || tcu_state == TCU_APP_STATE_REVERSE);

    pdm_svc_channel_enable(RTD_LIGHT, rtd_active);
}
