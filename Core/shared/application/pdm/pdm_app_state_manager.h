#ifndef APPLICATION_PDM_APP_STATE_MANAGER_H_
#define APPLICATION_PDM_APP_STATE_MANAGER_H_

void pdm_app_state_machine_init(void);
void pdm_app_state_machine_step(void);

typedef enum
{
    PDM_STATE_STARTUP = 0,
//    PDM_STATE_CONFIGURATION,
    PDM_STATE_IDLE,
//    PDM_STATE_ACTIVE,
//    PDM_STATE_CHANNEL_FAULT,
//    PDM_STATE_CRITICAL_FAULT,
//    PDM_STATE_SHUTDOWN
} pdm_app_state_t;

pdm_app_state_t pdm_app_state_machine_get_state(void);


#endif /* APPLICATION_PDM_APP_STATE_MANAGER_H_ */
