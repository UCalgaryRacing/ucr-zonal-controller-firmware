#include "rco_svc_reset_button.h"
#include "rco_data.h"
#include "rco_drv_reset.h"


/*============================================================================*/
/* Private Variables                                                          */
/*============================================================================*/

static bool g_initialized = false;

static bool g_reset_debounced;
static uint8_t g_reset_debounce_counter;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

void rco_svc_reset_init()
{
    g_initialized = true;
    g_reset_debounce_counter = 0; // check if needed? doesn't exist in rtd file
    return;
}

// update to data layer
void rco_svc_reset_button_update()
{

    bool raw = rco_drv_reset_is_button_pressed();

    //if raw value is different from previous stable value, begin counting 
    if (raw != g_reset_debounced)
    {
        g_reset_debounce_counter++;

        //if same reading for 5 continuous samples, update value
        if (g_reset_debounce_counter >= 5)
        {
            g_reset_debounced = raw;
            g_reset_debounce_counter = 0;
        }
    }
    else
    {
        g_reset_debounce_counter = 0;
    }
    
    rco_drv_broadcast_reset_button(g_reset_debounced);

    rco_data_set_reset_pressed(g_reset_debounced);

}
