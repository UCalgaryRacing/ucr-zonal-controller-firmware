#ifndef CONFIG_MCO_CONFIG_H_
#define CONFIG_MCO_CONFIG_H_

#include <stdint.h>

#define MCO_DEFAULT_POWER_LIMIT_W (60000.0f)

typedef struct
{
    const float *motor_rpm_breakpoints;
    const float *motor_current_limit_rms_a;
    uint32_t current_limit_map_size;
    float power_limit_w;
} mco_config_t;

extern const mco_config_t mco_default_config;

void mco_config_apply(const mco_config_t *config);

#endif /* CONFIG_MCO_CONFIG_H_ */
