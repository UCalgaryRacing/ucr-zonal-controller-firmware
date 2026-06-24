typedef enum
{
    /* VNF9Q20F Device 0 - Channels 1-4 */
    VBATT_SW_1 = 0,
    VBATT_SW_2,
    VBATT_SW_3,
    VBATT_SW_4,

    /* VNF9Q20F Device 1 - Channels 5-8 */
    VBATT_SW_5,
    VBATT_SW_6,
    VBATT_SW_7,
    VBATT_SW_8,

    /* TPS4XXXX Device 0 - Channels 9-12 */
    VBATT_SW_9,
    VBATT_SW_10,
    VBATT_SW_11,
    VBATT_SW_12,

    /* TPS4XXXX Device 1 - Channels 13-16 */
    VBATT_SW_13,
    VBATT_SW_14,
    VBATT_SW_15,
    VBATT_SW_16,

    /* TPS4XXXX Device 2 - Regulated 12V Channels 17-20 */
    VBATT_REG_SW_1,
    VBATT_REG_SW_2,
    VBATT_REG_SW_3,
    VBATT_REG_SW_4,

    PDM_CHANNEL_COUNT
} pdm_channel_id_t;


typedef struct
{
    bool enabled;
    uint8_t duty_percent;
    uint16_t frequency_hz;
} pdm_channel_pwm_config_t;

typedef struct
{
    float inom_a;
    uint16_t tnom_ms;
} pdm_channel_i2t_params_t;

typedef enum
{
    PDM_CHANNEL_MODE_RESISTIVE = 0,
    PDM_CHANNEL_MODE_INDUCTIVE,
} pdm_channel_mode_t;

typedef enum
{
    PDM_CHANNEL_CCM_DISABLED = 0,
    PDM_CHANNEL_CCM_ENABLED,
} pdm_channel_ccm_t;

typedef enum
{
    PDM_CHANNEL_TYPE_VNF9Q20F  = 0U,
    PDM_CHANNEL_TYPE_TPS4H160,
    PDM_CHANNEL_TYPE_TPS4H000
} pdm_channel_type_t;
