
# include "pdm_drv_lm5060.h"
# include "pdm_config_pinout.h"


void LM5060_config_for_shutdown(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}


bool lm5060_disable_pdm(bool latch_off){
    

    if (latch_off){
    	LM5060_config_for_shutdown();
        HAL_GPIO_WritePin(PDM_LM5060_GLV_TIMER_GPIO_PORT, PDM_LM5060_GLV_TIMER_PIN, GPIO_PIN_SET);
        return true;
    }

    else
        return false;


}
