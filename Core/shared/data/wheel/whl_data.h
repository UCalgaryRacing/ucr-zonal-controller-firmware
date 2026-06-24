#ifndef WHL_DATA_H
#define WHL_DATA_H

#include <stdint.h>
#include <stdbool.h>
#include "com_typ_common.h"

/*============================================================================*/
/* Wheel Data Structs                                                         */
/*============================================================================*/

typedef struct 
{
    bool button_0;
    bool button_1;
    bool button_2;
    bool button_3;
    uint8_t encoder_0;
    uint8_t encoder_1;
} whl_data_state_t;

typedef struct
{
    bool driver_flag_trigger;
    bool lap_completed_trigger;
    bool sector_completed_trigger;
    uint8_t sector_number;
    uint32_t event_time_ms;
    uint8_t lap_number;
} whl_data_event_t;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t whl_data_init(void);

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

status_t whl_data_set_state(whl_data_state_t *state);
status_t whl_data_set_event(whl_data_event_t *event);

void whl_data_set_button_0(bool state);
void whl_data_set_button_1(bool state);
void whl_data_set_button_2(bool state);
void whl_data_set_button_3(bool state);

/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

status_t whl_data_get_state(whl_data_state_t * dest);
status_t whl_data_get_event(whl_data_event_t * dest);
bool whl_data_get_button_0();
bool whl_data_get_button_1();
bool whl_data_get_button_2();
bool whl_data_get_button_3();

#endif