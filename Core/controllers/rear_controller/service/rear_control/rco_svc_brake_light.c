#include "tcu_data.h"
#include "pdm_svc_channel.h"
#include "pdm_config.h"
#include "com_typ_common.h"
#include "rco_svc_brake_light.h"

static bool g_initialized = false; 

status_t rco_svc_brake_light_init(void)
{
    g_initialized = true;
    return OK;
}


void rco_svc_brake_light_update(void)
{
    bool braking = tcu_data_get_brake_active();
    rco_data_set_brake_light(braking);
    pdm_svc_channel_enable(BRAKE_LIGHT, braking);
}
