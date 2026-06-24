#include "tcu_drv_rtd.h"
#include "stm32h7xx_hal.h"

static bool g_initialized = false;

status_t tcu_drv_rtd_init(void)
{
    g_initialized = true;
    return OK;
}

bool tcu_drv_rtd_is_button_pressed(void)
{
    GPIO_PinState raw = HAL_GPIO_ReadPin(TCU_RTD_BUTTON_GPIO_PORT, TCU_RTD_BUTTON_GPIO_PIN);
    return (raw == GPIO_PIN_SET) ? false : true;
}
