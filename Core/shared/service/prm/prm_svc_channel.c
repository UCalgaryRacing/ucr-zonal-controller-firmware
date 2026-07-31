#include "prm_svc_channel.h"
#include "prm_config.h"
#include "prm_drv_tlv767xx.h"


// Helper functions
static inline const prm_channel_config_t * get_channel_config(prm_channel_id_t channel_id)
{
    return &prm_default_config[channel_id];
}

// Init
int prm_svc_channel_init(prm_channel_id_t channel_id)
{
    const prm_channel_config_t *channel_config = get_channel_config(channel_id);

    if(channel_config->enabled == false)
    {
        return -1;
    }
    else
    {
        prm_drv_tlv767xx_device_init(&channel_config->hw);
        return 1;
    }
}

