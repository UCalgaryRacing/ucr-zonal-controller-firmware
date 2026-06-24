/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/

#include "tcu_app_state_manager.h"
#include "tcu_data.h"
#include "rco_svc_buzzer.h"
#include "stm32h7xx_hal.h"
#include "mcu_svc_hsd.h"
#include "mcu_config.h"
#include "pdm_svc_channel.h"
#include "pdm_config.h"
#include "rco_svc_drs.h"
#include "rco_data.h"
#include "cmsis_os2.h"
#include "rco_svc_logging.h"

/*============================================================================*/
/* Private Variables                                                          */
/*============================================================================*/

static tcu_app_state_t g_current_state = TCU_APP_STATE_STARTUP;

static tcu_app_state_t g_previous_state = TCU_APP_STATE_STARTUP;

static uint32_t g_state_enter_time = 0; 

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/

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
    tcu_app_state_t next_state = tcu_data_get_state();

    if (next_state != g_current_state)
    {
        on_state_exit(g_current_state);

        g_previous_state = g_current_state;
        g_current_state = next_state;

        on_state_entry(g_current_state);
    }
}



/*============================= ENTRY / EXIT =================================*/

static void on_state_entry(tcu_app_state_t state)
{
    g_state_enter_time = HAL_GetTick();
    switch (state)
    {
        case TCU_APP_STATE_READY_TO_DRIVE:
            rco_svc_buzzer_enable(2000);
            mcu_svc_hsd_channel_enable(BAMOCAR_FRG, true);
            break;

        case TCU_APP_STATE_REVERSE:
            //tcu_svc_buzzer_start_reverse_mode(300);
            break;

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
    	case TCU_APP_STATE_READY_TO_DRIVE:
    		mcu_svc_hsd_channel_enable(BAMOCAR_FRG, false);
    		break;
        case TCU_APP_STATE_REVERSE:
            //tcu_svc_buzzer_stop();
            //tcu_svc_reverse_disable();
            break;

        default:
            break;
    }
}


tcu_app_state_t tcu_app_state_machine_get_state(void)
{
    return g_current_state;
}
