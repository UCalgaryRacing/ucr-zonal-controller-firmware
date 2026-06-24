#include "acu_data.h"
#include "com_typ_common.h"
#include "pdm_config.h"
#include "pdm_svc_channel.h"
#include "rco_svc_tssi.h"
#include "rco_data.h"

static bool g_initialized = false; 

static bool g_tssi_red_state = false;
static uint32_t g_tssi_red_time_start;
static uint32_t g_tssi_red_time_elapsed;

static bool g_acu_fault_latch;

// initialized as green on, red off before anything
status_t rco_svc_tssi_init(void)
{
    pdm_svc_channel_enable(TSSI_GREEN, true);
    pdm_svc_channel_enable(TSSI_RED, false);

    g_tssi_red_time_start = HAL_GetTick();
    g_tssi_red_time_elapsed = 0;
    g_acu_fault_latch = false;

    g_initialized = true;
    return OK;
}

void rco_svc_tssi_update(void)
{
    /* when IMD or BMS fault detected
     * 1. GREEN off
     * 2. RED flash (2-5Hz, 50% duty)
     */
    bool bms_fault = acu_data_get_bms_fault_active();
    bool imd_fault = acu_data_get_imd_fault_active();

    bool acu_fault = bms_fault || imd_fault;

    // initial latching
    if (acu_fault && (g_acu_fault_latch == false))
    { 
        // enable red light, turn green light off
        g_tssi_red_state = true;
        g_tssi_red_time_start = HAL_GetTick();

        pdm_svc_channel_enable(TSSI_RED, true);  
        pdm_svc_channel_enable(TSSI_GREEN, false);

        g_acu_fault_latch = true;
    }

    // keep red flashing
    else if (g_acu_fault_latch == true)
    {
    	// check reset button, return to no fault state
    	if (rco_data_get_reset_pressed() == true)
    	{
    		g_acu_fault_latch = false;
    		g_tssi_red_state = false;

    	}

    	else
    	{
			pdm_svc_channel_enable(TSSI_GREEN, false);

			g_tssi_red_time_elapsed = HAL_GetTick() - g_tssi_red_time_start;

			if (g_tssi_red_time_elapsed >= TSSI_RED_FLASH_DURATION_MS)
			{
				g_tssi_red_state = !g_tssi_red_state;
				pdm_svc_channel_enable(TSSI_RED, g_tssi_red_state);

				g_tssi_red_time_start = HAL_GetTick();
			}
    	}
    }

    // green on red off
    else 
    {
    	pdm_svc_channel_enable(TSSI_GREEN, true);
    	pdm_svc_channel_enable(TSSI_RED, false);
    }

    rco_data_set_tssi_faulted(g_acu_fault_latch);
}
