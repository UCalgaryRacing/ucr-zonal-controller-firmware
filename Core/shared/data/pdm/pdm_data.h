#ifndef DATA_PDM_DAT_DATA_H_
#define DATA_PDM_DAT_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"
#include "pdm_config.h"
#include "pdm_app_state_manager.h"


typedef enum
{
    PDM_CHANNEL_STATE_OFF = 0,
    PDM_CHANNEL_STATE_ON,
    PDM_CHANNEL_STATE_FAULT
} pdm_channel_state_t;

typedef enum
{
    PDM_CHANNEL_FAULT_NONE = 0,
    PDM_CHANNEL_FAULT_OVERCURRENT,
    PDM_CHANNEL_FAULT_OVERTEMP,
    PDM_CHANNEL_FAULT_SHORT_TO_GND,
    PDM_CHANNEL_FAULT_OPEN_LOAD,
    PDM_CHANNEL_FAULT_SHORT_TO_VCC,
    PDM_CHANNEL_FAULT_UNDERVOLTAGE,
    PDM_CHANNEL_FAULT_OVERVOLTAGE,
    PDM_CHANNEL_FAULT_COMM_ERROR
} pdm_channel_fault_type_t;


/*============================================================================*/
/* Measurement Data Structures                                                */
/*============================================================================*/

typedef struct
{
    float voltage_v;
} pdm_voltage_measurement_t;

typedef struct
{
    float current_a;
} pdm_current_measurement_t;

typedef struct
{
    float power_w;
} pdm_power_measurement_t;

typedef struct
{
    pdm_voltage_measurement_t voltage;
    pdm_current_measurement_t current;
    pdm_power_measurement_t power;
} pdm_electrical_measurements_t;

/*============================================================================*/
/* Channel Runtime Data Structures                                            */
/*============================================================================*/

typedef struct
{
    pdm_channel_fault_type_t fault_type;
    bool fault_active;
    uint16_t fault_count;
    uint32_t fault_timestamp_ms;
} pdm_channel_fault_t;

typedef struct
{
    pdm_electrical_measurements_t measurements;
    pdm_channel_state_t state;
    pdm_channel_fault_t fault;
} pdm_channel_data_t;

/*============================================================================*/
/* GLV Bus Data Structures                                                    */
/*============================================================================*/

typedef struct
{
    pdm_electrical_measurements_t measurements;
    float energy_wh;
    float charge_ah;
    float soc_percent;
    float soe_percent;
    float lowest_cell_voltage_v;
    float highest_cell_temperature_c;
    bool undervoltage_fault;
    bool overtemperature_fault;
} pdm_glv_data_t;

typedef struct
{
    pdm_glv_data_t glv;
    pdm_channel_data_t channels[PDM_TOTAL_CHANNELS];
    uint8_t num_channels;
} pdm_full_system_data_t;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

void pdm_data_init(void);

/*============================================================================*/
/* Channel Data Setters (called by service layer)                             */
/*============================================================================*/

void pdm_data_set_channel_measurements(pdm_channel_id_t channel_id,
                                       const pdm_electrical_measurements_t *measurements);

void pdm_data_set_channel_state(pdm_channel_id_t channel_id, pdm_channel_state_t state);

void pdm_data_set_channel_fault(pdm_channel_id_t channel_id, const pdm_channel_fault_t *fault);

void pdm_data_set_channel_voltage(pdm_channel_id_t channel_id, float voltage_v);

void pdm_data_set_channel_current(pdm_channel_id_t channel_id, float current_a);

void pdm_data_set_channel_power(pdm_channel_id_t channel_id, float power_w);

/*============================================================================*/
/* Channel Data Getters (called by other tasks)                               */
/*============================================================================*/

bool pdm_data_get_channel(pdm_channel_id_t channel_id, pdm_channel_data_t *out);

bool pdm_data_get_channel_measurements(pdm_channel_id_t channel_id,
                                       pdm_electrical_measurements_t *out);

pdm_channel_state_t pdm_data_get_channel_state(pdm_channel_id_t channel_id);

bool pdm_data_get_channel_fault(pdm_channel_id_t channel_id, pdm_channel_fault_t *out);

float pdm_data_get_channel_voltage(pdm_channel_id_t channel_id);

float pdm_data_get_channel_current(pdm_channel_id_t channel_id);

float pdm_data_get_channel_power(pdm_channel_id_t channel_id);

bool pdm_data_is_channel_fault_active(pdm_channel_id_t channel_id);

/*============================================================================*/
/* GLV Bus Data Setters (called by service layer)                             */
/*============================================================================*/

void pdm_data_set_glv_measurements(const pdm_electrical_measurements_t *measurements);

void pdm_data_set_glv_voltage(float voltage_v);

void pdm_data_set_glv_current(float current_a);

void pdm_data_set_glv_power(float power_w);

void pdm_data_set_glv_charge(float charge_ah);

void pdm_data_set_glv_soc(float soc_percent,float soe_percent, float energy_wh, float charge_ah);
    
void pdm_data_set_glv_lowest_cell_voltage(float voltage_v);

void pdm_data_set_glv_highest_cell_temperature(float temperature_c);

void pdm_data_set_glv_undervoltage_fault(bool fault_active);

void pdm_data_set_glv_overtemperature_fault(bool fault_active);

/*============================================================================*/
/* GLV Bus Data Getters (called by other tasks)                               */
/*============================================================================*/

void pdm_data_get_glv(pdm_glv_data_t *out);

void pdm_data_get_glv_measurements(pdm_electrical_measurements_t *out);

float pdm_data_get_glv_voltage(void);

float pdm_data_get_glv_current(void);

float pdm_data_get_glv_power(void);

float pdm_data_get_glv_soc_percent(void);

float pdm_data_get_glv_soe_percent(void);

float pdm_data_get_glv_energy_wh(void);

float pdm_data_get_glv_charge_ah(void);

bool pdm_data_get_glv_overtemperature_fault(void);

bool pdm_data_get_glv_undervoltage_fault(void);

/*============================================================================*/
/* System Data Setters (called by service/application layer)                  */
/*============================================================================*/

void pdm_data_set_system_state(pdm_app_state_t state);

void pdm_data_set_system_enabled(bool enabled);

/*============================================================================*/
/* System Data Getters (called by other tasks)                                */
/*============================================================================*/

pdm_app_state_t pdm_data_get_system_state(void);

bool pdm_data_is_system_enabled(void);

/*============================================================================*/
/* Channel Statistics (called by service layer to update, others to read)     */
/*============================================================================*/

void pdm_data_inc_channel_switch_count(pdm_channel_id_t channel_id);

void pdm_data_inc_channel_fault_count(pdm_channel_id_t channel_id);

void pdm_data_update_channel_peak_current(pdm_channel_id_t channel_id, float current_a);

void pdm_data_add_channel_energy(pdm_channel_id_t channel_id, float energy_wh);

/*============================================================================*/
/* System Statistics (called by service layer to update, others to read)      */
/*============================================================================*/

void pdm_data_inc_system_fault_count(void);

void pdm_data_inc_system_state_change_count(void);

void pdm_data_inc_system_comm_error_count(void);

void pdm_data_update_system_peak_power(float power_w);

void pdm_data_add_system_energy(float energy_wh);

/*============================================================================*/
/* Aggregate Data Access                                                      */
/*============================================================================*/

void pdm_data_get_all(pdm_full_system_data_t *out);

#endif /* DATA_PDM_DAT_DATA_H_ */
