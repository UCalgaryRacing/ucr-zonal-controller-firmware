#include "ins_svc_pot.h"
#include "ins_data.h"

#include "mcu_config.h"
#include "mcu_svc_analog.h"
#include "mcu_analog_channels.h"

/*============================================================================*/
/* Static Helper Functions                                                    */
/*============================================================================*/

static inline const mcu_analog_config_t* pot_get_config(mcu_analog_channel_id_t channel_id)
{
    const mcu_analog_config_t *config = mcu_svc_get_channel_config(channel_id);

    // if (config == NULL || (config->type != TCU_ANALOG_TYPE_APPS && config->type != TCU_ANALOG_TYPE_BSE))
    //     return NULL;

    return config;
}

static inline void pot_read_sensor(mcu_analog_channel_id_t channel_id, float *voltage)
{
    mcu_svc_get_analog_voltage(channel_id, voltage);
}

/*============================================================================*/
/* Update Functions                                                           */
/*============================================================================*/

void ins_svc_front_pots_update(mcu_analog_channel_id_t fl_pot_channel_id, mcu_analog_channel_id_t fr_pot_channel_id)
{
    const mcu_analog_config_t *fl_pot_config = pot_get_config(fl_pot_channel_id);
    const mcu_analog_config_t *fr_pot_config = pot_get_config(fr_pot_channel_id);

    ins_suspension_data_t suspension_data;

    if (fl_pot_config != NULL && fr_pot_config != NULL)
    {
        float fl_pot_voltage = 0;
        pot_read_sensor(fl_pot_channel_id, &fl_pot_voltage);
        suspension_data.front_left_suspension = fl_pot_voltage;

        float fr_pot_voltage = 0;
        pot_read_sensor(fr_pot_channel_id, &fr_pot_voltage);
        suspension_data.front_right_suspension = fr_pot_voltage;

    // TODO: insert function converting voltage to suspension travel
    }

    ins_data_set_suspension_travel(FL_SUSPENSION, suspension_data.front_left_suspension);
    ins_data_set_suspension_travel(FR_SUSPENSION, suspension_data.front_right_suspension);

}