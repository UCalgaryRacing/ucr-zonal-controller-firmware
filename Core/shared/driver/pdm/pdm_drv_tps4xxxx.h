#ifndef DRIVERS_PDM_DRV_TPS4XXXX_H_
#define DRIVERS_PDM_DRV_TPS4XXXX_H_

#include <stdint.h>
#include <stdbool.h>
#include "pdm_config.h"

/*============================================================================*/
/* IC Type-Based Parameter Getters                                            */
/*============================================================================*/

static inline uint16_t tps4xxxx_get_k_cs(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_K_CS : TPS4H000_K_CS;
}

static inline uint16_t tps4xxxx_get_k_cl(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_K_CL : TPS4H000_K_CL;
}

static inline uint16_t tps4xxxx_get_ron_mohm(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_RON_MOHM : TPS4H000_RON_MOHM;
}

static inline float tps4xxxx_get_icl_int_a(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_ICL_INT_A : TPS4H000_ICL_INT_A;
}

static inline uint16_t tps4xxxx_get_r_cs(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_CS_RESISTOR_OHMS : TPS4H000_CS_RESISTOR_OHMS;
}

static inline uint16_t tps4xxxx_get_r_cl(pdm_channel_type_t ic_type)
{
    return (ic_type == PDM_CHANNEL_TYPE_TPS4H160) ? TPS4H160_CL_RESISTOR_OHMS : TPS4H000_CL_RESISTOR_OHMS;
}

/*============================================================================*/
/* Driver Initialization                                                      */
/*============================================================================*/

int tps4xxxx_driver_init(void);
int tps4xxxx_device_init(const tps4xxxx_hw_t *hw, const tps4xxxx_config_t *settings);
int tps4xxxx_enable_channel(const tps4xxxx_hw_t *hw, bool enable);
int tps4xxxx_enable_all_channels(pdm_channel_id_t device_id);
int tps4xxxx_disable_all_channels(pdm_channel_id_t device_id);

/*============================================================================*/
/* Diagnostic Configuration                                                   */
/*============================================================================*/

int tps4xxxx_set_diag_enable(pdm_channel_id_t device_id, bool enable);

/*============================================================================*/
/* Fault Status Reading                                                       */
/*============================================================================*/

bool tps4xxxx_read_fault(const tps4xxxx_hw_t *hw);
int tps4xxxx_read_current_sense_adc(ADC_HandleTypeDef *adc_handle, uint16_t *adc_value);
int tps4xxxx_read_channel_current(const tps4xxxx_hw_t *hw, float ratio, float *current_a);
float tps4xxxx_adc_to_current(uint16_t adc_value, float r_cs);

#endif /* DRIVERS_PDM_DRV_TPS4XXXX_H_ */
