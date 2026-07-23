#ifndef MCU_CONFIG_HSD_H_
#define MCU_CONFIG_HSD_H_
// TODO: is this fine? used to be in ther shared service layer. 

#include "stm32h753xx.h"
#include "pdm_config.h"

typedef enum
{
    HSD_5V_FAST_1_CTL = 0,
    HSD_5V_FAST_2_CTL,
    HSD_12V_1_CTL,
    HSD_12V_2_CTL,
    HSD_5V_SLOW_CTL,

    MCU_HSD_TOTAL_CHANNELS,
} mcu_channel_id_t;

typedef struct
{
    bool enabled;
    tps4xxxx_hw_t hw;
} mcu_channel_config_t;

typedef struct
{
    GPIO_TypeDef *input_port;
    uint16_t      input_pin; 
} mcu_hsd_hw_t;
// TODO: change to not using tps4xxxx hw type (used in the old board), use a normal gpio service?
#endif /*MCU_CONFIG_HSD_H_*/
