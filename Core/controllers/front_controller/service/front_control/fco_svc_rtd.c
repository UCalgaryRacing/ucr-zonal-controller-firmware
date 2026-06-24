#include "fco_svc_rtd.h"

#include "mcu_config.h"
#include "tcu_data.h"
#include "tcu_drv_rtd.h"

static bool g_initialized = false;
static bool g_rtd_debounced;
static uint8_t g_rtd_debounce_counter;

void fco_svc_rtd_init(void)
{
    g_initialized = true;
    return;
}

void fco_svc_rtd_button_update(void)
{
    bool raw = tcu_drv_rtd_is_button_pressed();

    if (raw != g_rtd_debounced)
    {
        g_rtd_debounce_counter++;

        if (g_rtd_debounce_counter >= 5U)
        {
            g_rtd_debounced = raw;
            g_rtd_debounce_counter = 0U;
        }
    }
    else
    {
        g_rtd_debounce_counter = 0U;
    }

    tcu_data_set_rtd_pressed(g_rtd_debounced);
}
