#include "mcu_svc_hsd.h"
#include "mcu_config.h"
#include "com_typ_common.h"
#include "pdm_drv_tps4xxxx.h"

//TODO make tps4xxx service which both mcu and pdm can call instead of rewriting

static inline bool is_valid_channel_id(mcu_channel_id_t channel_id)
{
    return (channel_id < MCU_HSD_TOTAL_CHANNELS);
}


static inline const mcu_channel_config_t* get_channel_config(mcu_channel_id_t channel_id)
{
    if (!is_valid_channel_id(channel_id))
    {
        return NULL;
    }
    return &mcu_hsd_default_config[channel_id];
}



status_t mcu_svc_hsd_channel_enable(mcu_channel_id_t channel_id, bool enable)
{
    const mcu_channel_config_t *channel_config = get_channel_config(channel_id);
    if (channel_config == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    int result;

    result = tps4xxxx_enable_channel(&channel_config->hw, enable);

    return (result == 0) ? OK : ERROR_GENERAL;
}