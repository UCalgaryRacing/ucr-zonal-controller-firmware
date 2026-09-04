// #include "ins_svc_wheel_speed.h"
// #include "ins_config.h"
// #include "ins_config_pinout.h"
// #include "ins_drv_timer.h"
// #include "ins_data.h"

// #include <string.h>

// /*============================================================================*/
// /* Static Variables                                                           */
// /*============================================================================*/
// //TODO: why is some of it here and some in data 
// typedef struct 
// {
//     volatile uint32_t previous_capture_ticks;
//     volatile uint32_t last_edge_time_ms;
//     volatile bool has_previous_edge;

//     volatile float raw_wheel_rpm;

//     float rpm_sample_buffer[INS_WHEEL_RPM_MOVING_AVERAGE_SAMPLES];
//     uint8_t rpm_sample_count;
//     uint8_t rpm_sample_index;

//     bool initialized;

// } ins_wheel_speed_runtime_data_t;


// // static volatile uint32_t g_previous_capture_ticks = 0U;
// // static volatile uint32_t g_last_edge_time_ms = 0U;
// // static volatile bool g_has_previous_edge = false;
// // static volatile bool g_initialized = false;
// // static volatile float g_raw_wheel_rpm = 0.0f;

// // static float g_rpm_sample_buffer[INS_WHEEL_RPM_MOVING_AVERAGE_SAMPLES];
// // static uint8_t g_rpm_sample_count = 0U;
// // static uint8_t g_rpm_sample_index = 0U;

// static ins_wheel_speed_runtime_data_t g_wheel_speed_runtime_data[INS_WHEEL_SPEED_SENSOR_COUNT];

// /*============================================================================*/
// /* Static Helper Functions                                                    */
// /*============================================================================*/

// static void ins_svc_wheel_speed_reset_moving_average(ins_sensor_id_t id)
// {
//     g_wheel_speed_runtime_data[id].rpm_sample_count = 0U;
//     g_wheel_speed_runtime_data[id].rpm_sample_index = 0U;

//     memset(g_wheel_speed_runtime_data[id].rpm_sample_buffer, 0, sizeof(g_wheel_speed_runtime_data[id].rpm_sample_buffer));
// }

// static float ins_svc_wheel_speed_apply_moving_average(ins_sensor_id_t id, float raw_rpm)
// {
//     g_wheel_speed_runtime_data[id].rpm_sample_buffer[g_wheel_speed_runtime_data[id].rpm_sample_index] = raw_rpm;
//     g_wheel_speed_runtime_data[id].rpm_sample_index = (g_wheel_speed_runtime_data[id].rpm_sample_index + 1U) % INS_WHEEL_RPM_MOVING_AVERAGE_SAMPLES;

//     if (g_wheel_speed_runtime_data[id].rpm_sample_count < INS_WHEEL_RPM_MOVING_AVERAGE_SAMPLES)
//     {
//         g_wheel_speed_runtime_data[id].rpm_sample_count++;
//     }

//     float sum = 0.0f;
//     for (uint8_t i = 0U; i < g_wheel_speed_runtime_data[id].rpm_sample_count; i++)
//     {
//         sum += g_wheel_speed_runtime_data[id].rpm_sample_buffer[i];
//     }

//     return sum / (float)g_wheel_speed_runtime_data[id].rpm_sample_count;
// }

// /*============================================================================*/
// /* Initialization                                                             */
// /*============================================================================*/

// status_t ins_svc_wheel_speed_init(void)
// {
//     memset(&g_wheel_speed_runtime_data, 0, sizeof(g_wheel_speed_runtime_data));

//     for (uint8_t i = 0U; i < RL_WHEEL_SPEED; i++) // rear wheel speed not used in this repo
//     {
//         ins_svc_wheel_speed_reset_moving_average(i);

//         status_t status = ins_drv_timer_start_input_capture_it(ins_wheel_speed_sensor_config[i].hw.timer_handle, ins_wheel_speed_sensor_config[i].hw.timer_channel);

//         if (status != OK)
//         {
//             return status;
//         }

//         g_wheel_speed_runtime_data[i].initialized = true;
//     }

//     return OK;
// }

// /*============================================================================*/
// /* Wheel Speed                                                                */
// /*============================================================================*/

// void ins_svc_wheel_speed_on_rising_edge(ins_sensor_id_t id)
// {
//     if (g_wheel_speed_runtime_data[id].initialized == false)
//     {
//         return;
//     }

//     uint32_t now_ticks = ins_drv_timer_get_capture(ins_wheel_speed_sensor_config[id].hw.timer_handle, ins_wheel_speed_sensor_config[id].hw.timer_channel);
//     uint32_t now_ms = HAL_GetTick();

//     // If the wheel has been stopped for too long, re-arm using this edge only.
//     if ((g_wheel_speed_runtime_data[id].has_previous_edge == false) || ((now_ms - g_wheel_speed_runtime_data[id].last_edge_time_ms) > INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS))
//     {
//         g_wheel_speed_runtime_data[id].previous_capture_ticks = now_ticks;
//         g_wheel_speed_runtime_data[id].last_edge_time_ms = now_ms;
//         ins_data_set_wheel_period_ticks(id, 0U);
//         g_wheel_speed_runtime_data[id].raw_wheel_rpm = 0.0f;
//         ins_data_set_wheel_speed_valid(id, false);
//         ins_svc_wheel_speed_reset_moving_average(id);
//         g_wheel_speed_runtime_data[id].has_previous_edge = true;
//         return;
//     }

//     uint32_t delta_ticks = now_ticks - g_wheel_speed_runtime_data[id].previous_capture_ticks;
//     g_wheel_speed_runtime_data[id].previous_capture_ticks = now_ticks;
//     g_wheel_speed_runtime_data[id].last_edge_time_ms = now_ms;

//     if (delta_ticks == 0U)
//     {
//         return;
//     }

//     float wheel_rpm = (60.0f * (float)INS_FRONT_WHEEL_TIMER_FREQUENCY_HZ) / ((float)delta_ticks * (float)INS_FRONT_WHEEL_PULSES_PER_ROTATION);

//     ins_data_set_wheel_period_ticks(id, delta_ticks);
//     g_wheel_speed_runtime_data[id].raw_wheel_rpm = wheel_rpm;
//     ins_data_set_wheel_speed_valid(id, true);
// }

// void ins_svc_wheel_speed_update(ins_sensor_id_t id)
// {
//     if (g_wheel_speed_runtime_data[id].initialized == false)
//     {
//         return;
//     }

//     float raw_rpm = g_wheel_speed_runtime_data[id].raw_wheel_rpm;

//     if ((HAL_GetTick() - g_wheel_speed_runtime_data[id].last_edge_time_ms) > INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS)
//     {
//         ins_data_set_wheel_period_ticks(id, 0U);
//         g_wheel_speed_runtime_data[id].raw_wheel_rpm = 0.0f;
//         raw_rpm = 0.0f;
//         ins_data_set_wheel_speed_valid(id, false);
//         ins_svc_wheel_speed_reset_moving_average(id);
//     }

//     float smoothed_rpm = ins_svc_wheel_speed_apply_moving_average(id, raw_rpm);
//     ins_data_set_wheel_speed_rpm(id, smoothed_rpm);
// }

// void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
// {
//     if (ins_drv_timer_is_handle(htim, INS_FL_WHEEL_TIMER_HANDLE) && ins_drv_timer_is_active_channel(htim, INS_FL_WHEEL_TIMER_CHANNEL))
//     {
//         ins_svc_wheel_speed_on_rising_edge(FL_WHEEL_SPEED);
//     }

//     else if (ins_drv_timer_is_handle(htim, INS_FR_WHEEL_TIMER_HANDLE) && ins_drv_timer_is_active_channel(htim, INS_FR_WHEEL_TIMER_CHANNEL))
//     {
//         ins_svc_wheel_speed_on_rising_edge(FR_WHEEL_SPEED);
//     }
// }
