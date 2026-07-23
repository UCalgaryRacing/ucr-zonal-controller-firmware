#include "fco_svc_ptt.h"
#include "whl_data.h"
#include "fco_drv_ptt.h"

static bool g_initialized = false;

status_t fco_svc_ptt_init(void)
{
    fco_drv_ptt_set_on(false);
    
    g_initialized = true;
    return OK;
}

void fco_svc_ptt_update(void)
{
    bool ptt_pressed = whl_data_get_button_0();

    fco_drv_ptt_set_on(ptt_pressed);
}
