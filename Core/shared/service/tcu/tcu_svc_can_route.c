#include "can_config.h"
#include "ucr_03.h"
#include "can_typ_common.h"
#include "tcu_data.h"
#include "can_svc_can.h"
#include "com_typ_common.h"

static bool g_initialized = false;

static void tcu_svc_can_rx_apps_data(can_msg_t *msg);
static void tcu_svc_can_rx_front_bse_data(can_msg_t *msg);
static void tcu_svc_can_rx_front_controller_state(can_msg_t *msg);


status_t tcu_svc_can_route_init(void)
{
    //TODO start CAN here
    com_svc_can_register(APPS_CAN_ID, tcu_svc_can_rx_apps_data);
    com_svc_can_register(FRONT_BSE_CAN_ID, tcu_svc_can_rx_front_bse_data);
	com_svc_can_register(FRONT_CONTROLLER_STATE_CAN_ID, tcu_svc_can_rx_front_controller_state);
    g_initialized = true;
    return OK;
}

/*============================================================================*/
/* CAN Receive Mapping                                                        */
/*============================================================================*/

static void tcu_svc_can_rx_apps_data(can_msg_t *msg)
{
    //received by rear controller / logger
    struct ucr_03_apps_t apps_can_msg;
    ucr_03_apps_unpack(&apps_can_msg, msg->data, msg->dlc);

    tcu_apps_data_t apps_data = {0};
    apps_data.sensor1.percent = apps_can_msg.apps1_percent_travel;
    apps_data.sensor2.percent = apps_can_msg.apps2_percent_travel;
    apps_data.pedal_percent = apps_can_msg.apps1_percent_travel;
    tcu_data_set_apps(&apps_data);

    tcu_apps_plausibility_t plausibility;
    tcu_data_get_apps_plausibility(&plausibility);
    plausibility.is_plausible = !(apps_can_msg.apps_fault);
    tcu_data_set_apps_plausibility(&plausibility);
}

static void tcu_svc_can_rx_front_bse_data(can_msg_t *msg)
{
    //received by rear controller
    struct ucr_03_front_bse_t front_bse_can_msg;
    ucr_03_front_bse_unpack(&front_bse_can_msg, msg->data, msg->dlc);

    tcu_bse_data_t bse_data = {0};

    bse_data.pressure_psi = front_bse_can_msg.front_bse_pressure_psi;
    bse_data.is_braking = front_bse_can_msg.braking_active;
    bse_data.is_hard_braking = front_bse_can_msg.hard_braking_active;
    tcu_data_set_bse(&bse_data);
}

void tcu_svc_can_rx_front_controller_state(can_msg_t *msg)
{
    //received by rear controller
    struct ucr_03_front_controller_state_t front_controller_state_msg;
    ucr_03_front_controller_state_unpack(&front_controller_state_msg, msg->data, msg->dlc);

    tcu_app_state_t tcu_state = {0};  

    tcu_state = front_controller_state_msg.state;
    tcu_data_set_state(tcu_state);
}

