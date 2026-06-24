#ifndef CONFIG_MCU_CONFIG_H_
#define CONFIG_MCU_CONFIG_H_

#include <stdbool.h>
#include "pdm_config.h"
#include <stdint.h>
#include "mcu_analog_channels.h"
#include "stm32h7xx_hal.h"



#define MCU_HSD_TOTAL_CHANNELS 4   


/*============================================================================*/
 /* Rear Controller Mapping                                                     */
/*============================================================================*/
#define TS_FAN_PWM     DOUT_1
#define ACCU_FAN_PWM   DOUT_2
#define BAMOCAR_RFE    DOUT_3
#define BAMOCAR_FRG    DOUT_4 


typedef struct
{
    bool enabled;
    tps4xxxx_hw_t hw;
}   mcu_channel_config_t;


extern const mcu_channel_config_t mcu_hsd_default_config[MCU_HSD_TOTAL_CHANNELS];


/*============================================================================*/
/* ANALOG INPUTS                                                              */
/*============================================================================*/


/*============================================================================*/
/* Rear Controler Mapping                                                    */
/*============================================================================*/

#define BATT_HIGHEST_TEMP      MCU_AIN3 
#define BATT_LOWEST_VOLTAGE    MCU_AIN4

/*============================================================================*/
/* System Topology                                                            */
/*============================================================================*/

#define MCU_TOTAL_ANALOG_CHANNELS            8U


typedef struct {
    ADC_HandleTypeDef *adc_handle;
    float adc_vref;
    uint16_t adc_max;  
    bool calibrated;
    bool dma_started;
    uint8_t adc_channels;
} mcu_adc_context_t;

/*============================================================================*/
/* Channel Hardware Mapping                                                   */
/*============================================================================*/



typedef struct
{
    mcu_adc_context_t *adc_context;
    uint8_t adc_buffer_number;
    uint8_t adc_buffer_index;
} analog_hw_t;

/*============================================================================*/
/* Analog calibration (one row in mcu_analog_config)                          */
/*============================================================================*/

typedef struct
{
    float valid_min_v;
    float valid_max_v;
    float active_min_v;
    float active_max_v;
    float scaling_factor;
    bool positive_slope;
} mcu_analog_settings_t;

typedef struct
{
    bool enabled;  
    mcu_analog_settings_t settings;
    analog_hw_t hw;           
} mcu_analog_config_t;



extern const mcu_analog_config_t mcu_analog_config[MCU_TOTAL_ANALOG_CHANNELS];


#endif /* CONFIG_MCU_CONFIG_H_ */
