#include "fco_drv_ptt.h"
#include "stm32h7xx_hal.h"

static bool g_initialized = false;

status_t fco_drv_ptt_init(void)
{
    g_initialized = true;
    return OK;
}

//TODO: error checking? 
void fco_drv_ptt_set_on(bool on)
{
    HAL_GPIO_WritePin(PTT_CTL_GPIO_Port, PTT_CTL_GPIO_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
