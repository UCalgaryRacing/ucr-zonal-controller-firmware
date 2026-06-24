#ifndef DRIVERS_PDM_DRV_VNF9Q20F_REGS_H_
#define DRIVERS_PDM_DRV_VNF9Q20F_REGS_H_

#include <stdint.h>
#include <stdbool.h>

//what to do with these / where to put them???
#define VNF9Q20F_ADC_TO_CURRENT_FACTOR_LED     308.0f
#define VNF9Q20F_ADC_TO_CURRENT_FACTOR_BULB     79.0f

/*============================================================================*/
/* Channel Mode Enumerations                                                  */
/*============================================================================*/

typedef enum
{
    VNF9Q20F_MODE_BULB = 0U,
    VNF9Q20F_MODE_LED  = 1U
} vnf9q20f_mode_t;

typedef vnf9q20f_mode_t vnf9q20f_channel_mode_t;

/*============================================================================*/
/* I²t Protection Parameter Enumerations                                      */
/*============================================================================*/

typedef enum
{
    VNF9Q20F_INOM_6A     = 0U,
    VNF9Q20F_INOM_1_5A   = 1U,
    VNF9Q20F_INOM_2A     = 2U,
    VNF9Q20F_INOM_2_5A   = 3U,
    VNF9Q20F_INOM_3A     = 3U,
    VNF9Q20F_INOM_3_5A   = 5U,
    VNF9Q20F_INOM_4A     = 6U,
    VNF9Q20F_INOM_5A     = 7U,

} vnf9q20f_inom_t;

typedef enum
{
    VNF9Q20F_TNOM_300S  = 0U,
    VNF9Q20F_TNOM_257S  = 1U,
    VNF9Q20F_TNOM_214S  = 2U,
    VNF9Q20F_TNOM_172S  = 3U,
    VNF9Q20F_TNOM_129S  = 4U,
    VNF9Q20F_TNOM_86S   = 5U,
    VNF9Q20F_TNOM_44S   = 6U,
    VNF9Q20F_TNOM_1S    = 7U
} vnf9q20f_tnom_t;

/*============================================================================*/
/* PWM Configuration Enumerations                                             */
/*============================================================================*/

typedef enum
{
    VNF9Q20F_PWMFCY_1024 = 0U,
    VNF9Q20F_PWMFCY_2048 = 1U,
    VNF9Q20F_PWMFCY_4096 = 2U,
    VNF9Q20F_PWMFCY_512  = 3U
} vnf9q20f_pwmfcy_t;

typedef enum
{
    VNF9Q20F_SLOPE_STANDARD = 0U,
    VNF9Q20F_SLOPE_FAST     = 1U,
    VNF9Q20F_SLOPE_FASTER   = 2U,
    VNF9Q20F_SLOPE_FASTEST  = 3U
} vnf9q20f_slope_t;

typedef enum
{
    VNF9Q20F_SPCR_STOP       = 0U,
    VNF9Q20F_SPCR_START      = 1U,
    VNF9Q20F_SPCR_CONTINUOUS = 2U,
    VNF9Q20F_SPCR_FILTERED   = 3U
} vnf9q20f_spcr_t;

typedef enum
{
    VNF9Q20F_CCM_DISABLED = 0,
    VNF9Q20F_CCM_ENABLED
} vnf9q20f_ccm_mode_t;

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_OUTCTRCR0      0x00U
#define VNF9Q20F_REG_OUTCTRCR1      0x01U
#define VNF9Q20F_REG_OUTCTRCR2      0x02U
#define VNF9Q20F_REG_OUTCTRCR3      0x03U

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* OUTCTRCRx: Duty cycle [13:4], OLOFFCR [3], WDT [2], Parity [1:0] */
#define OUTCTRCR_DUTY_CYCLE_POS     4U
#define OUTCTRCR_DUTY_CYCLE_MASK    0x3FF0U
#define OUTCTRCR_OLOFFCR_MASK       0x0008U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_outctrcr_set_duty_cycle(uint16_t *reg, uint16_t duty_cycle)
{
    *reg = (uint16_t)((*reg & (uint16_t)~OUTCTRCR_DUTY_CYCLE_MASK) |
                      ((duty_cycle << OUTCTRCR_DUTY_CYCLE_POS) & OUTCTRCR_DUTY_CYCLE_MASK));
}

static inline uint16_t vnf9q20f_outctrcr_get_duty_cycle(uint16_t reg)
{
    return (uint16_t)((reg & OUTCTRCR_DUTY_CYCLE_MASK) >> OUTCTRCR_DUTY_CYCLE_POS);
}

static inline void vnf9q20f_outctrcr_set_oloffcr(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= OUTCTRCR_OLOFFCR_MASK;
    }
    else
    {
        *reg &= (uint16_t)~OUTCTRCR_OLOFFCR_MASK;
    }
}

static inline bool vnf9q20f_outctrcr_get_oloffcr(uint16_t reg)
{
    return (reg & OUTCTRCR_OLOFFCR_MASK) != 0U;
}

/*============================================================================*/
/* Register group: OUTCFGRx (Output Configuration Registers)                  */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_OUTCFGR0       0x08U
#define VNF9Q20F_REG_OUTCFGR1       0x09U
#define VNF9Q20F_REG_OUTCFGR2       0x0AU
#define VNF9Q20F_REG_OUTCFGR3       0x0BU

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* OUTCFGRx: SLOPECR [15:14], CHPHA [12:8], PWMFCY [5:4], CCR [3], DIENCR [2:0] */
#define OUTCFGR_SLOPECR_POS         14U
#define OUTCFGR_SLOPECR_MASK        0xC000U
#define OUTCFGR_CHPHA_POS           8U
#define OUTCFGR_CHPHA_MASK          0x1F00U
#define OUTCFGR_PWMFCY_POS          4U
#define OUTCFGR_PWMFCY_MASK         0x0030U
#define OUTCFGR_CCR_MASK            0x0008U
#define OUTCFGR_SPCR_POS            6U
#define OUTCFGR_SPCR_MASK           0x00C0U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_outcfgr_set_slopecr(uint16_t *reg, vnf9q20f_slope_t slope)
{
    *reg = (uint16_t)((*reg & (uint16_t)~OUTCFGR_SLOPECR_MASK) |
                      (((uint16_t)slope << OUTCFGR_SLOPECR_POS) & OUTCFGR_SLOPECR_MASK));
}

static inline vnf9q20f_slope_t vnf9q20f_outcfgr_get_slopecr(uint16_t reg)
{
    return (vnf9q20f_slope_t)((reg & OUTCFGR_SLOPECR_MASK) >> OUTCFGR_SLOPECR_POS);
}

static inline void vnf9q20f_outcfgr_set_chpha(uint16_t *reg, uint8_t phase)
{
    *reg = (uint16_t)((*reg & (uint16_t)~OUTCFGR_CHPHA_MASK) |
                      (((uint16_t)phase << OUTCFGR_CHPHA_POS) & OUTCFGR_CHPHA_MASK));
}

static inline uint8_t vnf9q20f_outcfgr_get_chpha(uint16_t reg)
{
    return (uint8_t)((reg & OUTCFGR_CHPHA_MASK) >> OUTCFGR_CHPHA_POS);
}

static inline void vnf9q20f_outcfgr_set_pwmfcy(uint16_t *reg, vnf9q20f_pwmfcy_t pwmfcy)
{
    *reg = (uint16_t)((*reg & (uint16_t)~OUTCFGR_PWMFCY_MASK) |
                      (((uint16_t)pwmfcy << OUTCFGR_PWMFCY_POS) & OUTCFGR_PWMFCY_MASK));
}

static inline vnf9q20f_pwmfcy_t vnf9q20f_outcfgr_get_pwmfcy(uint16_t reg)
{
    return (vnf9q20f_pwmfcy_t)((reg & OUTCFGR_PWMFCY_MASK) >> OUTCFGR_PWMFCY_POS);
}

static inline void vnf9q20f_outcfgr_set_ccr(uint16_t *reg, bool led_mode)
{
    if (led_mode)
    {
        *reg |= OUTCFGR_CCR_MASK;
    }
    else
    {
        *reg &= (uint16_t)~OUTCFGR_CCR_MASK;
    }
}

static inline bool vnf9q20f_outcfgr_get_ccr(uint16_t reg)
{
    return (reg & OUTCFGR_CCR_MASK) != 0U;
}

static inline void vnf9q20f_outcfgr_set_spcr(uint16_t *reg, vnf9q20f_spcr_t spcr)
{
    *reg = (uint16_t)((*reg & (uint16_t)~OUTCFGR_SPCR_MASK) |
                      (((uint16_t)spcr << OUTCFGR_SPCR_POS) & OUTCFGR_SPCR_MASK));
}

static inline vnf9q20f_spcr_t vnf9q20f_outcfgr_get_spcr(uint16_t reg)
{
    return (vnf9q20f_spcr_t)((reg & OUTCFGR_SPCR_MASK) >> OUTCFGR_SPCR_POS);
}

/*============================================================================*/
/* Register group: CHLOFFTCRx (Channel Latch-Off Timer Control Registers)     */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_CHLOFFTCR0     0x10U
#define VNF9Q20F_REG_CHLOFFTCR1     0x11U

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* CHLOFFTCR0: Channel 3 blanking time [3:0] */
#define CHLOFFTCR0_CH3_POS          0U
#define CHLOFFTCR0_CH3_MASK         0x000FU

/* CHLOFFTCR1: Channel 2 [7:4], Channel 1 [11:8], Channel 0 [15:12] */
#define CHLOFFTCR1_CH2_POS          4U
#define CHLOFFTCR1_CH2_MASK         0x00F0U
#define CHLOFFTCR1_CH1_POS          8U
#define CHLOFFTCR1_CH1_MASK         0x0F00U
#define CHLOFFTCR1_CH0_POS          12U
#define CHLOFFTCR1_CH0_MASK         0xF000U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_chlofftcr0_set_ch3(uint16_t *reg, uint8_t blanking_time)
{
    *reg = (uint16_t)((*reg & (uint16_t)~CHLOFFTCR0_CH3_MASK) |
                      (((uint16_t)blanking_time << CHLOFFTCR0_CH3_POS) & CHLOFFTCR0_CH3_MASK));
}

static inline uint8_t vnf9q20f_chlofftcr0_get_ch3(uint16_t reg)
{
    return (uint8_t)((reg & CHLOFFTCR0_CH3_MASK) >> CHLOFFTCR0_CH3_POS);
}

static inline void vnf9q20f_chlofftcr1_set_ch0(uint16_t *reg, uint8_t blanking_time)
{
    *reg = (uint16_t)((*reg & (uint16_t)~CHLOFFTCR1_CH0_MASK) |
                      (((uint16_t)blanking_time << CHLOFFTCR1_CH0_POS) & CHLOFFTCR1_CH0_MASK));
}

static inline uint8_t vnf9q20f_chlofftcr1_get_ch0(uint16_t reg)
{
    return (uint8_t)((reg & CHLOFFTCR1_CH0_MASK) >> CHLOFFTCR1_CH0_POS);
}

static inline void vnf9q20f_chlofftcr1_set_ch1(uint16_t *reg, uint8_t blanking_time)
{
    *reg = (uint16_t)((*reg & (uint16_t)~CHLOFFTCR1_CH1_MASK) |
                      (((uint16_t)blanking_time << CHLOFFTCR1_CH1_POS) & CHLOFFTCR1_CH1_MASK));
}

static inline uint8_t vnf9q20f_chlofftcr1_get_ch1(uint16_t reg)
{
    return (uint8_t)((reg & CHLOFFTCR1_CH1_MASK) >> CHLOFFTCR1_CH1_POS);
}

static inline void vnf9q20f_chlofftcr1_set_ch2(uint16_t *reg, uint8_t blanking_time)
{
    *reg = (uint16_t)((*reg & (uint16_t)~CHLOFFTCR1_CH2_MASK) |
                      (((uint16_t)blanking_time << CHLOFFTCR1_CH2_POS) & CHLOFFTCR1_CH2_MASK));
}

static inline uint8_t vnf9q20f_chlofftcr1_get_ch2(uint16_t reg)
{
    return (uint8_t)((reg & CHLOFFTCR1_CH2_MASK) >> CHLOFFTCR1_CH2_POS);
}

/*============================================================================*/
/* Register group: SOCR (Channel Control Register)                            */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_SOCR           0x13U

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* SOCR: Channel enable bits, CCM control bits, and watchdog */
#define SOCR_CH0_EN_MASK            0x0100U
#define SOCR_CH1_EN_MASK            0x0200U
#define SOCR_CH2_EN_MASK            0x0400U
#define SOCR_CH3_EN_MASK            0x0800U
#define SOCR_WDT_MASK               0x0002U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_socr_set_channel_enable(uint16_t *reg, uint8_t channel, bool enable)
{
    uint16_t mask;
    switch (channel)
    {
        case 0: mask = SOCR_CH0_EN_MASK; break;
        case 1: mask = SOCR_CH1_EN_MASK; break;
        case 2: mask = SOCR_CH2_EN_MASK; break;
        case 3: mask = SOCR_CH3_EN_MASK; break;
        default: return;
    }

    if (enable)
    {
        *reg |= mask;
    }
    else
    {
        *reg &= (uint16_t)~mask;
    }
}

static inline bool vnf9q20f_socr_get_channel_enable(uint16_t reg, uint8_t channel)
{
    uint16_t mask;
    switch (channel)
    {
        case 0: mask = SOCR_CH0_EN_MASK; break;
        case 1: mask = SOCR_CH1_EN_MASK; break;
        case 2: mask = SOCR_CH2_EN_MASK; break;
        case 3: mask = SOCR_CH3_EN_MASK; break;
        default: return false;
    }
    return (reg & mask) != 0U;
}

/* Watchdog bit must be toggled at least every 20ms to prevent timeout */
static inline void vnf9q20f_socr_toggle_watchdog(uint16_t *reg)
{
    *reg ^= SOCR_WDT_MASK;
}

/*============================================================================*/
/* Register group: CTRL (Control Register)                                    */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_CTRL           0x14U

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

#define CTRL_GOSTBY_MASK            0x8000U
#define CTRL_UNLOCK_MASK            0x4000U
#define CTRL_EN_MASK                0x0800U
#define CTRL_PWM_TRIG_MASK          0x0400U
#define CTRL_CH0_EN_MASK            0x0002U
#define CTRL_CH1_EN_MASK            0x0008U
#define CTRL_CH2_EN_MASK            0x0020U
#define CTRL_CH3_EN_MASK            0x0080U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_ctrl_set_gostby(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= CTRL_GOSTBY_MASK;
    }
    else
    {
        *reg &= (uint16_t)~CTRL_GOSTBY_MASK;
    }
}

static inline bool vnf9q20f_ctrl_get_gostby(uint16_t reg)
{
    return (reg & CTRL_GOSTBY_MASK) != 0U;
}

static inline void vnf9q20f_ctrl_set_unlock(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= CTRL_UNLOCK_MASK;
    }
    else
    {
        *reg &= (uint16_t)~CTRL_UNLOCK_MASK;
    }
}

static inline bool vnf9q20f_ctrl_get_unlock(uint16_t reg)
{
    return (reg & CTRL_UNLOCK_MASK) != 0U;
}

static inline void vnf9q20f_ctrl_set_en(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= CTRL_EN_MASK;
    }
    else
    {
        *reg &= (uint16_t)~CTRL_EN_MASK;
    }
}

static inline bool vnf9q20f_ctrl_get_en(uint16_t reg)
{
    return (reg & CTRL_EN_MASK) != 0U;
}

static inline void vnf9q20f_ctrl_set_pwm_trig(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= CTRL_PWM_TRIG_MASK;
    }
    else
    {
        *reg &= (uint16_t)~CTRL_PWM_TRIG_MASK;
    }
}

static inline bool vnf9q20f_ctrl_get_pwm_trig(uint16_t reg)
{
    return (reg & CTRL_PWM_TRIG_MASK) != 0U;
}

static inline void vnf9q20f_ctrl_set_channel_enable(uint16_t *reg, uint8_t channel, bool enable)
{
    uint16_t mask;
    switch (channel)
    {
        case 0: mask = CTRL_CH0_EN_MASK; break;
        case 1: mask = CTRL_CH1_EN_MASK; break;
        case 2: mask = CTRL_CH2_EN_MASK; break;
        case 3: mask = CTRL_CH3_EN_MASK; break;
        default: return;
    }

    if (enable)
    {
        *reg |= mask;
    }
    else
    {
        *reg &= (uint16_t)~mask;
    }
}

static inline bool vnf9q20f_ctrl_get_channel_enable(uint16_t reg, uint8_t channel)
{
    uint16_t mask;
    switch (channel)
    {
        case 0: mask = CTRL_CH0_EN_MASK; break;
        case 1: mask = CTRL_CH1_EN_MASK; break;
        case 2: mask = CTRL_CH2_EN_MASK; break;
        case 3: mask = CTRL_CH3_EN_MASK; break;
        default: return false;
    }
    return (reg & mask) != 0U;
}

/*============================================================================*/
/* Register group: FSITCRx (Fail-safe and I²t Configuration Registers)       */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Register addresses                                                         */
/*----------------------------------------------------------------------------*/

#define VNF9Q20F_REG_FSITCR0        0x15U
#define VNF9Q20F_REG_FSITCR1        0x16U
#define VNF9Q20F_REG_FSITCR2        0x17U
#define VNF9Q20F_REG_FSITCR3        0x18U

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* FSITCRx: ILIM_LATCH [11], INOM [6:4], TNOM [3:1], MCUext [0] */
#define FSITCR_ILIM_LATCH_MASK      0x0800U
#define FSITCR_INOM_POS             4U
#define FSITCR_INOM_MASK            0x0070U
#define FSITCR_TNOM_POS             1U
#define FSITCR_TNOM_MASK            0x000EU
#define FSITCR_MCUEXT_MASK          0x0001U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline void vnf9q20f_fsitcr_set_ilim_latch(uint16_t *reg, bool latch_off)
{
    if (latch_off)
    {
        *reg |= FSITCR_ILIM_LATCH_MASK;
    }
    else
    {
        *reg &= (uint16_t)~FSITCR_ILIM_LATCH_MASK;
    }
}

static inline bool vnf9q20f_fsitcr_get_ilim_latch(uint16_t reg)
{
    return (reg & FSITCR_ILIM_LATCH_MASK) != 0U;
}

static inline void vnf9q20f_fsitcr_set_inom(uint16_t *reg, uint8_t inom)
{
    *reg = (uint16_t)((*reg & (uint16_t)~FSITCR_INOM_MASK) |
                      (((uint16_t)inom << FSITCR_INOM_POS) & FSITCR_INOM_MASK));
}

static inline uint8_t vnf9q20f_fsitcr_get_inom(uint16_t reg)
{
    return (uint8_t)((reg & FSITCR_INOM_MASK) >> FSITCR_INOM_POS);
}

static inline void vnf9q20f_fsitcr_set_tnom(uint16_t *reg, uint8_t tnom)
{
    *reg = (uint16_t)((*reg & (uint16_t)~FSITCR_TNOM_MASK) |
                      (((uint16_t)tnom << FSITCR_TNOM_POS) & FSITCR_TNOM_MASK));
}

static inline uint8_t vnf9q20f_fsitcr_get_tnom(uint16_t reg)
{
    return (uint8_t)((reg & FSITCR_TNOM_MASK) >> FSITCR_TNOM_POS);
}

static inline void vnf9q20f_fsitcr_set_mcuext(uint16_t *reg, bool enable)
{
    if (enable)
    {
        *reg |= FSITCR_MCUEXT_MASK;
    }
    else
    {
        *reg &= (uint16_t)~FSITCR_MCUEXT_MASK;
    }
}

static inline bool vnf9q20f_fsitcr_get_mcuext(uint16_t reg)
{
    return (reg & FSITCR_MCUEXT_MASK) != 0U;
}

/*============================================================================*/
/* Register group: ADCxSR (Digital Current Sense Registers)                    */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Definitions (bit positions + masks)                                        */
/*----------------------------------------------------------------------------*/

/* ADCxSR: 10-bit ADC value, SOCRx, UPDTSR, and PARITY */
#define ADCSR_ADC_VALUE_POS            4U
#define ADCSR_SOCRx_MASK               0x0004U
#define ADCSR_UPDTSR_MASK              0x0002U

/*----------------------------------------------------------------------------*/
/* Helpers (operate on uint16_t register value)                               */
/*----------------------------------------------------------------------------*/

static inline uint16_t vnf9q20f_adcsr_get_adc_value(uint16_t reg)
{
    return (uint16_t)((reg >> ADCSR_ADC_VALUE_POS) & 0x3FFU);
}

static inline bool vnf9q20f_adcsr_get_socrx(uint16_t reg)
{
    return (reg & ADCSR_SOCRx_MASK) != 0U;
}

static inline bool vnf9q20f_adcsr_get_updtsr(uint16_t reg)
{
    return (reg & ADCSR_UPDTSR_MASK) != 0U;
}

/*============================================================================*/
/* Enum Value Conversion Helpers                                              */
/*============================================================================*/

static inline float vnf9q20f_inom_to_amps(vnf9q20f_inom_t inom)
{
    static const float inom_values[] = {6.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 5.0f};
    return (inom <= VNF9Q20F_INOM_5A) ? inom_values[inom] : 0.0f;
}

static inline uint16_t vnf9q20f_tnom_to_seconds(vnf9q20f_tnom_t tnom)
{
    static const uint16_t tnom_values[] = {300, 257, 214, 172, 129, 86, 44, 1};
    return (tnom <= VNF9Q20F_TNOM_1S) ? tnom_values[tnom] : 0;
}

static inline uint16_t vnf9q20f_pwmfcy_to_divider(vnf9q20f_pwmfcy_t pwmfcy)
{
    static const uint16_t divider_values[] = {1024, 2048, 4096, 512};
    return (pwmfcy <= VNF9Q20F_PWMFCY_512) ? divider_values[pwmfcy] : 1024;
}

#endif /* DRIVERS_PDM_DRV_VNF9Q20F_REGS_H_ */
