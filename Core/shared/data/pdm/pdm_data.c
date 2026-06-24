#include "pdm_data.h"
#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"
#include <string.h>

static pdm_full_system_data_t g_pdm_data;

static osMutexId_t g_pdm_mutex;

static const osMutexAttr_t g_pdm_mutex_attr =
{
    .name = "pdm_data_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit,
    .cb_mem = NULL,
    .cb_size = 0U,
};

/*============================================================================*/
/* Private Helpers                                                            */
/*============================================================================*/

static inline void lock(void)
{
    osMutexAcquire(g_pdm_mutex, osWaitForever);
}

static inline void unlock(void)
{
    osMutexRelease(g_pdm_mutex);
}

static inline bool is_valid_channel(pdm_channel_id_t channel_id)
{
    return (channel_id < PDM_TOTAL_CHANNELS);
}

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

void pdm_data_init(void)
{
    g_pdm_mutex = osMutexNew(&g_pdm_mutex_attr);
    memset(&g_pdm_data, 0, sizeof(g_pdm_data));
    g_pdm_data.num_channels = PDM_TOTAL_CHANNELS;

    for (uint8_t i = 0; i < PDM_TOTAL_CHANNELS; i++)
    {
        g_pdm_data.channels[i].state = PDM_CHANNEL_STATE_OFF;
        g_pdm_data.channels[i].fault.fault_type = PDM_CHANNEL_FAULT_NONE;
        g_pdm_data.channels[i].fault.fault_active = false;
    }
}

/*============================================================================*/
/* Channel Data Setters                                                       */
/*============================================================================*/

void pdm_data_set_channel_measurements(pdm_channel_id_t channel_id,
                                       const pdm_electrical_measurements_t *measurements)
{
    if (!is_valid_channel(channel_id) || measurements == NULL)
    {
        return;
    }

    lock();
    memcpy(&g_pdm_data.channels[channel_id].measurements, measurements,
           sizeof(*measurements));
    unlock();
}

void pdm_data_set_channel_state(pdm_channel_id_t channel_id, pdm_channel_state_t state)
{
    if (!is_valid_channel(channel_id))
    {
        return;
    }

    lock();
    g_pdm_data.channels[channel_id].state = state;
    unlock();
}

void pdm_data_set_channel_fault(pdm_channel_id_t channel_id, const pdm_channel_fault_t *fault)
{
    if (!is_valid_channel(channel_id) || fault == NULL)
    {
        return;
    }

    lock();
    memcpy(&g_pdm_data.channels[channel_id].fault, fault, sizeof(*fault));
    unlock();
}

void pdm_data_set_channel_voltage(pdm_channel_id_t channel_id, float voltage_v)
{
    if (!is_valid_channel(channel_id))
    {
        return;
    }

    lock();
    g_pdm_data.channels[channel_id].measurements.voltage.voltage_v = voltage_v;
    unlock();
}

void pdm_data_set_channel_current(pdm_channel_id_t channel_id, float current_a)
{
    if (!is_valid_channel(channel_id))
    {
        return;
    }

    lock();
    g_pdm_data.channels[channel_id].measurements.current.current_a = current_a;

    unlock();
}

void pdm_data_set_channel_power(pdm_channel_id_t channel_id, float power_w)
{
    if (!is_valid_channel(channel_id))
    {
        return;
    }

    lock();
    g_pdm_data.channels[channel_id].measurements.power.power_w = power_w;

    unlock();
}

/*============================================================================*/
/* Channel Data Getters                                                       */
/*============================================================================*/

bool pdm_data_get_channel(pdm_channel_id_t channel_id, pdm_channel_data_t *out)
{
    if (!is_valid_channel(channel_id) || out == NULL)
    {
        return false;
    }

    lock();
    memcpy(out, &g_pdm_data.channels[channel_id], sizeof(*out));
    unlock();

    return true;
}

bool pdm_data_get_channel_measurements(pdm_channel_id_t channel_id,
                                       pdm_electrical_measurements_t *out)
{
    if (!is_valid_channel(channel_id) || out == NULL)
    {
        return false;
    }

    lock();
    memcpy(out, &g_pdm_data.channels[channel_id].measurements, sizeof(*out));
    unlock();

    return true;
}

pdm_channel_state_t pdm_data_get_channel_state(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel(channel_id))
    {
        return PDM_CHANNEL_STATE_OFF;
    }

    lock();
    pdm_channel_state_t state = g_pdm_data.channels[channel_id].state;
    unlock();

    return state;
}

bool pdm_data_get_channel_fault(pdm_channel_id_t channel_id, pdm_channel_fault_t *out)
{
    if (!is_valid_channel(channel_id) || out == NULL)
    {
        return false;
    }

    lock();
    memcpy(out, &g_pdm_data.channels[channel_id].fault, sizeof(*out));
    unlock();

    return true;
}

float pdm_data_get_channel_voltage(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel(channel_id))
    {
        return 0.0f;
    }

    lock();
    float val = g_pdm_data.channels[channel_id].measurements.voltage.voltage_v;
    unlock();

    return val;
}

float pdm_data_get_channel_current(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel(channel_id))
    {
        return 0.0f;
    }

    lock();
    float val = g_pdm_data.channels[channel_id].measurements.current.current_a;
    unlock();

    return val;
}

float pdm_data_get_channel_power(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel(channel_id))
    {
        return 0.0f;
    }

    lock();
    float val = g_pdm_data.channels[channel_id].measurements.power.power_w;
    unlock();

    return val;
}

bool pdm_data_is_channel_fault_active(pdm_channel_id_t channel_id)
{
    if (!is_valid_channel(channel_id))
    {
        return false;
    }

    lock();
    bool active = g_pdm_data.channels[channel_id].fault.fault_active;
    unlock();

    return active;
}

/*============================================================================*/
/* GLV Bus Data Setters                                                       */
/*============================================================================*/

void pdm_data_set_glv_measurements(const pdm_electrical_measurements_t *measurements)
{
    if (measurements == NULL)
    {
        return;
    }

    lock();
    memcpy(&g_pdm_data.glv.measurements, measurements, sizeof(*measurements));
    unlock();
}

void pdm_data_set_glv_voltage(float voltage_v)
{
    lock();
    g_pdm_data.glv.measurements.voltage.voltage_v = voltage_v;
    unlock();
}

void pdm_data_set_glv_current(float current_a)
{
    lock();
    g_pdm_data.glv.measurements.current.current_a = current_a;
    unlock();
}

void pdm_data_set_glv_power(float power_w)
{
    lock();
    g_pdm_data.glv.measurements.power.power_w = power_w;
    unlock();
}

void pdm_data_set_glv_charge(float charge_ah)
{
    lock();
    g_pdm_data.glv.charge_ah = charge_ah;
    unlock();
}

void pdm_data_set_glv_soc(float soc_percent,float soe_percent, float energy_wh, float charge_ah)
{
    lock();
    g_pdm_data.glv.soc_percent = soc_percent;
    g_pdm_data.glv.soe_percent = soe_percent;
    g_pdm_data.glv.energy_wh = energy_wh;
    g_pdm_data.glv.charge_ah = charge_ah;
    unlock();
}

void pdm_data_set_glv_lowest_cell_voltage(float voltage_v)
{
    lock();
    g_pdm_data.glv.lowest_cell_voltage_v = voltage_v;
    unlock();
}

void pdm_data_set_glv_highest_cell_temperature(float temperature_c)
{
    lock();
    g_pdm_data.glv.highest_cell_temperature_c = temperature_c;
    unlock();
}

void pdm_data_set_glv_undervoltage_fault(bool fault_active)
{
    lock();
    g_pdm_data.glv.undervoltage_fault = fault_active;
    unlock();
}
void pdm_data_set_glv_overtemperature_fault(bool fault_active)
{
    lock();
    g_pdm_data.glv.overtemperature_fault = fault_active;
    unlock();
}
/*============================================================================*/
/* GLV Bus Data Getters                                                       */
/*============================================================================*/

void pdm_data_get_glv(pdm_glv_data_t *out)
{
    if (out == NULL)
    {
        return;
    }

    lock();
    memcpy(out, &g_pdm_data.glv, sizeof(*out));
    unlock();
}

void pdm_data_get_glv_measurements(pdm_electrical_measurements_t *out)
{
    if (out == NULL)
    {
        return;
    }

    lock();
    memcpy(out, &g_pdm_data.glv.measurements, sizeof(*out));
    unlock();
}

float pdm_data_get_glv_voltage(void)
{
    lock();
    float val = g_pdm_data.glv.measurements.voltage.voltage_v;
    unlock();

    return val;
}

float pdm_data_get_glv_current(void)
{
    lock();
    float val = g_pdm_data.glv.measurements.current.current_a;
    unlock();

    return val;
}

float pdm_data_get_glv_power(void)
{
    lock();
    float val = g_pdm_data.glv.measurements.power.power_w;
    unlock();

    return val;
}

float pdm_data_get_glv_soc_percent(void)
{
    lock();
    float val = g_pdm_data.glv.soc_percent;
    unlock();

    return val;
}

float pdm_data_get_glv_soe_percent(void)
{
    lock();
    float val = g_pdm_data.glv.soe_percent;
    unlock();

    return val;
}

float pdm_data_get_glv_energy_wh(void)
{
    lock();
    float val = g_pdm_data.glv.energy_wh;
    unlock();

    return val;
}

float pdm_data_get_glv_charge_ah(void)
{
    lock();
    float val = g_pdm_data.glv.charge_ah;
    unlock();

    return val;
}

bool pdm_data_get_glv_overtemperature_fault(void)
{
    lock();
    bool val = g_pdm_data.glv.overtemperature_fault;
    unlock();

    return val;
}

bool pdm_data_get_glv_undervoltage_fault(void)
{
    lock();
    bool val = g_pdm_data.glv.undervoltage_fault;
    unlock();

    return val;
}

/*============================================================================*/
/* Aggregate Data Access                                                      */
/*============================================================================*/

void pdm_data_get_all(pdm_full_system_data_t *out)
{
    if (out == NULL)
    {
        return;
    }

    lock();
    memcpy(out, &g_pdm_data, sizeof(*out));
    unlock();
}
