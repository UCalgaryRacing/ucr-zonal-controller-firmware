#include <stdint.h>
#include "prm_config.h"
#include "prm_config_pinout.h"
#include "prm_channel_types.h"

const prm_channel_config_t prm_default_config[PRM_TOTAL_NUMBER_CHANNELS] =
{
    [INTERNAL_3V3_2] = {
        .enabled = true,
        .hw = {
            .en_port = PRM_3V3_INTERNAL_2_EN_PORT,
            .en_pin = PRM_3V3_INTERNAL_2_EN_PIN
        }
    },

    [INTERNAL_5V_2] = {
        .enabled = true,
        .hw = {
            .en_port = PRM_5V_INTERNAL_2_EN_PORT,
            .en_pin = PRM_5V_INTERNAL_2_EN_PIN
        }
    },

    [INTERNAL_12V] = {
        .enabled = true,
        .hw = {
            .en_port = PRM_12V_INTERNAL_EN_PORT,
            .en_pin = PRM_12V_INTERNAL_EN_PIN
        }
    },

    [EXTERNAL_5V_1] = {
        .enabled = false,
        .hw = {
            .en_port = PRM_5V_EXTERNAL_1_EN_PORT,
            .en_pin = PRM_5V_EXTERNAL_1_EN_PIN
        }
    },

    [EXTERNAL_5V_2] = {
        .enabled = false,
        .hw = {
            .en_port = PRM_5V_EXTERNAL_2_EN_PORT,
            .en_pin = PRM_5V_EXTERNAL_2_EN_PIN
        }
    },

    [EXTERNAL_12V_1] = {
        .enabled = false,
        .hw = {
            .en_port = PRM_12V_EXTERNAL_1_EN_PORT,
            .en_pin = PRM_12V_EXTERNAL_1_EN_PIN
        }
    },

    [EXTERNAL_12V_2] = {
        .enabled = false,
        .hw = {
            .en_port = PRM_12V_EXTERNAL_2_EN_PORT,
            .en_pin = PRM_12V_EXTERNAL_2_EN_PIN
        }
    }
};