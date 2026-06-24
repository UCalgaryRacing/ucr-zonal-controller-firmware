#include "ins_svc_wheel_speed.h"
#include "ins_config.h"
#include "ins_config_pinout.h"
#include "ins_drv_timer.h"
#include "ins_data.h"

#include <string.h>

static volatile uint32_t g_previous_capture_ticks = 0U;
static volatile uint32_t g_last_edge_time_ms = 0U;
static volatile bool g_has_previous_edge = false;
static volatile bool g_initialized = false;
static volatile float g_raw_wheel_rpm = 0.0f;

static float g_rpm_sample_buffer[INS_FRONT_WHEEL_RPM_MOVING_AVERAGE_SAMPLES];
static uint8_t g_rpm_sample_count = 0U;
static uint8_t g_rpm_sample_index = 0U;

static const ins_wheel_speed_sensor_t g_sensor_id = (ins_wheel_speed_sensor_t)INS_FRONT_WHEEL_SENSOR_ID;

static void ins_svc_wheel_speed_reset_moving_average(void)
{
    g_rpm_sample_count = 0U;
    g_rpm_sample_index = 0U;
    memset(g_rpm_sample_buffer, 0, sizeof(g_rpm_sample_buffer));
}

static float ins_svc_wheel_speed_apply_moving_average(float raw_rpm)
{
    g_rpm_sample_buffer[g_rpm_sample_index] = raw_rpm;
    g_rpm_sample_index = (g_rpm_sample_index + 1U) % INS_FRONT_WHEEL_RPM_MOVING_AVERAGE_SAMPLES;

    if (g_rpm_sample_count < INS_FRONT_WHEEL_RPM_MOVING_AVERAGE_SAMPLES)
    {
        g_rpm_sample_count++;
    }

    float sum = 0.0f;
    for (uint8_t i = 0U; i < g_rpm_sample_count; i++)
    {
        sum += g_rpm_sample_buffer[i];
    }

    return sum / (float)g_rpm_sample_count;
}

status_t ins_svc_wheel_speed_init(void)
{
    g_previous_capture_ticks = 0U;
    g_last_edge_time_ms = HAL_GetTick();
    g_has_previous_edge = false;
    g_raw_wheel_rpm = 0.0f;
    ins_svc_wheel_speed_reset_moving_average();

    ins_data_set_wheel_rpm(g_sensor_id, 0.0f);
    ins_data_set_wheel_period_ticks(g_sensor_id, 0U);
    ins_data_set_wheel_valid(g_sensor_id, false);

    status_t status = ins_drv_timer_start_input_capture_it(INS_FRONT_WHEEL_TIMER_HANDLE,
                                                           INS_FRONT_WHEEL_TIMER_CHANNEL);
    if (status != OK)
    {
        return status;
    }

    g_initialized = true;

    return OK;
}

void ins_svc_wheel_speed_on_rising_edge(void)
{
    if (g_initialized == false)
    {
        return;
    }

    uint32_t now_ticks = ins_drv_timer_get_capture(INS_FRONT_WHEEL_TIMER_HANDLE, INS_FRONT_WHEEL_TIMER_CHANNEL);
    uint32_t now_ms = HAL_GetTick();

    // If the wheel has been stopped for too long, re-arm using this edge only.
    if ((g_has_previous_edge == false) || ((now_ms - g_last_edge_time_ms) > INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS))
    {
        g_previous_capture_ticks = now_ticks;
        g_last_edge_time_ms = now_ms;
        ins_data_set_wheel_period_ticks(g_sensor_id, 0U);
        g_raw_wheel_rpm = 0.0f;
        ins_data_set_wheel_valid(g_sensor_id, false);
        ins_svc_wheel_speed_reset_moving_average();
        g_has_previous_edge = true;
        return;
    }

    uint32_t delta_ticks = now_ticks - g_previous_capture_ticks;
    g_previous_capture_ticks = now_ticks;
    g_last_edge_time_ms = now_ms;

    if (delta_ticks == 0U)
    {
        return;
    }

    float wheel_rpm = (60.0f * (float)INS_FRONT_WHEEL_TIMER_FREQUENCY_HZ) / ((float)delta_ticks * (float)INS_FRONT_WHEEL_PULSES_PER_ROTATION);

    ins_data_set_wheel_period_ticks(g_sensor_id, delta_ticks);
    g_raw_wheel_rpm = wheel_rpm;
    ins_data_set_wheel_valid(g_sensor_id, true);
}

void ins_svc_wheel_speed_update(void)
{
    if (g_initialized == false)
    {
        return;
    }

    float raw_rpm = g_raw_wheel_rpm;

    if ((HAL_GetTick() - g_last_edge_time_ms) > INS_FRONT_WHEEL_STOPPED_TIMEOUT_MS)
    {
        ins_data_set_wheel_period_ticks(g_sensor_id, 0U);
        g_raw_wheel_rpm = 0.0f;
        raw_rpm = 0.0f;
        ins_data_set_wheel_valid(g_sensor_id, false);
        ins_svc_wheel_speed_reset_moving_average();
    }

    float smoothed_rpm = ins_svc_wheel_speed_apply_moving_average(raw_rpm);
    ins_data_set_wheel_rpm(g_sensor_id, smoothed_rpm);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (ins_drv_timer_is_handle(htim, INS_FRONT_WHEEL_TIMER_HANDLE)
        && ins_drv_timer_is_active_channel(htim, INS_FRONT_WHEEL_TIMER_CHANNEL))
    {
        ins_svc_wheel_speed_on_rising_edge();
    }
}
