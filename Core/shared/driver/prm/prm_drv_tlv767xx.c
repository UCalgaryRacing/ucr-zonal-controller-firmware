#include "prm_drv_tlv767xx.h"

void prm_drv_tlv767xx_device_init(const tlv767xx_hw_t *hw)
{
    HAL_GPIO_WritePin(hw->en_port,hw->en_pin,GPIO_PIN_SET);
}
