/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/

#include "tcu_app_state_manager.h"
#include "tcu_data.h"
#include "acu_data.h"
#include "stm32h7xx_hal.h"
#include "pdm_config.h"
#include "acu_data.h"

/*============================================================================*/
/* Private Variables                                                          */
/*============================================================================*/

static tcu_app_state_t g_current_state = TCU_APP_STATE_STARTUP;

static tcu_app_state_t g_previous_state = TCU_APP_STATE_STARTUP;

static uint32_t g_state_enter_time = 0; 

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/

static tcu_app_state_t handle_startup_state(void);
static tcu_app_state_t handle_idle_state(void);
static tcu_app_state_t handle_rtd_state(void);
// static tcu_app_state_t handle_reverse_state(void);
// static tcu_app_state_t handle_motor_disabled_state(void);

static void on_state_entry(tcu_app_state_t state);
static void on_state_exit(tcu_app_state_t state);

/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

void tcu_app_state_machine_init(void)
{
    g_current_state = TCU_APP_STATE_STARTUP;
    g_previous_state = TCU_APP_STATE_STARTUP;
}

void tcu_app_state_machine_step(void)
{
    tcu_app_state_t next_state = g_current_state;
	tcu_data_set_state(g_current_state);
    switch (g_current_state)
    {
        case TCU_APP_STATE_STARTUP:
            next_state = handle_startup_state();
            break;

        case TCU_APP_STATE_IDLE:
            next_state = handle_idle_state();
            break;

        case TCU_APP_STATE_READY_TO_DRIVE:
            next_state = handle_rtd_state();
            break;

        // case TCU_APP_STATE_REVERSE:
        //     next_state = handle_reverse_state();
        //     break;

        default:
            next_state = TCU_APP_STATE_STARTUP;
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


static tcu_app_state_t handle_startup_state(void)
{
    //TODO add bamocar and acu communication validation
    //if (tcu_data_get_bamocar_ready()
//    {
//        return TCU_APP_STATE_IDLE;
//    }
//
//    return TCU_APP_STATE_STARTUP;

	return TCU_APP_STATE_IDLE;
}


static tcu_app_state_t handle_idle_state(void)
{
	if(tcu_data_get_brake_active() &&
       tcu_data_get_rtd_pressed() &&
	   acu_data_get_aculv_ts_active() &&
       (HAL_GetTick() - g_state_enter_time) >= 2000)
    {
        return TCU_APP_STATE_READY_TO_DRIVE;
    }

    return TCU_APP_STATE_IDLE;
}


static tcu_app_state_t handle_rtd_state(void)
{

    if ((tcu_data_get_rtd_pressed() &&
       tcu_data_get_brake_active() &&
	   (HAL_GetTick() - g_state_enter_time) >= 2000)
    	|| !acu_data_get_aculv_ts_active())
    {
        return TCU_APP_STATE_IDLE;
    }

    return TCU_APP_STATE_READY_TO_DRIVE;
}


// static tcu_app_state_t handle_reverse_state(void)
// {
//     // if (!tcu_data_get_motor_request_enabled())
//     // {
//     //     return TCU_APP_STATE_MOTOR_DISABLED;
//     // }

//     // if (acu_data_get_aculv_ts_active() &&
//         if (tcu_data_get_brake_active() &&
//         tcu_data_get_rtd_pressed() &&
//         (HAL_GetTick() - g_state_enter_time) >= 2000)
//     {
//         return TCU_APP_STATE_READY_TO_DRIVE;
//     }

//     return TCU_APP_STATE_REVERSE;
// }


/*============================= ENTRY / EXIT =================================*/

static void on_state_entry(tcu_app_state_t state)
{
    g_state_enter_time = HAL_GetTick();
    switch (state)
    {
        case TCU_APP_STATE_READY_TO_DRIVE:
        case TCU_APP_STATE_IDLE:
        case TCU_APP_STATE_STARTUP:
        case TCU_APP_STATE_MOTOR_DISABLED:
        default:
            break;
    }
}


static void on_state_exit(tcu_app_state_t state)
{
    switch (state)
    {
        default:
            break;
    }
}


//TODO remove this and use data layer setter instead
tcu_app_state_t tcu_app_state_machine_get_state(void)
{
    return g_current_state;
}
