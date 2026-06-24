#include "fco_svc_ptt.h"
#include "mcu_config.h"
#include "tcu_data.h"
#include "mcu_svc_hsd.h"
#include "whl_data.h"

static bool g_initialized = false;
static bool g_ptt_active = false;

status_t fco_svc_ptt_init(void)
{
	mcu_svc_hsd_channel_enable(PTT, false);
    g_initialized = true;
    return OK;
}

void fco_svc_ptt_update(void)
{
    bool ptt_pressed = whl_data_get_button_0();
    g_ptt_active = ptt_pressed;
	mcu_svc_hsd_channel_enable(PTT, ptt_pressed);
}
