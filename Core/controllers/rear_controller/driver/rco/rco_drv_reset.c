#include "stm32h7xx_hal.h"
#include "rco_drv_reset.h"

static bool g_initialized = false;

// do we need this? if it's configured in CubeMX. analogous RTD init function is not called anywhere 
status_t rco_drv_reset_init(void)
{
    g_initialized = true;
    return OK;
}

bool rco_drv_reset_is_button_pressed(void)
{
    GPIO_PinState raw = HAL_GPIO_ReadPin(RCO_RST_BUTTON_GPIO_PORT, RCO_RST_BUTTON_GPIO_PIN);
    return (raw == GPIO_PIN_SET) ? false : true;
}

//true = reset pressed, pulling down reset net
void rco_drv_broadcast_reset_button(bool reset_state)
{
    //BSPD
    HAL_GPIO_WritePin(RCO_BSPD_RESET_PORT, RCO_BSPD_RESET_PIN, reset_state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}