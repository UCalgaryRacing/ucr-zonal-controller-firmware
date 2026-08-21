#ifndef DRIVER_MCU_DRV_ANALOG_H_
#define DRIVER_MCU_DRV_ANALOG_H_

#include <stdbool.h>
#include <stdint.h>
#include "com_typ_common.h"
#include "stm32h7xx_hal.h"
#include "mcu_config.h"
#include "mcu_config_pinout.h"

#define MCU_MAX_ADC_CHANNELS 16U

status_t mcu_drv_analog_read_raw(const analog_hw_t *hw, uint16_t *adc_value);

status_t mcu_drv_analog_init(const analog_hw_t *hw);

status_t mcu_drv_analog_start_adc(const analog_hw_t *hw);

typedef struct
{
    ADC_HandleTypeDef *handle;
    bool calibrated;
    bool dma_started;
} adc_instance_state_t;

#endif /* DRIVER_MCU_DRV_ANALOG_H_ */
