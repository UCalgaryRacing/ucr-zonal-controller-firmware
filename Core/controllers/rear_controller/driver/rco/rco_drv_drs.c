#include "rco_drv_drs.h"
#include "rco_config_pinout.h"
#include <stdint.h>
#include "stm32h7xx_hal.h"

void rco_drv_drs_set_duty(float duty_cycle)
{
    uint32_t ccr_register_value;

    ccr_register_value = (uint32_t)(duty_cycle * DRS_MAX_TIMER_VALUE);

    RCO_DRS_PWM_TIMER_REGISTER->CCR2 = ccr_register_value;

}
void rco_drv_drs_init()
{
    HAL_TIM_PWM_Start(RCO_DRW_PWM_TIMER, TIM_CHANNEL_2);
}