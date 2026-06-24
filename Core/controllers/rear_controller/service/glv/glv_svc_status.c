#include "glv_svc_status.h"

#include "glv_svc_thermistor.h"
#include "mcu_config.h"
#include "mcu_svc_analog.h"
#include "pdm_svc_channel.h"
#include "pdm_data.h"
#include "glv_config.h"

#define GLV_CELL_CAPACITY_AH                        3.0f
#define GLV_PACK_SERIES_CELL_COUNT                  4.0f
#define GLV_PACK_PARALLEL_CELL_COUNT                10.0f
#define GLV_PACK_VOLTAGE_FULLY_CHARGED_V            4.2f*(GLV_PACK_SERIES_CELL_COUNT)
#define GLV_PACK_VOLTAGE_EMPTY_V                    2.5f*(GLV_PACK_SERIES_CELL_COUNT)
#define GLV_PACK_CAPACITY_AH                        (GLV_CELL_CAPACITY_AH * GLV_PACK_PARALLEL_CELL_COUNT)
#define GLV_PACK_ENERGY_WH                          (GLV_PACK_CAPACITY_AH * GLV_PACK_VOLTAGE_FULLY_CHARGED_V)

static float initial_soc = 100.0f; // Start at 100% SOC, will be updated on first run of task_fast
static float initial_soe = 100.0f; // Start at 100% SOE, will be updated on first run of task_fast
static float initial_energy_wh = GLV_PACK_ENERGY_WH;
static float initial_capacity_ah = GLV_PACK_CAPACITY_AH;


/* function that splits the curve into multiple straight line segments using nested if statements*/
#define VOLTAGE_TO_SOC_1C(v) \
    ((v) >= 4.15f ? 100.0f : \
     (v) >= 3.95f ? 80.0f + ((v) - 3.95f) * (20.0f / (4.15f - 3.95f)) : \
     (v) >= 3.70f ? 55.0f + ((v) - 3.70f) * (25.0f / (3.95f - 3.70f)) : \
     (v) >= 3.50f ? 30.0f + ((v) - 3.50f) * (25.0f / (3.70f - 3.50f)) : \
     (v) >= 3.25f ? 10.0f + ((v) - 3.25f) * (20.0f / (3.50f - 3.25f)) : \
     (v) >= 2.50f ?  0.0f + ((v) - 2.50f) * (10.0f / (3.25f - 2.50f)) : 0.0f)


/* function that fits a polynomial to the curve */

#define VOLTAGE_TO_SOC_1C_POLY(v) \
    ((v) >= 4.15f ? 100.0f : \
     (v) <= 2.50f ? 0.0f : \
     (-116.716f * (v)*(v)*(v)*(v)*(v) + \
       1932.915f * (v)*(v)*(v)*(v) - \
      12658.625f * (v)*(v)*(v) + \
      40846.852f * (v)*(v) - \
      64949.773f * (v) + \
      40733.450f))


static inline const mcu_analog_config_t* glv_get_config(mcu_analog_channel_id_t channel_id)
{
    return mcu_svc_get_channel_config(channel_id);
}

static inline void glv_read_sensor(mcu_analog_channel_id_t channel_id, float *voltage)
{
    mcu_svc_get_analog_voltage(channel_id, voltage);
}

float glv_svc_analog_update(mcu_analog_channel_id_t glv_channel_id)
{
    const mcu_analog_config_t *glv_config = glv_get_config(glv_channel_id);

    if (glv_config != NULL)
    {
        float voltage = 0.0f;
        glv_read_sensor(glv_channel_id, &voltage);

        if (glv_channel_id == BATT_HIGHEST_TEMP)
        {
            return glv_svc_convert_voltage_to_temperature(voltage);
        }

        return voltage;
    }

    return 0.0f;
}

void glv_svc_disable_glv(void)
{
    pdm_svc_channel_disable_all();
}


void glv_svc_update_glv_status(void){
    //TODO implement this function to update any status related to glv, such as checking if sensors are active, if readings are valid,
    // make it such that this function updates the global pdm full system data strcut in the pdm data layer, then make anothor function that
    // that grabs those fault status from the data layer and then turns off the glv if it detects a fault.

    // grab the 2 configs from the config layer so that we can check with the allowable values for the sensosrs

    bool undervoltage_fault = false;
    bool overtemperature_fault = false;

    const mcu_analog_config_t *highest_temp_config = glv_get_config(BATT_HIGHEST_TEMP);
    const mcu_analog_config_t *lowest_voltage_config = glv_get_config(BATT_LOWEST_VOLTAGE);


    /*                                                  DATA UPDATE                                         */

    // use the newest values to update the data layer with the values of the sensors
    float highest_temp_c = glv_svc_analog_update(BATT_HIGHEST_TEMP);
    float lowest_voltage_v = glv_svc_analog_update(BATT_LOWEST_VOLTAGE);

    // update values in the pdm data layer.
    pdm_data_set_glv_highest_cell_temperature(highest_temp_c);
    pdm_data_set_glv_lowest_cell_voltage(lowest_voltage_v);


    

    if (highest_temp_c > GLV_CELL_MAXIMUM_TEMPERATURE_C)
    {
        overtemperature_fault = true;
    }

    if (lowest_voltage_v < lowest_voltage_config->settings.active_min_v)
    {
        undervoltage_fault = true;
    }

    // update the fault status in the pdm data layer

    pdm_data_set_glv_overtemperature_fault(overtemperature_fault);
    pdm_data_set_glv_undervoltage_fault(undervoltage_fault);

}


/*                                                  FAULT CHECKING                                      */
void glv_svc_apply_glv_plausability(void){
/* TODO make it such that this function checks the fault parameters from the data layer
 and then turns off the glv if the fault status is set.*/

    bool undervoltage_fault = pdm_data_get_glv_undervoltage_fault();
    bool overtemperature_fault = pdm_data_get_glv_overtemperature_fault();
    
    if (undervoltage_fault || overtemperature_fault)
    {
        glv_svc_disable_glv();
    } 
    
    //glv_svc_disable_glv(); // for testing if the pin goes high or not.

        
}


/*                                                  INITIALIZE SOC                                      */
/* function needs to grab the voltage of the GLV bus and calculate the state of charge                  */
/* this function should be called during the 1st initialization function, so propably fast task init    */
void glv_svc_init_soc(void){

	float glv_bus_voltage;
    status_t voltage_status = pdm_svc_read_glv_voltage(&glv_bus_voltage);
    float soc = VOLTAGE_TO_SOC_1C((glv_bus_voltage/GLV_PACK_SERIES_CELL_COUNT)); // convert bus voltage to cell voltage for SOC calculation

    //float soc = VOLTAGE_TO_SOC_1C_POLY((glv_bus_voltage/GLV_PACK_SERIES_CELL_COUNT)); // convert bus voltage to cell voltage for SOC calculation using polynomial fit

    float Capacity_Ah = GLV_PACK_CAPACITY_AH * (soc / 100.0f);
    float Energy_Wh = Capacity_Ah * glv_bus_voltage; 
    float soe = (Energy_Wh / GLV_PACK_ENERGY_WH) * 100.0f;

    /*set the initial values in this file, these will be used since the INA gives you the time intregal of charge and energy, no need to perform time intregal ourselves*/

    initial_soc = soc;
    initial_soe = soe;
    initial_energy_wh = Energy_Wh;
    initial_capacity_ah = Capacity_Ah;

    pdm_data_set_glv_soc(soc, soe, Energy_Wh, Capacity_Ah);


}


/*                                                  UPDATE SOC                                                                   */
/* function reads the values for the accumelated charge and energy spent form the INA229 and subtracts it from the initial values*/
/* function then updates the data layer                                                                                          */
void glv_svc_update_soc(void){

    /* read the INA229 registers and calculate the values accordingly*/
	float capacity_discharged_Ah;
    pdm_svc_read_glv_charge(&capacity_discharged_Ah);
    float remaining_capacity_Ah = initial_capacity_ah - capacity_discharged_Ah;
    float current_soc = (remaining_capacity_Ah / GLV_PACK_CAPACITY_AH) * 100.0f;

    float energy_discharged_Wh;
    pdm_svc_read_glv_energy(&energy_discharged_Wh);
    float remaining_energy_wh = initial_energy_wh - energy_discharged_Wh;
    float current_soe = (remaining_energy_wh / GLV_PACK_ENERGY_WH) * 100.0f;
    

    pdm_data_set_glv_soc(current_soc, current_soe, remaining_energy_wh, remaining_capacity_Ah);

}
