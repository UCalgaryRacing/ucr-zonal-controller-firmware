#include "whl_data.h"
#include <string.h>

whl_data_event_t g_wheel_event;
whl_data_state_t g_wheel_state;
static bool g_initialized;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t whl_data_init(void)
{
    memset(&g_wheel_state, 0, sizeof(g_wheel_state));

    g_wheel_state.button_0 = false;
    g_wheel_state.button_1 = false;
    g_wheel_state.button_2 = false;
    g_wheel_state.button_3 = false;
    g_wheel_state.encoder_0 = 0U;
    g_wheel_state.encoder_1 = 0U;

    memset(&g_wheel_event, 0, sizeof(g_wheel_event));

    g_wheel_event.driver_flag_trigger = false;
    g_wheel_event.lap_completed_trigger = false;
    g_wheel_event.sector_completed_trigger = false;
    g_wheel_event.sector_number = 0U;
    g_wheel_event.event_time_ms = 0U;
    g_wheel_event.lap_number = 0U;

    g_initialized = true;
    return OK;
}

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

status_t whl_data_set_state(whl_data_state_t *state)
{
    if(g_initialized)
    {
        g_wheel_state = *state;
        return OK;
    }
    
    return ERROR_NOT_INITIALIZED;
}

status_t whl_data_set_event(whl_data_event_t *event)
{
    if(g_initialized)
    {
        g_wheel_event = *event;
        return OK;
    }
    
    return ERROR_NOT_INITIALIZED;
}

void whl_data_set_button_0(bool state)
{
    g_wheel_state.button_0 = state;
}

void whl_data_set_button_1(bool state)
{
    g_wheel_state.button_1 = state;
}

void whl_data_set_button_2(bool state)
{
    g_wheel_state.button_2 = state;
}

void whl_data_set_button_3(bool state)
{
    g_wheel_state.button_3 = state;
}

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

status_t whl_data_get_state(whl_data_state_t * dest)
{
    if(g_initialized)
    {
        *dest = g_wheel_state;
        return OK;
    }
    
    return ERROR_NOT_INITIALIZED;
}

status_t whl_data_get_event(whl_data_event_t * dest)
{
    if(g_initialized)
    {
        *dest = g_wheel_event;
        return OK;
    }
    
    return ERROR_NOT_INITIALIZED;
}


bool whl_data_get_button_0()
{
    return g_wheel_state.button_0;
}

bool whl_data_get_button_1()
{
    return g_wheel_state.button_1;
}

bool whl_data_get_button_2()
{
    return g_wheel_state.button_2;
}

bool whl_data_get_button_3()
{
    return g_wheel_state.button_3;
}