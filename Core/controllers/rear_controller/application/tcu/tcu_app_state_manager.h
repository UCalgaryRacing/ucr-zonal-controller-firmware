#ifndef APPLICATION_TCU_APP_STATE_MANAGER_H_
#define APPLICATION_TCU_APP_STATE_MANAGER_H_

void tcu_app_state_machine_init(void);
void tcu_app_state_machine_step(void);

typedef enum
{
    TCU_APP_STATE_STARTUP = 0,
    TCU_APP_STATE_IDLE,
    TCU_APP_STATE_READY_TO_DRIVE,
    TCU_APP_STATE_REVERSE,
    TCU_APP_STATE_MOTOR_DISABLED
} tcu_app_state_t;

tcu_app_state_t tcu_app_state_machine_get_state(void);


#endif /* APPLICATION_TCU_APP_STATE_MANAGER_H_ */
