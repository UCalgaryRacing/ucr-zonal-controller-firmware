#include "mco_config.h"

#include <stddef.h>

#include "mco_svc_power_limit.h"
#include "mco_svc_traction_control.h"

static const float g_motor_rpm_breakpoints[] = {

    0.0000f,    100.8384f,  201.9321f,  303.2541f,  403.4282f,

    503.8433f,  604.5443f,  705.5927f,  805.6602f,  906.1918f,

    1007.2540f, 1107.5011f, 1208.4036f, 1308.5922f, 1409.5432f,

    1509.8535f, 1611.0139f, 1711.5930f, 1811.6078f, 1912.5918f,

    2013.0812f, 2113.0943f, 2214.2064f, 2314.8594f, 2414.9522f,

    2515.9148f, 2617.4931f, 2717.6842f, 2817.8501f, 2918.7017f,

    3020.2252f, 3120.5547f, 3221.0841f, 3321.5497f, 3421.6747f,

    3522.7959f, 3622.8991f, 3723.0488f, 3824.0586f, 3925.1823f,

    4025.9390f, 4126.0933f, 4226.8638f, 4328.0723f, 4428.3455f,

    4528.8947f, 4629.5916f, 4730.3065f, 4830.9062f, 4931.2565f,

    5032.2403f, 5132.6500f, 5233.3123f, 5334.0173f, 5434.5660f,

    5534.7674f, 5635.2440f

};
 
static const float g_motor_current_limit_rms_a[] = {

    228.7553f, 229.4152f, 230.0508f, 230.7175f, 231.4609f,

    232.3447f, 233.4212f, 234.7106f, 236.1942f, 237.8936f,

    239.8196f, 241.9526f, 244.3187f, 246.8701f, 249.6296f,

    252.5524f, 255.6784f, 258.9652f, 262.4219f, 266.1147f,

    269.9944f, 274.0551f, 278.3267f, 282.5770f, 286.6004f,

    290.1957f, 293.0021f, 294.6824f, 286.5139f, 289.6185f,

    292.2276f, 294.2111f, 295.5356f, 296.1455f, 296.0127f,

    295.0513f, 292.9257f, 289.1735f, 283.6456f, 277.0246f,

    270.0836f, 263.2581f, 256.6418f, 250.3884f, 244.5712f,

    239.0687f, 233.8464f, 228.8741f, 224.1221f, 219.5749f,

    215.1803f, 210.9798f, 206.9303f, 203.0344f, 199.2899f,

    195.6905f, 192.1992f

};
 


const mco_config_t mco_default_config = {
    .motor_rpm_breakpoints = g_motor_rpm_breakpoints,
    .motor_current_limit_rms_a = g_motor_current_limit_rms_a,
    .current_limit_map_size = (uint32_t)(sizeof(g_motor_rpm_breakpoints) / sizeof(g_motor_rpm_breakpoints[0])),
    .power_limit_w = MCO_DEFAULT_POWER_LIMIT_W,
};

void mco_config_apply(const mco_config_t *config)
{
    const mco_config_t *active_config = (config != NULL) ? config : &mco_default_config;

    mco_svc_traction_control_init();
    mco_svc_power_limit_init();
    mco_svc_traction_control_set_current_limit_map(active_config->motor_rpm_breakpoints,
                                                   active_config->motor_current_limit_rms_a,
                                                   active_config->current_limit_map_size);
    mco_svc_power_limit_set_limit_w(active_config->power_limit_w);
}
