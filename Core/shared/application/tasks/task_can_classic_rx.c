#include "can_config.h"
#include "com_typ_common.h"
#include "can_typ_common.h"
#include "cmsis_os.h"
#include "can_svc_can.h"

extern osMessageQueueId_t Can1RxQueueHandle;
extern osMessageQueueId_t Can2RxQueueHandle;

static const uint32_t period = 1; // ms
static uint32_t nextWakeTime;

void task_can_classic_rx_init()
{
    nextWakeTime = osKernelGetTickCount();

    return;
}


void task_can_classic_rx_loop()
{
    nextWakeTime += period;


    can_msg_t msg;

    if (osMessageQueueGet(Can1RxQueueHandle, &msg, NULL, 0) == osOK)
        {
            com_svc_can_route(&msg);
        }

    if (osMessageQueueGet(Can2RxQueueHandle, &msg, NULL, 0) == osOK)
        {
            com_svc_can_route(&msg);
        }

	osDelayUntil(nextWakeTime);

    return;
}
