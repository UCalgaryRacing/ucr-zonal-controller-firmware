#include "pdm_svc_channel.h"
#include "pdm_config.h"
#include "rco_svc_buzzer.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

/*============================================================================*/
/* Private Variables                                                          */
/*============================================================================*/

static uint32_t g_buzzer_off_time;
static bool g_buzzer_active;

/*============================================================================*/
/* Private Function Prototypes                                                */
/*============================================================================*/

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

void rco_svc_buzzer_enable(uint32_t duration_ms)
{
    g_buzzer_active = true;
    g_buzzer_off_time = HAL_GetTick() + duration_ms;

    pdm_svc_channel_enable(RTD_BUZZER, true);
}

void rco_svc_buzzer_update(void)
{
    if (g_buzzer_active && HAL_GetTick() >= g_buzzer_off_time)
    {
        g_buzzer_active = false;
        pdm_svc_channel_enable(RTD_BUZZER, false);
    }
}
