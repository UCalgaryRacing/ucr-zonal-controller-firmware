#ifndef RCO_DATA_H_
#define RCO_DATA_H_

#include <stdint.h>
#include <stdbool.h>
//#include "C:\Users\esall\OneDrive - University of Calgary\UCalgary Racing\UCR-03\Software\Controllers\UCR-03_Front_Controller_Dev\Core\shared\service\common\com_typ_common.h"
#include "com_typ_common.h"

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t rco_data_init(void);

/*============================================================================*/
/* RCO Data Struct                                                            */
/*============================================================================*/

typedef struct
{
    bool tssi_faulted;
    bool brake_light_on;
    bool drs_active;
    bool reset_pressed;
} rco_data_t;


typedef struct
{
    bool tractive_pump_1_enable;
    bool tractive_pump_2_enable;
    bool tractive_fan_1_enable;
    bool tractive_fan_2_enable;
    bool accumulator_fan_1_enable;
    bool accumulator_fan_2_enable;
    float tractive_fan_1_duty_cycle;
    float tractive_fan_2_duty_cycle;
    float accumulator_fan_1_duty_cycle;
    float accumulator_fan_2_duty_cycle;
} rear_cooling_control_data_t;

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

void rco_data_set_reset_pressed(bool reset_pressed);
void rco_data_set_brake_light(bool brake_light_on);
void rco_data_set_tssi_faulted(bool tssi_faulted);
void rco_data_set_drs(bool drs_active);
status_t rco_data_set_data(rco_data_t *rco_data);
void rco_data_set_drs_active(bool drs_active);

void rco_data_set_tractive_pump_1_enable(bool enable);
void rco_data_set_tractive_pump_2_enable(bool enable);
void rco_data_set_tractive_fan_1_enable(bool enable);
void rco_data_set_tractive_fan_2_enable(bool enable);
void rco_data_set_accumulator_fan_1_enable(bool enable);
void rco_data_set_accumulator_fan_2_enable(bool enable);
void rco_data_set_tractive_fan_1_duty_cycle(float duty_cycle);
void rco_data_set_tractive_fan_2_duty_cycle(float duty_cycle);
void rco_data_set_accumulator_fan_1_duty_cycle(float duty_cycle);
void rco_data_set_accumulator_fan_2_duty_cycle(float duty_cycle);
status_t rco_data_set_cooling_data(rear_cooling_control_data_t *cooling_data);

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

status_t rco_data_get_data(rco_data_t *rco_data);
bool rco_data_get_reset_pressed(void);
bool rco_data_get_drs_active();
bool rco_data_get_drs();

status_t rco_data_get_cooling_data(rear_cooling_control_data_t *cooling_data);
bool rco_data_get_tractive_pump_1_enable(void);
bool rco_data_get_tractive_pump_2_enable(void);
bool rco_data_get_tractive_fan_1_enable(void);
bool rco_data_get_tractive_fan_2_enable(void);
bool rco_data_get_accumulator_fan_1_enable(void);
bool rco_data_get_accumulator_fan_2_enable(void);
float rco_data_get_tractive_fan_1_duty_cycle(void);
float rco_data_get_tractive_fan_2_duty_cycle(void);
float rco_data_get_accumulator_fan_1_duty_cycle(void);
float rco_data_get_accumulator_fan_2_duty_cycle(void);

#endif /* RCO_DATA_H_ */
