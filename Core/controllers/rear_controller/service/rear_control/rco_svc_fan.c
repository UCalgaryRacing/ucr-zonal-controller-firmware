#include "com_typ_common.h"
#include "whl_data.h"
#include "pdm_config.h"
#include "pdm_svc_channel.h"
#include "rco_data.h"
#include "stm32h7xx_hal.h" // Required for HAL_GetTick()

/*============================================================================*/
/* Private Definitions & Variables                                            */
/*============================================================================*/
typedef enum {
    COOLING_MODE_ALL_OFF = 0,
    COOLING_MODE_PUMPS_ACC_FANS,
    COOLING_MODE_PUMPS_TRAC_FANS,
    COOLING_MODE_PUMPS_ONLY,
    COOLING_MODE_UNKNOWN
} cooling_mode_t;

#define DEVICE_TURN_ON_DELAY_MS 1000U // 500ms delay between each device turning on

static cooling_mode_t g_current_mode = COOLING_MODE_UNKNOWN;
static uint8_t g_sequence_step = 0;
static uint32_t g_last_sequence_tick = 0;

/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

void rco_svc_fan_update(void)
{
    whl_data_state_t wheel_state;
    whl_data_get_state(&wheel_state);

    cooling_mode_t target_mode = COOLING_MODE_UNKNOWN;

    // 1. Determine Requested Target Mode
    if (wheel_state.encoder_0 == 5 || wheel_state.encoder_0 == 7 || wheel_state.encoder_0 == 15 || wheel_state.encoder_0 == 13)
    {
        target_mode = COOLING_MODE_ALL_OFF;
    }
    else if (wheel_state.encoder_0 == 9 || wheel_state.encoder_0 == 1 || wheel_state.encoder_0 == 0 || wheel_state.encoder_0 == 8)
    {
        target_mode = COOLING_MODE_PUMPS_ACC_FANS;
    }
    else if (wheel_state.encoder_0 == 12 || wheel_state.encoder_0 == 4 || wheel_state.encoder_0 == 6 || wheel_state.encoder_0 == 14)
    {
        target_mode = COOLING_MODE_PUMPS_TRAC_FANS;
    }
    else if (wheel_state.encoder_0 == 10 || wheel_state.encoder_0 == 2 || wheel_state.encoder_0 == 3 || wheel_state.encoder_0 == 11) 
    {
        target_mode = COOLING_MODE_PUMPS_ONLY;
    }

    // 2. Handle Mode Change (Reset Sequence & Immediate Off)
    if (target_mode != g_current_mode && target_mode != COOLING_MODE_UNKNOWN)
    {
        g_current_mode = target_mode;
        
        // Reset sequencer
        g_sequence_step = 0; 
        
        // Setting tick slightly in the past ensures the first device turns on immediately
        g_last_sequence_tick = HAL_GetTick() - DEVICE_TURN_ON_DELAY_MS; 

        // Safest approach on mode transition: Turn everything OFF instantly.
        // The sequencer will turn the required devices back on one by one.
        pdm_svc_channel_enable(TRACTIVE_PUMP_1, false);
        pdm_svc_channel_enable(TRACTIVE_PUMP_2, false);
        pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
        pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
        pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
        pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);

        rco_data_set_tractive_pump_1_enable(false);
        rco_data_set_tractive_pump_2_enable(false);
        rco_data_set_tractive_fan_1_enable(false);
        rco_data_set_tractive_fan_2_enable(false);
        rco_data_set_accumulator_fan_1_enable(false);
        rco_data_set_accumulator_fan_2_enable(false);
    }

    if (g_current_mode == COOLING_MODE_ALL_OFF)
    {
        g_sequence_step = 255;
    }
    else if (g_sequence_step < 4)
    {
        if ((HAL_GetTick() - g_last_sequence_tick) >= DEVICE_TURN_ON_DELAY_MS)
        {
            g_last_sequence_tick = HAL_GetTick();

            switch (g_sequence_step)
            {
                case 0: // Step 0: Pump 1
                    pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
                    rco_data_set_tractive_pump_1_enable(true);

                    pdm_svc_channel_enable(TRACTIVE_PUMP_2, false);
                    rco_data_set_tractive_pump_2_enable(false);

                    pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
				   rco_data_set_tractive_fan_1_enable(false);

                   pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
                   rco_data_set_accumulator_fan_1_enable(false);

                   pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
				   rco_data_set_tractive_fan_2_enable(false);

                   pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
                   rco_data_set_accumulator_fan_2_enable(false);
                    break;

                case 1: // Step 1: Pump 2
                    pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
				    rco_data_set_tractive_pump_1_enable(true);

                    pdm_svc_channel_enable(TRACTIVE_PUMP_2, true);
                    rco_data_set_tractive_pump_2_enable(true);

                    pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
				    rco_data_set_tractive_fan_1_enable(false);

                    pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
                    rco_data_set_accumulator_fan_1_enable(false);

                    pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
				    rco_data_set_tractive_fan_2_enable(false);

                    pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
                    rco_data_set_accumulator_fan_2_enable(false);
                    break;

                case 2: // Step 2: Fan 1 (Tractive or Accumulator based on mode)
                    if (g_current_mode == COOLING_MODE_PUMPS_TRAC_FANS) {
                        pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
						rco_data_set_tractive_pump_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_PUMP_2, true);
						rco_data_set_tractive_pump_2_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_1, true);
						rco_data_set_tractive_fan_1_enable(true);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
						rco_data_set_accumulator_fan_1_enable(false);

						pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
						rco_data_set_tractive_fan_2_enable(false);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
						rco_data_set_accumulator_fan_2_enable(false);

                    } else if (g_current_mode == COOLING_MODE_PUMPS_ACC_FANS) {
                        pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
						rco_data_set_tractive_pump_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_PUMP_2, true);
						rco_data_set_tractive_pump_2_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
						rco_data_set_tractive_fan_1_enable(false);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_1, true);
						rco_data_set_accumulator_fan_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
						rco_data_set_tractive_fan_2_enable(false);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
						rco_data_set_accumulator_fan_2_enable(false);
                    }
                    break;

                case 3: // Step 3: Fan 2 (Tractive or Accumulator based on mode)
                    if (g_current_mode == COOLING_MODE_PUMPS_TRAC_FANS) {
                        pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
						rco_data_set_tractive_pump_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_PUMP_2, true);
						rco_data_set_tractive_pump_2_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_1, true);
						rco_data_set_tractive_fan_1_enable(true);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_1, false);
						rco_data_set_accumulator_fan_1_enable(false);

						pdm_svc_channel_enable(TRACTIVE_FAN_2, true);
						rco_data_set_tractive_fan_2_enable(true);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_2, false);
						rco_data_set_accumulator_fan_2_enable(false);
                        
                    } else if (g_current_mode == COOLING_MODE_PUMPS_ACC_FANS) {
                        pdm_svc_channel_enable(TRACTIVE_PUMP_1, true);
						rco_data_set_tractive_pump_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_PUMP_2, true);
						rco_data_set_tractive_pump_2_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_1, false);
						rco_data_set_tractive_fan_1_enable(false);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_1, true);
						rco_data_set_accumulator_fan_1_enable(true);

						pdm_svc_channel_enable(TRACTIVE_FAN_2, false);
						rco_data_set_tractive_fan_2_enable(false);

						pdm_svc_channel_enable(ACCUMULATOR_FAN_2, true);
						rco_data_set_accumulator_fan_2_enable(true);
                    }
                    break;
            }
            g_sequence_step++; // Move to next device
        }
    }
}
