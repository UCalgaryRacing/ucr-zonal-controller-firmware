#include "mco_svc_traction_control.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "ins_data.h"
#include "mco_svc_bamocar.h"
#include "tcu_data.h"

static float mco_svc_traction_control_lookup_current_limit_rms(float motor_rpm);
static float mco_svc_traction_control_clamp(float value, float min_value, float max_value);

static bool g_initialized = false;
static const float g_bamocar_peak_current_a = 424.4f;
static const float g_sqrt_2 = 1.414f;
static const float g_motor_to_rear_wheel_gear_ratio = 3.818f;
static const float g_min_slip_reference_wheel_rpm = 1.0f;
static const float g_default_rpm_breakpoints[] = {
    0.0f,
    6000.0f
};

static const float g_default_current_limit_rms_a[] = {
    300.1f,
    300.1f
};

static const float *g_active_rpm_breakpoints = g_default_rpm_breakpoints;
static const float *g_active_current_limit_rms_a = g_default_current_limit_rms_a;
static uint32_t g_active_table_size = (uint32_t)(sizeof(g_default_rpm_breakpoints) / sizeof(g_default_rpm_breakpoints[0]));

#define LC_START_PCT           15.0f
#define LC_RAMP_RATE           150.0f
#define LC_RAMP_CAP_PCT        60.0f
#define LC_RAMP_MAX_TIME_MS    300.0f
#define LC_HANDOVER_SPEED_RPM  300.0f
#define LC_TARGET_SLIP_RATIO   0.15f
#define LC_KP                  0.5f
#define LC_KI                  0.1f
#define LC_KD                  0.0f
#define LC_PCT_RATE_INCREASE   5.00f
#define LC_PCT_RATE_DECREASE   0.75f

static bool lc_ramp_active = true;
static float previous_slip_error = 0.0f;
static float slip_error = 0.0f;
static float integral_error = 0.0f;
static float prev_calculated_torque = 0.0f;


float mco_svc_launch_control_calculate_torque_percent(float requested_torque_percent)
{
    float rpm_front = ins_data_get_wheel_speed_rpm(FR_WHEEL_SPEED);
    float rpm_rear = tcu_data_get_motor_rpm() / g_motor_to_rear_wheel_gear_ratio;
    float slip_ratio = (rpm_rear - rpm_front) / rpm_front;
    float lc_time_elapsed = (HAL_GetTick() - tcu_data_get_traction_control_time_started());
    float lc_calculated_torque;
    float deriv_error;

    if (lc_ramp_active)
    {
        lc_calculated_torque = LC_START_PCT + (LC_PCT_RATE_INCREASE * lc_time_elapsed / 1000.0f);
        lc_calculated_torque = mco_svc_traction_control_clamp(lc_calculated_torque, 0.0f, LC_RAMP_CAP_PCT);
        lc_calculated_torque = mco_svc_traction_control_clamp(lc_calculated_torque, 0.0f, requested_torque_percent);

        if ((lc_time_elapsed >= LC_RAMP_MAX_TIME_MS) || (rpm_front >= LC_HANDOVER_SPEED_RPM) || (slip_ratio >= LC_TARGET_SLIP_RATIO))
        {
            lc_ramp_active = false;
            previous_slip_error = LC_TARGET_SLIP_RATIO - slip_ratio;
            integral_error = lc_calculated_torque;
            prev_calculated_torque = lc_calculated_torque;
        }

        return lc_calculated_torque;
    }

    if (!lc_ramp_active)
    {
        slip_error = LC_TARGET_SLIP_RATIO - slip_ratio;
        integral_error += LC_KI * slip_error * 0.01f;
        integral_error = mco_svc_traction_control_clamp(integral_error, 0.0f, LC_RAMP_CAP_PCT);
        
        deriv_error = (slip_error - previous_slip_error) / 0.01f;

        lc_calculated_torque = integral_error + (LC_KP * slip_error) + (LC_KD * deriv_error);
        lc_calculated_torque = mco_svc_traction_control_clamp(lc_calculated_torque, 0.0f, 100.0f);
        lc_calculated_torque = mco_svc_traction_control_clamp(lc_calculated_torque, prev_calculated_torque * LC_PCT_RATE_DECREASE, prev_calculated_torque * LC_PCT_RATE_INCREASE);
        lc_calculated_torque = mco_svc_traction_control_clamp(lc_calculated_torque, 0.0f, requested_torque_percent);
    
        prev_calculated_torque = lc_calculated_torque;

        return lc_calculated_torque;
    }

    return 0.0f;
}


void mco_svc_traction_control_init(void)
{
    mco_svc_traction_control_reset_current_limit_map();
    g_initialized = true;
}

bool mco_svc_traction_control_set_current_limit_map(const float *rpm_breakpoints,
                                                    const float *current_limit_rms_a,
                                                    uint32_t table_size)
{
    if ((rpm_breakpoints == NULL) || (current_limit_rms_a == NULL) || (table_size < 2U))
    {
        return false;
    }

    for (uint32_t i = 1U; i < table_size; i++)
    {
        if (rpm_breakpoints[i] <= rpm_breakpoints[i - 1U])
        {
            return false;
        }
    }

    g_active_rpm_breakpoints = rpm_breakpoints;
    g_active_current_limit_rms_a = current_limit_rms_a;
    g_active_table_size = table_size;

    return true;
}

void mco_svc_traction_control_reset_current_limit_map(void)
{
    g_active_rpm_breakpoints = g_default_rpm_breakpoints;
    g_active_current_limit_rms_a = g_default_current_limit_rms_a;
    g_active_table_size = (uint32_t)(sizeof(g_default_rpm_breakpoints) / sizeof(g_default_rpm_breakpoints[0]));
}

float mco_svc_traction_control_calculate_max_torque_percent(void)
{
    if (g_initialized == false)
    {
        return 100.0f;
    }

    float motor_rpm = tcu_data_get_motor_rpm();
    if (motor_rpm < 0.0f)
    {
        motor_rpm = 0.0f;
    }
    float tc_limit_current_rms = mco_svc_traction_control_lookup_current_limit_rms(motor_rpm);
    float tc_limit_current_peak = tc_limit_current_rms * g_sqrt_2;
    float tc_limit_percent = tc_limit_current_peak * 100.0f / g_bamocar_peak_current_a;

    return mco_svc_traction_control_clamp(tc_limit_percent, 0.0f, 100.0f);
}

float mco_svc_traction_control_limit_torque_percent(float requested_torque_percent)
{
    float clamped_request = mco_svc_traction_control_clamp(requested_torque_percent, 0.0f, 100.0f);
    float tc_max_percent = mco_svc_traction_control_calculate_max_torque_percent();

    return (clamped_request < tc_max_percent) ? clamped_request : tc_max_percent;
}

float mco_svc_traction_control_calculate_motor_speed_slip_ratio(void)
{
    float front_left_wheel_rpm = ins_data_get_wheel_speed_rpm(FL_WHEEL_SPEED);
    float motor_rpm = tcu_data_get_motor_rpm();
    float rear_wheel_rpm = motor_rpm / g_motor_to_rear_wheel_gear_ratio;
    float measured_slip_ratio;

    if (front_left_wheel_rpm < g_min_slip_reference_wheel_rpm)
    {
        measured_slip_ratio = 0.0f;
    }
    else
    {
        measured_slip_ratio = (rear_wheel_rpm - front_left_wheel_rpm) / front_left_wheel_rpm;
    }

    tcu_data_set_traction_control_measured_slip_ratio(measured_slip_ratio);

    return measured_slip_ratio;
}

bool mco_svc_traction_control_update_bamocar_n_lim(float desired_slip_ratio)
{
    float front_left_wheel_rpm = ins_data_get_wheel_speed_rpm(FL_WHEEL_SPEED);
    float n_lim_percent = 100.0f;
    tcu_data_set_traction_control_desired_slip_ratio(desired_slip_ratio);

    if (front_left_wheel_rpm >= MCO_TC_N_LIM_ENGAGE_WHEEL_RPM)
    {
        float desired_rear_wheel_rpm = front_left_wheel_rpm * (1.0f + desired_slip_ratio);
        float desired_motor_rpm = desired_rear_wheel_rpm * g_motor_to_rear_wheel_gear_ratio;

        n_lim_percent = desired_motor_rpm * 100.0f / (float)BAMOCAR_MOTOR_MAX_RPM;
        n_lim_percent = mco_svc_traction_control_clamp(n_lim_percent, 0.0f, 100.0f);

        if (front_left_wheel_rpm < MCO_TC_N_LIM_FULL_TC_WHEEL_RPM)
        {
            n_lim_percent = mco_svc_traction_control_clamp(n_lim_percent,
                                                           MCO_TC_N_LIM_MIN_PERCENT,
                                                           100.0f);
        }
    }

    tcu_data_set_traction_control_bamocar_n_lim_percent(n_lim_percent);

    return (mco_svc_bamocar_set_n_lim_percent(n_lim_percent) == 0);
}

static float mco_svc_traction_control_clamp(float value, float min_value, float max_value)
{
    if (value < min_value)
    {
        return min_value;
    }

    if (value > max_value)
    {
        return max_value;
    }

    return value;
}

static float mco_svc_traction_control_lookup_current_limit_rms(float motor_rpm)
{
    if ((g_active_table_size == 0U) || (g_active_rpm_breakpoints == NULL) || (g_active_current_limit_rms_a == NULL))
    {
        return 0.0f;
    }

    if (motor_rpm <= g_active_rpm_breakpoints[0])
    {
        return g_active_current_limit_rms_a[0];
    }

    if (motor_rpm >= g_active_rpm_breakpoints[g_active_table_size - 1U])
    {
        return g_active_current_limit_rms_a[g_active_table_size - 1U];
    }

    for (uint32_t i = 0U; i < (g_active_table_size - 1U); i++)
    {
        float rpm_lo = g_active_rpm_breakpoints[i];
        float rpm_hi = g_active_rpm_breakpoints[i + 1U];

        if ((motor_rpm >= rpm_lo) && (motor_rpm <= rpm_hi))
        {
            float current_lo = g_active_current_limit_rms_a[i];
            float current_hi = g_active_current_limit_rms_a[i + 1U];
            float alpha = (motor_rpm - rpm_lo) / (rpm_hi - rpm_lo);

            return current_lo + (alpha * (current_hi - current_lo));
        }
    }

    return g_active_current_limit_rms_a[g_active_table_size - 1U];
}

