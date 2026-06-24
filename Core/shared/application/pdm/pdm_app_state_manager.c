#include "pdm_app_state_manager.h"
#include "pdm_data.h"
#include "stm32h7xx_hal.h"

/*============================================================================*/
/* Private Variables                                                          */
/*============================================================================*/

static pdm_app_state_t g_current_state = PDM_STATE_STARTUP;

static pdm_app_state_t g_previous_state = PDM_STATE_STARTUP;

static uint32_t g_state_enter_time = 0; 

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/

static pdm_app_state_t handle_startup_state(void);
static pdm_app_state_t handle_idle_state(void);

static void on_state_entry(pdm_app_state_t state);
static void on_state_exit(pdm_app_state_t state);

/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

void pdm_app_state_machine_init(void)
{
    g_current_state = PDM_STATE_STARTUP;
    g_previous_state = PDM_STATE_STARTUP;
}

void pdm_app_state_machine_step(void)
{
    pdm_app_state_t next_state = g_current_state;

    switch (g_current_state)
    {
        case PDM_STATE_STARTUP:
            next_state = handle_startup_state();
            break;

        case PDM_STATE_IDLE:
            next_state = handle_idle_state();
            break;
    }

    if (next_state != g_current_state)
    {
        on_state_exit(g_current_state);

        g_previous_state = g_current_state;
        g_current_state = next_state;

        on_state_entry(g_current_state);
    }
}

/*============================== STATE HANDLERS ==============================*/


static pdm_app_state_t handle_startup_state(void)
{
    return PDM_STATE_STARTUP;
}


static pdm_app_state_t handle_idle_state(void)
{
    return PDM_STATE_IDLE;
}


/*============================= ENTRY / EXIT =================================*/

static void on_state_entry(pdm_app_state_t state)
{
    g_state_enter_time = HAL_GetTick();
    switch (state)
    {
        case PDM_STATE_STARTUP:
            break;

        case PDM_STATE_IDLE:
            break;

        default:
            break;
    }
}


static void on_state_exit(pdm_app_state_t state)
{
    switch (state)
    {
        case PDM_STATE_STARTUP:
            break;

        case PDM_STATE_IDLE:
            break;

        default:
            break;
    }
}