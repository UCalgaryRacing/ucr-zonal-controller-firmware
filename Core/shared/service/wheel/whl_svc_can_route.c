#include "can_config.h"
#include "com_typ_common.h"
#include "ucr_03.h"
#include "can_typ_common.h"
#include "can_svc_can.h"
#include "acu_data.h"
#include "acu_svc_can_route.h"
#include "rco_data.h"
#include "whl_svc_can_route.h"
#include "whl_data.h"

static void whl_svc_can_rx_wheel_state(can_msg_t *msg);
static bool g_initialized = false;

status_t whl_svc_can_route_init(void)
{
    //TODO start CAN here
    com_svc_can_register(STEERING_WHEEL_STATE_CAN_ID, whl_svc_can_rx_wheel_state);
    g_initialized = true;
    return OK;
}


void whl_svc_can_rx_wheel_state(can_msg_t *msg)
{
    struct ucr_03_wheel_transmit_state_t wheel_state_can_msg;
    ucr_03_wheel_transmit_state_unpack(&wheel_state_can_msg,msg->data,msg->dlc);
    
    whl_data_state_t state;

    state.button_0 = wheel_state_can_msg.button0;
    state.button_1 = wheel_state_can_msg.button1;
    state.button_2 = wheel_state_can_msg.button2;
    state.button_3 = wheel_state_can_msg.button3;
    state.encoder_0 = wheel_state_can_msg.encoder0;
    state.encoder_1 = wheel_state_can_msg.encoder1;

    whl_data_set_state(&state);

}