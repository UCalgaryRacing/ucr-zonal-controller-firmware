#ifndef DATA_ACU_DATA_H_
#define DATA_ACU_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"

/*============================================================================*/
/* Accumulator LV Data Structure                                              */
/*============================================================================*/

typedef struct
{
    float battery_current;
    float battery_voltage;
    float battery_power;
    float ts_voltage;
    bool ts_active;

} acu_aculv_data_t;

typedef struct
{
    float sdc_mid1;
    float sdc_mid2;
    float sdc7;
    float sdc8;
    float sdc_end;
    float sdc_reserve;
} acu_sdc_voltages_t;

typedef struct
{
    bool bms_fault;
    bool imd_fault;
    uint32_t last_acu_msg;
} acu_fault_t;

#define ACU_SDC_RESERVE_MIN_VOLTAGE_V (9.0f)

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t acu_data_init(void);
status_t acu_fault_init(void);

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

status_t acu_data_set_aculv(const acu_aculv_data_t *aculv_data);
status_t acu_data_set_sdc_voltages(const acu_sdc_voltages_t *sdc_voltages);
status_t acu_data_set_fault_status(const acu_fault_t *acu_fault_status);

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

void acu_data_get_fault_status(acu_fault_t *acu_fault_status);

float acu_data_get_aculv_battery_voltage(void);
float acu_data_get_aculv_battery_current(void);
float acu_data_get_aculv_battery_power(void);
float acu_data_get_aculv_ts_voltage(void);
bool acu_data_get_aculv_ts_active(void);

float acu_data_get_sdc_reserve_voltage(void);

bool acu_data_get_bms_fault_active(void);
bool acu_data_get_imd_fault_active(void);
uint32_t acu_data_get_last_acu_msg(void);


#endif /* DATA_ACU_DATA_H_ */
