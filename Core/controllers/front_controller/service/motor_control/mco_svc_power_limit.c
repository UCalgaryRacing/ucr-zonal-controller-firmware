#include <stdbool.h>
#include <stdint.h>
#include "mco_svc_bamocar.h"
#include "acu_data.h"
#include "tcu_data.h"
#include "utilities.h"
#include <math.h>

#include "mco_svc_power_limit.h"

/* Tuning constants — adjust on vehicle if needed */
#define MCO_PL_TARGET_MARGIN_FRACTION       0.03f
#define MCO_PL_DC_POWER_FILTER_ALPHA        0.15f
#define MCO_PL_POWER_DEADBAND_W             200.0f
#define MCO_PL_INTEGRATION_DECAY            0.95f
#define MCO_PL_MAX_DROP_PER_CYCLE_PERCENT   2.0f
#define MCO_PL_MAX_RISE_PER_CYCLE_PERCENT   0.5f
#define MCO_PL_KP                           0.000005f
#define MCO_PL_KI                           0.00002f
#define MCO_PL_MAX_INTEGRATION_FRACTION     0.3f

static float g_power_limit_w = 0.0f;
static bool g_initialized = false;
static const float g_torque_percent_full_scale = 100.0f;

static float g_dc_power_filtered = 0.0f;
static float g_pl_max_percent_slewed = 100.0f;
static float g_integration_fraction = 0.0f;
static uint32_t g_old_sample_time = 0u;

static void power_limit_reset_controller_state(void)
{
    g_dc_power_filtered = 0.0f;
    g_pl_max_percent_slewed = g_torque_percent_full_scale;
    g_integration_fraction = 0.0f;
    g_old_sample_time = 0u;
}

static float power_limit_get_target_w(void)
{
    return g_power_limit_w * (1.0f - MCO_PL_TARGET_MARGIN_FRACTION);
}

static float power_limit_rpm_to_torque_percent(float power_w, float rpm)
{
    float mechanical_torque = power_w * BAMOCAR_EFFICIENCY / (fmaxf(rpm, 1.0f) * 2.0f * 3.14f / 60.0f);
    float phase_current_rms = mechanical_torque / BAMOCAR_MOTOR_KT_CONSTANT;
    float phase_current_peak_fraction = phase_current_rms * 1.414f / BAMOCAR_PEAK_CURRENT;

    return clamp_float(phase_current_peak_fraction, 0.0f, 1.0f) * g_torque_percent_full_scale;
}

static float power_limit_get_dt_seconds(void)
{
    uint32_t current_time = HAL_GetTick();

    if (g_old_sample_time == 0u)
    {
        g_old_sample_time = current_time;
        return 0.0f;
    }

    float dt = (float)(current_time - g_old_sample_time) / 1000.0f;
    g_old_sample_time = current_time;

    return dt;
}

static float power_limit_feedforward_percent(void)
{
    float rpm = tcu_data_get_motor_rpm();

    return power_limit_rpm_to_torque_percent(power_limit_get_target_w(), rpm);
}

static float power_limit_feedback_trim_percent(float dt)
{
    if (dt <= 0.0f)
    {
        return 0.0f;
    }

    float dc_power = acu_data_get_aculv_battery_power();
    g_dc_power_filtered += MCO_PL_DC_POWER_FILTER_ALPHA * (dc_power - g_dc_power_filtered);

    float target_w = power_limit_get_target_w();
    float error = target_w - g_dc_power_filtered;

    if (error < 0.0f)
    {
        g_integration_fraction += (error * MCO_PL_KI * dt);
    }
    else if (error > MCO_PL_POWER_DEADBAND_W)
    {
        g_integration_fraction *= MCO_PL_INTEGRATION_DECAY;
    }

    g_integration_fraction = clamp_float(g_integration_fraction, -MCO_PL_MAX_INTEGRATION_FRACTION, 0.0f);

    float correction_fraction = 0.0f;

    if (error < -MCO_PL_POWER_DEADBAND_W)
    {
        correction_fraction = (MCO_PL_KP * error) + g_integration_fraction;
        correction_fraction = fminf(correction_fraction, 0.0f);
    }
    else if (error > MCO_PL_POWER_DEADBAND_W)
    {
        correction_fraction = 0.0f;
    }
    else
    {
        correction_fraction = fminf(g_integration_fraction, 0.0f);
    }

    return correction_fraction * g_torque_percent_full_scale;
}

static float power_limit_apply_slew(float raw_percent)
{
    float delta = raw_percent - g_pl_max_percent_slewed;

    if (delta < -MCO_PL_MAX_DROP_PER_CYCLE_PERCENT)
    {
        delta = -MCO_PL_MAX_DROP_PER_CYCLE_PERCENT;
    }
    else if (delta > MCO_PL_MAX_RISE_PER_CYCLE_PERCENT)
    {
        delta = MCO_PL_MAX_RISE_PER_CYCLE_PERCENT;
    }

    g_pl_max_percent_slewed = clamp_float(g_pl_max_percent_slewed + delta, 0.0f, g_torque_percent_full_scale);

    return g_pl_max_percent_slewed;
}

static float power_limit_calculate_max_torque_percent(void)
{
    float dt = power_limit_get_dt_seconds();
    float feedforward_percent = power_limit_feedforward_percent();
    float feedback_trim_percent = power_limit_feedback_trim_percent(dt);
    float raw_percent = clamp_float(feedforward_percent + feedback_trim_percent, 0.0f, g_torque_percent_full_scale);

    if (dt <= 0.0f)
    {
        g_pl_max_percent_slewed = raw_percent;
        return g_pl_max_percent_slewed;
    }

    return power_limit_apply_slew(raw_percent);
}

void mco_svc_power_limit_init(void)
{
    power_limit_reset_controller_state();
    g_initialized = true;
}

void mco_svc_power_limit_set_limit_w(float power_limit_w)
{
    if (power_limit_w < 0.0f)
    {
        power_limit_w = 0.0f;
    }

    g_power_limit_w = power_limit_w;
    g_integration_fraction = 0.0f;
}

float mco_svc_power_limit_get_limit_w(void)
{
    return g_power_limit_w;
}

/*
 * Feedforward ceiling from RPM and target power (includes safety margin).
 */
float mco_svc_power_limit_calculate_max_torque_percent1(void)
{
    return power_limit_feedforward_percent();
}

/*
 * Feedforward ceiling plus PI feedback trim (no output slew).
 */
float mco_svc_power_limit_calculate_max_torque_percent2(void)
{
    float dt = power_limit_get_dt_seconds();
    float feedforward_percent = power_limit_feedforward_percent();
    float feedback_trim_percent = power_limit_feedback_trim_percent(dt);

    return clamp_float(feedforward_percent + feedback_trim_percent, 0.0f, g_torque_percent_full_scale);
}

float mco_svc_power_limit_limit_torque_percent(float requested_torque_percent)
{
    float clamped_request = clamp_float(requested_torque_percent, 0.0f, g_torque_percent_full_scale);
    float pl_max_percent = power_limit_calculate_max_torque_percent();

    return (clamped_request < pl_max_percent) ? clamped_request : pl_max_percent;
}
