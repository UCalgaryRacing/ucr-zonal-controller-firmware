#include "mcu_svc_analog.h"

#include "mcu_drv_analog.h"

mcu_analog_runtime_config_t mcu_analog_runtime_config[MCU_TOTAL_ANALOG_CHANNELS];

static inline bool is_valid_channel_id(mcu_analog_channel_id_t channel_id)
{
    return (channel_id < MCU_TOTAL_ANALOG_CHANNELS);
}

const mcu_analog_config_t* mcu_svc_get_channel_config(mcu_analog_channel_id_t channel_id)
{
    if (!is_valid_channel_id(channel_id))
    {
        return NULL;
    }
    return &mcu_analog_config[channel_id];
}

static float mcu_svc_raw_to_voltage(uint16_t adc_raw, const mcu_adc_context_t *adc_settings);

status_t mcu_svc_analog_init(mcu_analog_channel_id_t channel_id)
{
    const mcu_analog_config_t *channel_config = mcu_svc_get_channel_config(channel_id);

    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    return mcu_drv_analog_init(&channel_config->hw);
}

status_t mcu_svc_analog_start(mcu_analog_channel_id_t channel_id)
{
    const mcu_analog_config_t *channel_config = mcu_svc_get_channel_config(channel_id);

    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    return mcu_drv_analog_start_adc(&channel_config->hw);
}

status_t mcu_svc_get_analog_voltage(mcu_analog_channel_id_t channel_id, float *voltage)
{
    const mcu_analog_config_t *channel_config = mcu_svc_get_channel_config(channel_id);

    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    uint16_t raw_adc;
    status_t status = mcu_drv_analog_read_raw(&channel_config->hw, &raw_adc);
    if (status != OK)
    {
        return status;
    }

    float unscaled_voltage = mcu_svc_raw_to_voltage(raw_adc, channel_config->hw.adc_context);
    *voltage = unscaled_voltage * channel_config->settings.scaling_factor;

    return OK;
}

status_t mcu_svc_get_analog_percent(mcu_analog_channel_id_t channel_id, float *percentage)
{
    const mcu_analog_config_t *channel_config = mcu_svc_get_channel_config(channel_id);

    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    float voltage;
    status_t status = mcu_svc_get_analog_voltage(channel_id, &voltage);
    if (status != OK)
    {
        return status;
    }

    return mcu_svc_voltage_to_percent(channel_config, voltage, percentage);
}

static float mcu_svc_raw_to_voltage(uint16_t adc_raw, const mcu_adc_context_t *adc_settings)
{
    return adc_raw * adc_settings->adc_vref / (adc_settings->adc_max - 1U);
}

status_t mcu_svc_voltage_to_percent(const mcu_analog_config_t *channel_config, float voltage, float *percentage)
{
    float percent;

    if (channel_config->settings.positive_slope)
    {
        float range = channel_config->settings.active_max_v - channel_config->settings.active_min_v;
        if (range <= 0.0f)
        {
            return ERROR_INVALID_PARAM;
        }
        percent = ((voltage - channel_config->settings.active_min_v) / range) * 100.0f;
    }
    else
    {
        float range = channel_config->settings.active_min_v - channel_config->settings.active_max_v;
        if (range <= 0.0f)
        {
            return ERROR_INVALID_PARAM;
        }
        percent = ((channel_config->settings.active_min_v - voltage) / range) * 100.0f;
    }

    *percentage = clamp_float(percent, 0.0f, 100.0f);
    return OK;
}
