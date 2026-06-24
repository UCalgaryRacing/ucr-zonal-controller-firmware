#include "pdm_svc_channel.h"
#include "pdm_config.h"
#include "pdm_drv_vnf9q20f.h"
#include "pdm_drv_tps4xxxx.h"
#include "pdm_data.h"
#include "pdm_drv_lm5060.h"
#include "com_typ_common.h"

//make this match the default config in initialization functions, then can update at runtime
//should this be defined in header or static in source file?
pdm_channel_runtime_config_t pdm_runtime_config[PDM_TOTAL_CHANNELS];

/*============================================================================*/
/* Private Helpers                                                            */
/*============================================================================*/

static inline bool is_valid_channel_id(pdm_channel_id_t channel_id)
{
    return (channel_id < PDM_TOTAL_CHANNELS);
}

static inline const pdm_channel_config_t* get_channel_config(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel_id(channel_id))
    {
        return NULL;
    }
    return &pdm_default_config[channel_id];
}

/*============================================================================*/
/* Service Initialization                                                     */
/*============================================================================*/

static bool vnf9q20f_device_initialized[VNF9Q20F_NUM_DEVICES] = {false};

status_t pdm_svc_channel_init(pdm_channel_id_t channel_id)
{
    const pdm_channel_config_t *channel_config = get_channel_config(channel_id);
    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    switch (channel_config->ic_type)
    {
        case PDM_CHANNEL_TYPE_VNF9Q20F:
        {
            if (vnf9q20f_driver_init() != 0)
            {
                return ERROR_GENERAL;
            }

            uint8_t ic = channel_config->hw.vnf9q20f.ic;
            if (!vnf9q20f_device_initialized[ic])
            {
                if (vnf9q20f_device_init(&channel_config->hw.vnf9q20f, true) != 0)
                {
                    return ERROR_GENERAL;
                }
                vnf9q20f_device_initialized[ic] = true;
            }
            break;
        }

        case PDM_CHANNEL_TYPE_TPS4H160:
        case PDM_CHANNEL_TYPE_TPS4H000:
        {
            if (tps4xxxx_driver_init() != 0)
            {
                return ERROR_GENERAL;
            }

            if (tps4xxxx_device_init(&channel_config->hw.tps4xxxx, &channel_config->device_settings.tps4xxxx_config) != 0)
            {
                return ERROR_GENERAL;
            }
            break;
        }

        default:
            return ERROR_NOT_SUPPORTED;
    }

    return OK;
}

/*============================================================================*/
/* Channel Enable/Disable                                                     */
/*============================================================================*/

status_t pdm_svc_channel_enable(pdm_channel_id_t channel_id, bool enable)
{
    const pdm_channel_config_t *channel_config = get_channel_config(channel_id);
    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    int result;

    switch (channel_config->ic_type)
    {
        case PDM_CHANNEL_TYPE_VNF9Q20F:
            result = vnf9q20f_enable_channel(&channel_config->hw.vnf9q20f, enable);
            break;

        case PDM_CHANNEL_TYPE_TPS4H160:
        case PDM_CHANNEL_TYPE_TPS4H000:
            result = tps4xxxx_enable_channel(&channel_config->hw.tps4xxxx, enable);
            break;

        default:
            return ERROR_NOT_SUPPORTED;
    }

    pdm_data_set_channel_state(channel_id, (result == 0) ? PDM_CHANNEL_STATE_ON : PDM_CHANNEL_STATE_OFF);

    return (result == 0) ? OK : ERROR_COMMUNICATION;
}

//update vnf9q20f watchdog
status_t pdm_svc_channel_update(pdm_channel_id_t channel_id)
{
    const pdm_channel_config_t *channel_config = get_channel_config(channel_id);
    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    //TODO maybe dont fail with error if called from tps4h channel, just return silently?
    if(channel_config->ic_type != PDM_CHANNEL_TYPE_VNF9Q20F)
    {
        return ERROR_INVALID_PARAM;
    }

    vnf9q20f_toggle_watchdog(&channel_config->hw.vnf9q20f);

    return OK;
}

status_t pdm_svc_channel_disable_all(void){
    //TODO add alternate definition to this function that simply goes thorugh a while loop and disables all channels sequentially.
    lm5060_disable_pdm(true);
    return OK;

}

/*                                                      ALTERNATE DEFINITION                                    */

// status_t pdm_svc_channel_disable_all(void){

//     int index = 0;

//     while (index < PDM_TOTAL_CHANNELS){
//         pdm_svc_channel_enable(index, false);
//         index++;
//     }
//     return OK;
// }

/*============================================================================*/
/* PWM Control                                                                */
/*============================================================================*/

status_t pdm_svc_channel_set_duty(pdm_channel_id_t channel_id, uint8_t duty_percent)
{
    const pdm_channel_config_t *channel_config = get_channel_config(channel_id);
    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    if (duty_percent > 100)
    {
        duty_percent = 100;
    }

    int result;

    switch (channel_config->ic_type)
    {
        case PDM_CHANNEL_TYPE_VNF9Q20F:
            result = vnf9q20f_set_duty_cycle_percent(&channel_config->hw.vnf9q20f, duty_percent);
            break;

        case PDM_CHANNEL_TYPE_TPS4H160:
        case PDM_CHANNEL_TYPE_TPS4H000:
            result = ERROR_INVALID_PARAM;
            break;

        default:
            return ERROR_NOT_SUPPORTED;
    }

    return (result == 0) ? OK : ERROR_COMMUNICATION;
}

status_t pdm_svc_glv_readings_init(void){
    PDM_INA229_Init();
    return OK;
}

status_t pdm_svc_read_glv_voltage(float *voltage_v){

    if (voltage_v == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_voltage(voltage_v);

    return OK;
}

status_t pdm_svc_read_glv_current(float *current_a){

    if (current_a == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_current(current_a);

    return OK;
}

status_t pdm_svc_read_glv_power(float *power_w){

    if (power_w == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_power(power_w);

    return OK;
}


status_t pdm_svc_read_pdm_temperature(float *temperature_c){

    if (temperature_c == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_temperature(temperature_c);

    return OK;
}

status_t pdm_svc_read_glv_charge(float *charge_c){

    if (charge_c == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_charge(charge_c);

    return OK;
}

status_t pdm_svc_read_glv_energy(float *energy_wh){

    if (energy_wh == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    PDM_INA229_update_glv_energy(energy_wh);

    return OK;
}

status_t pdm_svc_update_glv_data(void){

    float voltage, current, power;
    status_t voltage_status = pdm_svc_read_glv_voltage(&voltage);
    status_t current_status = pdm_svc_read_glv_current(&current);
    status_t power_status = pdm_svc_read_glv_power(&power);

    if (voltage_status != OK || current_status != OK || power_status != OK){
        return ERROR_COMMUNICATION;
    }

    pdm_data_set_glv_voltage(voltage);
    pdm_data_set_glv_current(current);
    pdm_data_set_glv_power(power);

    return OK;
}

// uint8_t pdm_svc_channel_get_duty(pdm_channel_id_t channel_id)
// {
//     const pdm_channel_config_t *config = get_channel_config(channel_id);
//     if (config == NULL)
//     {
//         return 0;
//     }

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//             return vnf9q20f_get_duty_cycle_percent(config->hw.ic_instance,
//                                                     config->hw.ic_channel);

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
// #if TPS4XXXX_TOTAL_CHANNELS > 0
//             /* TPS4xxxx has no hardware PWM; return 100 if enabled, 0 if disabled */
//             return tps4xxxx_get_channel_enable(config->hw.ic_instance,
//                                                config->hw.ic_channel) ? 100U : 0U;
// #else
//             return 0;
// #endif

//         default:
//             return 0;
//     }
// }

// /*============================================================================*/
// /* Channel Mode Configuration                                                 */
// /*============================================================================*/

// status_t pdm_svc_channel_set_mode(pdm_channel_id_t channel_id, pdm_channel_mode_t mode)
// {
//     const pdm_channel_config_t *config = get_channel_config(channel_id);
//     if (config == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     int result;

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//         {
//             vnf9q20f_mode_t vnf_mode = (mode == PDM_CHANNEL_MODE_RESISTIVE) 
//                                         ? VNF9Q20F_MODE_BULB 
//                                         : VNF9Q20F_MODE_LED;
//             result = vnf9q20f_set_channel_mode(config->hw.ic_instance,
//                                                 config->hw.ic_channel,
//                                                 vnf_mode);
//             break;
//         }

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
//             result = -1;
//             break;

//         default:
//             return ERROR_NOT_SUPPORTED;
//     }

//     return (result == 0) ? OK : ERROR_COMMUNICATION;
// }

// /*============================================================================*/
// /* Current and Power Monitoring                                               */
// /*============================================================================*/

// status_t pdm_svc_channel_read_current(pdm_channel_id_t channel_id, float *current_a)
// {
//     if (current_a == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     const pdm_channel_config_t *config = get_channel_config(channel_id);
//     if (config == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     uint16_t adc_value;
//     int result;

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//             result = vnf9q20f_read_current_sense(config->hw.ic_instance,
//                                                   config->hw.ic_channel,
//                                                   &adc_value);
//             if (result == 0)
//             {
//                 *current_a = vnf9q20f_adc_to_current(config->hw.ic_instance,
//                                                       config->hw.ic_channel,
//                                                       adc_value);
//             }
//             break;

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
// #if TPS4XXXX_TOTAL_CHANNELS > 0
//             result = tps4xxxx_read_channel_current(config->hw.ic_type,
//                                                     config->hw.ic_instance,
//                                                     config->hw.ic_channel,
//                                                     current_a);
// #else
//             result = -1;
// #endif
//             break;

//         default:
//             return ERROR_NOT_SUPPORTED;
//     }

//     return (result == 0) ? OK : ERROR_COMMUNICATION;
// }

// /*============================================================================*/
// /* Fault Management                                                           */
// /*============================================================================*/

// status_t pdm_svc_channel_get_faults(pdm_channel_id_t channel_id, pdm_channel_fault_t *fault)
// {
//     if (fault == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     const pdm_channel_config_t *config = get_channel_config(channel_id);
//     if (config == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     int result;

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//         {
//             result = vnf9q20f_read_channel_faults(config->hw.ic_instance,
//                                                    config->hw.ic_channel);
//             if (result == 0)
//             {
//                 const vnf9q20f_device_state_t *state = vnf9q20f_get_state(config->hw.ic_instance);
//                 if (state != NULL)
//                 {
//                     const vnf9q20f_channel_info_t *info = &state->channels[config->hw.ic_channel];
//                     fault->fault_type = map_vnf9q20f_fault(info->fault_type);
//                     fault->fault_active = info->fault_active;
//                     fault->fault_count = info->fault_count;
//                     fault->fault_timestamp_ms = info->fault_timestamp_ms;
//                 }
//             }
//             break;
//         }

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
// #if TPS4XXXX_TOTAL_CHANNELS > 0
//         {
//             tps4xxxx_channel_info_t info;
//             result = tps4xxxx_read_channel_info(config->hw.ic_type,
//                                                  config->hw.ic_instance,
//                                                  config->hw.ic_channel,
//                                                  &info);
//             if (result == 0)
//             {
//                 fault->fault_type = map_tps4xxxx_fault(info.fault_type);
//                 fault->fault_active = info.fault_active;
//                 fault->fault_count = info.fault_active ? 1U : 0U;
//                 fault->fault_timestamp_ms = info.fault_timestamp_ms;
//             }
//         }
// #else
//             fault->fault_type = PDM_CHANNEL_FAULT_NONE;
//             fault->fault_active = false;
//             fault->fault_count = 0;
//             fault->fault_timestamp_ms = 0;
//             result = -1;
// #endif
//             break;

//         default:
//             return ERROR_NOT_SUPPORTED;
//     }

//     return (result == 0) ? OK : ERROR_COMMUNICATION;
// }

// status_t pdm_svc_channel_clear_faults(pdm_channel_id_t channel_id)
// {
//     const pdm_channel_config_t *config = get_channel_config(channel_id);
//     if (config == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     int result;

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//             result = vnf9q20f_clear_channel_faults(config->hw.ic_instance,
//                                                     config->hw.ic_channel);
//             break;

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
// #if TPS4XXXX_TOTAL_CHANNELS > 0
//             /* TPS4xxxx FAULT pin clears when fault condition is removed; no software clear */
//             result = 0;
// #else
//             result = -1;
// #endif
//             break;

//         default:
//             return ERROR_NOT_SUPPORTED;
//     }

//     return (result == 0) ? OK : ERROR_COMMUNICATION;
// }

// bool pdm_svc_channel_any_fault(void)
// {
//     for (uint8_t i = 0; i < PDM_TOTAL_CHANNELS; i++)
//     {
//         pdm_channel_fault_t fault;
//         if (pdm_svc_channel_get_faults((pdm_channel_id_t)i, &fault) == OK)
//         {
//             if (fault.fault_active)
//             {
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// /*============================================================================*/
// /* Configuration Application                                                  */
// /*============================================================================*/

// status_t pdm_svc_channel_apply_config(pdm_channel_id_t channel_id,
//                                            const pdm_channel_config_t *config)
// {
//     if (config == NULL)
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     if (!is_valid_channel_id(channel_id))
//     {
//         return ERROR_INVALID_PARAM;
//     }

//     status_t result = OK;

//     switch (config->hw.ic_type)
//     {
//         case PDM_CHANNEL_TYPE_VNF9Q20F:
//         {
//             uint8_t dev = config->hw.ic_instance;
//             uint8_t ch = config->hw.ic_channel;

//             vnf9q20f_mode_t vnf_mode = (config->mode == PDM_CHANNEL_MODE_RESISTIVE)
//                                         ? VNF9Q20F_MODE_BULB
//                                         : VNF9Q20F_MODE_LED;
//             if (vnf9q20f_set_channel_mode(dev, ch, vnf_mode) != 0)
//             {
//                 result = ERROR_GENERAL;
//             }

//             if (vnf9q20f_set_duty_cycle_percent(dev, ch, config->pwm.duty_percent) != 0)
//             {
//                 result = ERROR_GENERAL;
//             }

//             if (vnf9q20f_set_i2t_protection(dev, ch, 
//                                              VNF9Q20F_DEFAULT_INOM, 
//                                              VNF9Q20F_DEFAULT_TNOM) != 0)
//             {
//                 result = ERROR_GENERAL;
//             }

//             if (config->enabled)
//             {
//                 if (vnf9q20f_set_channel_enable(dev, ch, true) != 0)
//                 {
//                     result = ERROR_GENERAL;
//                 }
//             }
//             break;
//         }

//         case PDM_CHANNEL_TYPE_TPS4H160:
//         case PDM_CHANNEL_TYPE_TPS4H000:
//             // TODO: Implement TPS4xxxx configuration application
//             result = ERROR_NOT_SUPPORTED;
//             break;

//         default:
//             result = ERROR_NOT_SUPPORTED;
//             break;
//     }

//     return result;
// }

// status_t pdm_svc_channel_apply_all_config(void)
// {
//     status_t result = OK;

//     for (uint8_t i = 0; i < PDM_TOTAL_CHANNELS; i++)
//     {
//         if (pdm_svc_channel_apply_config((pdm_channel_id_t)i, 
//                                           &pdm_default_config[i]) != OK)
//         {
//             result = ERROR_GENERAL;
//         }
//     }

//     return result;
// }
