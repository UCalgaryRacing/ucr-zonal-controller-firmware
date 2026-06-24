#include "can_config.h"
#include "com_typ_common.h"
#include "ucr_03.h"
#include "can_typ_common.h"
#include "can_svc_can.h"
#include "acu_data.h"
#include "acu_svc_can_route.h"
#include "rco_data.h"

static void rco_svc_can_rx_reset_data(can_msg_t *msg);
bool g_initialized = false;

status_t rco_svc_can_route_init(void)
{
    //TODO start CAN here
    com_svc_can_register(REAR_CONTROL_CAN_ID, rco_svc_can_rx_reset_data);
    g_initialized = true;
    return OK;
}


void rco_svc_can_rx_reset_data(can_msg_t *msg)
{
    //received by front controller
    struct ucr_03_rear_control_t rear_control_can_msg;
    ucr_03_rear_control_unpack(&rear_control_can_msg, msg->data, msg->dlc);

    rco_data_t rco_data;  
    
    rco_data.tssi_faulted = rear_control_can_msg.tssi_faulted;
    rco_data.brake_light_on = rear_control_can_msg.brake_light;
    rco_data.drs_active = rear_control_can_msg.drs;
    rco_data.reset_pressed = rear_control_can_msg.reset_button;
    
    rco_data_set_data(&rco_data);
}