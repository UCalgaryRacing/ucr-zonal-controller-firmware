#include "pdm_drv_vnf9q20f.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

static bool g_driver_initialized = false;

/*============================================================================*/
/* SPI Bus Mutex                                                              */
/*============================================================================*/

static osMutexId_t g_spi_mutex;

static const osMutexAttr_t g_spi_mutex_attr =
{
    .name = "vnf9q20f_spi_mutex",
    .attr_bits = osMutexPrioInherit,
    .cb_mem = NULL,
    .cb_size = 0U,
};

static inline void spi_lock(void)
{
    osMutexAcquire(g_spi_mutex, osWaitForever);
}

static inline void spi_unlock(void)
{
    osMutexRelease(g_spi_mutex);
}

/*----------------------------------------------------------------------------*/
/* Shadow register pointer helpers                                            */
/*----------------------------------------------------------------------------*/

static uint16_t* get_shadow_outctrcr(vnf9q20f_shadow_t *shadow, uint8_t channel)
{
    switch (channel)
    {
        case 0: return &shadow->outctrcr0;
        case 1: return &shadow->outctrcr1;
        case 2: return &shadow->outctrcr2;
        case 3: return &shadow->outctrcr3;
        default: return NULL;
    }
}

static uint16_t* get_shadow_outcfgr(vnf9q20f_shadow_t *shadow, uint8_t channel)
{
    switch (channel)
    {
        case 0: return &shadow->outcfgr0;
        case 1: return &shadow->outcfgr1;
        case 2: return &shadow->outcfgr2;
        case 3: return &shadow->outcfgr3;
        default: return NULL;
    }
}

static uint16_t* get_shadow_fsitcr(vnf9q20f_shadow_t *shadow, uint8_t channel)
{
    switch (channel)
    {
        case 0: return &shadow->fsitcr0;
        case 1: return &shadow->fsitcr1;
        case 2: return &shadow->fsitcr2;
        case 3: return &shadow->fsitcr3;
        default: return NULL;
    }
}

static uint8_t get_outctrcr_addr(uint8_t channel)
{
    switch (channel)
    {
        case 0: return VNF9Q20F_REG_OUTCTRCR0;
        case 1: return VNF9Q20F_REG_OUTCTRCR1;
        case 2: return VNF9Q20F_REG_OUTCTRCR2;
        case 3: return VNF9Q20F_REG_OUTCTRCR3;
        default: return 0xFF;
    }
}

static uint8_t get_outcfgr_addr(uint8_t channel)
{
    switch (channel)
    {
        case 0: return VNF9Q20F_REG_OUTCFGR0;
        case 1: return VNF9Q20F_REG_OUTCFGR1;
        case 2: return VNF9Q20F_REG_OUTCFGR2;
        case 3: return VNF9Q20F_REG_OUTCFGR3;
        default: return 0xFF;
    }
}

static uint8_t get_fsitcr_addr(uint8_t channel)
{
    switch (channel)
    {
        case 0: return VNF9Q20F_REG_FSITCR0;
        case 1: return VNF9Q20F_REG_FSITCR1;
        case 2: return VNF9Q20F_REG_FSITCR2;
        case 3: return VNF9Q20F_REG_FSITCR3;
        default: return 0xFF;
    }
}

static uint8_t get_outsr_addr(uint8_t channel)
{
    switch (channel)
    {
        case 0: return 0x20U;
        case 1: return 0x21U;
        case 2: return 0x22U;
        case 3: return 0x23U;
        default: return 0xFF;
    }
}

static uint8_t get_adcsr_addr(uint8_t channel)
{
    switch (channel)
    {
        case 0: return 0x28U;
        case 1: return 0x29U;
        case 2: return 0x2AU;
        case 3: return 0x2BU;
        default: return 0xFF;
    }
}

/*----------------------------------------------------------------------------*/
/* Low-level SPI helpers                                                      */
/*----------------------------------------------------------------------------*/

/* Brian Kernighan's algorithm */
static inline uint8_t popcount8(uint8_t byte)
{
    uint8_t count = 0;
    while (byte)
    {
        byte &= (byte - 1);
        count++;
    }
    return count;
}

/*
 * The VNF9Q20F uses odd parity: the total number of 1 bits in the
 * 24-bit message (command + data) must be odd. The parity bit is
 * bit 0 (LSB) of the third byte.
 */
static void apply_odd_parity(uint8_t buf[3])
{
    buf[2] &= 0xFE;

    uint8_t ones_count = popcount8(buf[0]) + popcount8(buf[1]) + popcount8(buf[2]);

    if ((ones_count & 0x01) == 0)
    {
        buf[2] |= 0x01;
    }
    /* If count is already odd, parity bit stays 0 (we already cleared it) */
}

static uint8_t build_command_byte(uint8_t opcode, uint8_t addr)
{
    return (uint8_t)((opcode << 6) | (addr & 0x3F));
}

static HAL_StatusTypeDef vnf9q20f_spi_transaction(const vnf9q20f_hw_t *hw, const uint8_t tx_buf[3], uint8_t rx_buf[3])
{
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef hal_status = HAL_SPI_TransmitReceive(hw->spi_handle, (uint8_t *)tx_buf, rx_buf, 3U, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_SET);
    return hal_status;
}

/*
 * Note: The parity bit (bit 0 of the data) will be overwritten to ensure
 * odd parity.
 */
static HAL_StatusTypeDef vnf9q20f_spi_write_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr, uint16_t data, uint8_t *gsb)
{
    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    tx_buf[0] = build_command_byte(0, reg_addr);
    tx_buf[1] = (uint8_t)(data >> 8);
    tx_buf[2] = (uint8_t)(data & 0xFF);

    apply_odd_parity(tx_buf);

    HAL_StatusTypeDef hal_status = vnf9q20f_spi_transaction(hw, tx_buf, rx_buf);

    if ((hal_status == HAL_OK) && (gsb != NULL))
    {
        *gsb = rx_buf[0];
    }

    return hal_status;
}

/*
 * For read commands, the data bytes are ignored by the device but the
 * parity bit must still be correct.
 */
static HAL_StatusTypeDef vnf9q20f_spi_read_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr, uint16_t *data, uint8_t *gsb)
{
    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    tx_buf[0] = build_command_byte(1, reg_addr);
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;

    /* Device checks parity even on read commands */
    apply_odd_parity(tx_buf);

    HAL_StatusTypeDef hal_status = vnf9q20f_spi_transaction(hw, tx_buf, rx_buf);

    if (hal_status == HAL_OK)
    {
        if (data != NULL)
        {
            *data = (uint16_t)(((uint16_t)rx_buf[1] << 8) | rx_buf[2]);
        }
        if (gsb != NULL)
        {
            *gsb = rx_buf[0];
        }
    }

    return hal_status;
}

/*
 * Opcode 3 (binary 11) = read device information (ROM access).
 * Used to clear the RSTB reset bit in the Global Status Byte after startup/reset.
 */
static HAL_StatusTypeDef vnf9q20f_spi_read_rom(const vnf9q20f_hw_t *hw, uint8_t rom_addr, uint16_t *data, uint8_t *gsb)
{
    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    tx_buf[0] = build_command_byte(3, rom_addr);
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;

    apply_odd_parity(tx_buf);

    HAL_StatusTypeDef hal_status = vnf9q20f_spi_transaction(hw, tx_buf, rx_buf);

    if (hal_status == HAL_OK)
    {
        if (data != NULL)
        {
            /* ROM is 8-bit, but device returns 16-bit. Second byte contains ROM data. */
            *data = (uint16_t)(((uint16_t)rx_buf[1] << 8) | rx_buf[2]);
        }
        if (gsb != NULL)
        {
            *gsb = rx_buf[0];
        }
    }

    return hal_status;
}

static HAL_StatusTypeDef vnf9q20f_spi_read_and_clear(const vnf9q20f_hw_t *hw, uint8_t reg_addr, uint16_t *data, uint8_t *gsb)
{
    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    tx_buf[0] = build_command_byte(2, reg_addr);
    tx_buf[1] = 0xFF;
    tx_buf[2] = 0xFF;

    HAL_StatusTypeDef hal_status = vnf9q20f_spi_transaction(hw, tx_buf, rx_buf);

    if (hal_status == HAL_OK)
    {
        if (data != NULL)
        {
            *data = (uint16_t)(((uint16_t)rx_buf[1] << 8) | rx_buf[2]);
        }
        if (gsb != NULL)
        {
            *gsb = rx_buf[0];
        }
    }

    return hal_status;
}

/*----------------------------------------------------------------------------*/
/* Shadow initialization helper                                               */
/*----------------------------------------------------------------------------*/

static void shadow_init_defaults(const vnf9q20f_hw_t *hw)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    shadow->outctrcr0 = 0x0000U;
    shadow->outctrcr1 = 0x0000U;
    shadow->outctrcr2 = 0x0000U;
    shadow->outctrcr3 = 0x0000U;

    shadow->outcfgr0  = 0x0000U;
    shadow->outcfgr1  = 0x0000U;
    shadow->outcfgr2  = 0x0000U;
    shadow->outcfgr3  = 0x0000U;

    shadow->chlofftcr0 = 0x0000U;
    shadow->chlofftcr1 = 0x0000U;

    shadow->socr = 0x0000U;
    shadow->ctrl = 0x0000U;

    shadow->fsitcr0 = 0x0000U;
    shadow->fsitcr1 = 0x0000U;
    shadow->fsitcr2 = 0x0000U;
    shadow->fsitcr3 = 0x0000U;
}

/*============================================================================*/
/* Driver Initialization                                                      */
/*============================================================================*/

int vnf9q20f_driver_init()
{
    if (g_driver_initialized)
    {
        return 0;
    }

    g_spi_mutex = osMutexNew(&g_spi_mutex_attr);
    if (g_spi_mutex == NULL)
    {
        return -1;
    }

    g_driver_initialized = true;
    return 0;
}

int vnf9q20f_device_init(const vnf9q20f_hw_t *hw, bool enter_normal)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    osDelay(1);
    HAL_GPIO_WritePin(hw->cs_port, hw->cs_pin, GPIO_PIN_RESET);

    if (vnf9q20f_software_reset(hw) != 0)
    {
        return -1;
    }

    /* Clear RSTB reset bit by reading ROM */
    spi_lock();
    uint8_t gsb = 0;
    HAL_StatusTypeDef hal_status = vnf9q20f_spi_read_rom(hw, 0x00, NULL, &gsb);
    spi_unlock();

    if (hal_status != HAL_OK)
    {
        return -1;
    }

    if (vnf9q20f_sync_device_to_shadow(hw) != 0)
    {
        return -1;
    }

    if (enter_normal)
    {
        if (vnf9q20f_enter_normal_mode(hw) != 0)
        {
            return -1;
        }
    }

    return 0;
}

int vnf9q20f_sync_shadow_to_device(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    HAL_StatusTypeDef s = HAL_OK;

    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCTRCR0, shadow->outctrcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCTRCR1, shadow->outctrcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCTRCR2, shadow->outctrcr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCTRCR3, shadow->outctrcr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCFGR0, shadow->outcfgr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCFGR1, shadow->outcfgr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCFGR2, shadow->outcfgr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_OUTCFGR3, shadow->outcfgr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CHLOFFTCR0, shadow->chlofftcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CHLOFFTCR1, shadow->chlofftcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_SOCR, shadow->socr, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CTRL, shadow->ctrl, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_FSITCR0, shadow->fsitcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_FSITCR1, shadow->fsitcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_FSITCR2, shadow->fsitcr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_FSITCR3, shadow->fsitcr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    spi_unlock();
    return 0;
}

int vnf9q20f_sync_device_to_shadow(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    HAL_StatusTypeDef s = HAL_OK;

    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCTRCR0, &shadow->outctrcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCTRCR1, &shadow->outctrcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCTRCR2, &shadow->outctrcr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCTRCR3, &shadow->outctrcr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCFGR0, &shadow->outcfgr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCFGR1, &shadow->outcfgr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCFGR2, &shadow->outcfgr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_OUTCFGR3, &shadow->outcfgr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_CHLOFFTCR0, &shadow->chlofftcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_CHLOFFTCR1, &shadow->chlofftcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_SOCR, &shadow->socr, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_CTRL, &shadow->ctrl, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_FSITCR0, &shadow->fsitcr0, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_FSITCR1, &shadow->fsitcr1, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_FSITCR2, &shadow->fsitcr2, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }
    s = vnf9q20f_spi_read_register(hw, VNF9Q20F_REG_FSITCR3, &shadow->fsitcr3, NULL);
    if (s != HAL_OK) { spi_unlock(); return -1; }

    spi_unlock();
    return 0;
}

/*============================================================================*/
/* Channel Enable/Disable Control                                             */
/*============================================================================*/

int vnf9q20f_enable_channel(const vnf9q20f_hw_t *hw, bool enable)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    vnf9q20f_socr_set_channel_enable(&shadow->socr, hw->channel, enable);

    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_SOCR, shadow->socr, NULL);

    spi_unlock();

    return (s == HAL_OK) ? 0 : -1;
}

// /*============================================================================*/
// /* PWM Configuration                                                          */
// /*============================================================================*/

int vnf9q20f_set_duty_cycle_percent(const vnf9q20f_hw_t *hw, uint8_t duty_percent)
{
    if (duty_percent > 100)
    {
        duty_percent = 100;
    }
    /* Convert 0-100% to 0-0x3FF (10-bit) */
    uint16_t duty_raw = (uint16_t)(((uint32_t)duty_percent * 0x3FF) / 100);
    return vnf9q20f_set_duty_cycle_raw(hw, duty_raw);
}

int vnf9q20f_set_duty_cycle_raw(const vnf9q20f_hw_t *hw, uint16_t duty_raw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }
    if (duty_raw > 0x3FF)
    {
        duty_raw = 0x3FF;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    uint16_t *reg = get_shadow_outctrcr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outctrcr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outctrcr_set_duty_cycle(reg, duty_raw);

    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

uint8_t vnf9q20f_get_duty_cycle_percent(const vnf9q20f_hw_t *hw)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    const uint16_t *reg = get_shadow_outctrcr(shadow, hw->channel);
    if (reg == NULL) return 0;

    uint16_t duty_raw = vnf9q20f_outctrcr_get_duty_cycle(*reg);
    return (uint8_t)(((uint32_t)duty_raw * 100) / 0x3FF);
}

int vnf9q20f_set_pwm_frequency(const vnf9q20f_hw_t *hw, vnf9q20f_pwmfcy_t pwmfcy)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outcfgr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outcfgr_set_pwmfcy(reg, pwmfcy);
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_set_channel_phase(const vnf9q20f_hw_t *hw, uint8_t phase_value)
{
    if (phase_value > 31)
    {
        phase_value = 31;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outcfgr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outcfgr_set_chpha(reg, phase_value);
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

/*============================================================================*/
/* Channel Mode Configuration                                                 */
/*============================================================================*/

int vnf9q20f_set_channel_mode(const vnf9q20f_hw_t *hw, vnf9q20f_mode_t mode)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;
    uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outcfgr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outcfgr_set_ccr(reg, (mode == VNF9Q20F_MODE_LED));
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

vnf9q20f_mode_t vnf9q20f_get_channel_mode(const vnf9q20f_hw_t *hw)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    const uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return VNF9Q20F_MODE_BULB;

    return vnf9q20f_outcfgr_get_ccr(*reg) ? VNF9Q20F_MODE_LED : VNF9Q20F_MODE_BULB;
}

int vnf9q20f_set_switching_slope(const vnf9q20f_hw_t *hw, vnf9q20f_slope_t slope)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;
    uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outcfgr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outcfgr_set_slopecr(reg, slope);
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

/*============================================================================*/
/* I²t Protection Configuration                                               */
/*============================================================================*/

int vnf9q20f_set_i2t_protection(const vnf9q20f_hw_t *hw, vnf9q20f_inom_t inom, vnf9q20f_tnom_t tnom)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    uint16_t *reg = get_shadow_fsitcr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_fsitcr_addr(hw->channel);

    spi_lock();

    vnf9q20f_fsitcr_set_inom(reg, (uint8_t)inom);
    vnf9q20f_fsitcr_set_tnom(reg, (uint8_t)tnom);
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_get_i2t_protection(const vnf9q20f_hw_t *hw, vnf9q20f_inom_t *inom, vnf9q20f_tnom_t *tnom)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    const uint16_t *reg = get_shadow_fsitcr(shadow, hw->channel);
    if (reg == NULL) return -1;

    if (inom != NULL)
    {
        *inom = (vnf9q20f_inom_t)vnf9q20f_fsitcr_get_inom(*reg);
    }
    if (tnom != NULL)
    {
        *tnom = (vnf9q20f_tnom_t)vnf9q20f_fsitcr_get_tnom(*reg);
    }

    return 0;
}

/*============================================================================*/
/* Current Sense Configuration and Reading                                    */
/*============================================================================*/

int vnf9q20f_set_current_sense_mode(const vnf9q20f_hw_t *hw, vnf9q20f_spcr_t spcr)
{
    vnf9q20f_shadow_t *shadow = hw->shadow;

    uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL) return -1;

    uint8_t reg_addr = get_outcfgr_addr(hw->channel);

    spi_lock();

    vnf9q20f_outcfgr_set_spcr(reg, spcr);
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, *reg, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_read_current_sense(const vnf9q20f_hw_t *hw, uint16_t *adc_value)
{
    if (!g_driver_initialized || adc_value == NULL)
    {
        return -1;
    }

    uint8_t reg_addr = get_adcsr_addr(hw->channel);
    uint16_t reg_value;
    int result = vnf9q20f_read_register(hw, reg_addr, &reg_value, NULL);

    if (result == 0)
    {
        /* Extract 10-bit ADC value from bits [13:4] */
        *adc_value = vnf9q20f_adcsr_get_adc_value(reg_value);
    }

    return result;
}

/*
 * ADC to Current Conversion
 *
 * The conversion factor depends on the channel mode:
 *   - LED mode (CCR bit = 1): factor = 308
 *   - BULB mode (CCR bit = 0): factor = 79
 *
 * Formula: I_load = adc_value / conversion_factor
 */
float vnf9q20f_adc_to_current(const vnf9q20f_hw_t *hw, uint16_t adc_value)
{
    if (!g_driver_initialized)
    {
        return 0.0f;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    const uint16_t *reg = get_shadow_outcfgr(shadow, hw->channel);
    if (reg == NULL)
    {
        return 0.0f;
    }

    bool is_led_mode = vnf9q20f_outcfgr_get_ccr(*reg);
    float conversion_factor = is_led_mode ? VNF9Q20F_ADC_TO_CURRENT_FACTOR_LED : VNF9Q20F_ADC_TO_CURRENT_FACTOR_BULB;

    return (float)adc_value / conversion_factor;
}

/*============================================================================*/
/* Status and Fault Reading                                                   */
/*============================================================================*/

// int vnf9q20f_read_channel_faults(const vnf9q20f_hw_t *hw)
// {
//     if (!is_valid_device(hw))
//     {
//         return 0;
//     }

//     const vnf9q20f_hw_t *hw;

//     hw = vnf9q20f_get_hw(hw);

//         vnf9q20f_shadow_t *shadow = hw->shadow;



//     uint16_t status;
//     if (vnf9q20f_read_channel_status(hw, hw->channel, &status, NULL) != 0)
//     {
//         return -1;
//     }

//     //TODO FIX THIS
//     // vnf9q20f_channel_info_t *info = &g_device_state[device_id].channels[channel];

//     /* Update enabled state from shadow */
//     info->enabled = vnf9q20f_get_channel_enable(hw, hw->channel);

//     /* Initialize fault fields */
//     info->fault_type = VNF9Q20F_FAULT_NONE;
//     info->fault_active = false;
//     info->fault_count = 0;
//     info->fault_timestamp_ms = HAL_GetTick();

//     /* Parse status register bits (based on OUTSR register definition) */
//     if (status & 0x0001U)
//     {
//         info->fault_type = VNF9Q20F_FAULT_OVERCURRENT;
//         info->fault_active = true;
//         info->fault_count = 1;
//     }
//     else if (status & 0x0002U)
//     {
//         info->fault_type = VNF9Q20F_FAULT_OVERTEMP;
//         info->fault_active = true;
//         info->fault_count = 1;
//     }
//     else if (status & 0x0004U)
//     {
//         info->fault_type = VNF9Q20F_FAULT_OPEN_LOAD;
//         info->fault_active = true;
//         info->fault_count = 1;
//     }
//     else if (status & 0x0008U)
//     {
//         info->fault_type = VNF9Q20F_FAULT_SHORT_TO_GND;
//         info->fault_active = true;
//         info->fault_count = 1;
//     }
//     else if (status & 0x0010U)
//     {
//         info->fault_type = VNF9Q20F_FAULT_SHORT_TO_VCC;
//         info->fault_active = true;
//         info->fault_count = 1;
//     }

//     return 0;
// }

// int vnf9q20f_read_channel_info(const vnf9q20f_hw_t *hw)
// {
//     if (!is_valid_device(hw))
//     {
//         return 0;
//     }

//     /* Read fault status (stores in g_device_state) */
//     if (vnf9q20f_read_channel_faults(hw, hw->channel) != 0)
//     {
//         return -1;
//     }

//     vnf9q20f_channel_info_t *info = &g_device_state[device_id].channels[channel];

//     /* Read current sense */
//     uint16_t adc_value;
//     if (vnf9q20f_read_current_sense(hw, hw->channel, &adc_value) == 0)
//     {
//         info->adc_raw = adc_value;
//         info->current_a = vnf9q20f_adc_to_current(hw, hw->channel, adc_value);
//         info->current_valid = true;
//         info->current_timestamp_ms = HAL_GetTick();
//     }
//     else
//     {
//         info->adc_raw = 0;
//         info->current_a = 0.0f;
//         info->current_valid = false;
//         info->current_timestamp_ms = 0;
//     }

//     return 0;
// }

// int vnf9q20f_read_channel_status(const vnf9q20f_hw_t *hw,
//                                   uint16_t *status, uint8_t *gsb)
// {
//     if (!g_driver_initialized || 
//         !is_valid_channel(hw->channel) || status == NULL)
//     {
//         return -1;
//     }

//     uint8_t reg_addr = get_outsr_addr(hw->channel);
//     return vnf9q20f_read_register(hw, reg_addr, status, gsb);
// }

// int vnf9q20f_clear_channel_faults(const vnf9q20f_hw_t *hw)
// {
//     if (!is_valid_device(hw))
//     {
//         return 0;
//     }

//     GPIO_TypeDef *cs_port;
//     uint16_t cs_pin;
//     if (get_device_cs(hw, &cs_port, &cs_pin) != 0)
//     {
//         return -1;
//     }

//     uint8_t reg_addr = get_outsr_addr(hw->channel);

//     spi_lock();
//     HAL_StatusTypeDef s = vnf9q20f_spi_read_and_clear(hw, reg_addr, NULL, NULL);
//     spi_unlock();

//     return (s == HAL_OK) ? 0 : -1;
// }

/*============================================================================*/
/* Device Mode Control                                                        */
/*============================================================================*/

int vnf9q20f_software_reset(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    spi_lock();

    uint8_t tx_buf[3] = {0xFF, 0x00, 0x00};
    uint8_t rx_buf[3];

    HAL_StatusTypeDef s = vnf9q20f_spi_transaction(hw, tx_buf, rx_buf);

    spi_unlock();

    if (s == HAL_OK)
    {
        shadow_init_defaults(hw);
    }

    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_enter_normal_mode(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    /* Frame 1: Write CTRL with UNLOCK=1 */
    uint16_t ctrl = 0x4000U;
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CTRL, ctrl, NULL);
    if (s != HAL_OK)
    {
        spi_unlock();
        return -1;
    }

    if (s == HAL_OK)
    {
        shadow->ctrl = ctrl;
    }

    /* Frame 2: Write CTRL with EN=1 */
    ctrl = 0x0800U;
    s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CTRL, ctrl, NULL);

    if (s == HAL_OK)
    {
        shadow->ctrl = ctrl;
    }

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_enter_failsafe_mode(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    vnf9q20f_ctrl_set_en(&shadow->ctrl, false);

    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_CTRL, shadow->ctrl, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

/*============================================================================*/
/* Standby Control (STDBY_NOT Pin)                                            */
/*============================================================================*/

int vnf9q20f_set_stdby_active(const vnf9q20f_hw_t *hw)
{
    /* STDBY_NOT is active low: HIGH = active, LOW = standby */
    HAL_GPIO_WritePin(hw->stdby_port, hw->stdby_pin, GPIO_PIN_SET);
    return 0;
}

int vnf9q20f_set_stdby_standby(const vnf9q20f_hw_t *hw)
{
    /* STDBY_NOT is active low: HIGH = active, LOW = standby */
    HAL_GPIO_WritePin(hw->stdby_port, hw->stdby_pin, GPIO_PIN_RESET);
    return 0;
}

int vnf9q20f_toggle_stdby(const vnf9q20f_hw_t *hw)
{
    HAL_GPIO_TogglePin(hw->stdby_port, hw->stdby_pin);
    return 0;
}

/*============================================================================*/
/* Watchdog Control                                                           */
/*============================================================================*/

int vnf9q20f_toggle_watchdog(const vnf9q20f_hw_t *hw)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    vnf9q20f_shadow_t *shadow = hw->shadow;

    spi_lock();

    vnf9q20f_socr_toggle_watchdog(&shadow->socr);

    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, VNF9Q20F_REG_SOCR, shadow->socr, NULL);

    spi_unlock();
    return (s == HAL_OK) ? 0 : -1;
}

/*============================================================================*/
/* Low-Level Register Access                                                  */
/*============================================================================*/

int vnf9q20f_write_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr, uint16_t data)
{
    if (!g_driver_initialized)
    {
        return -1;
    }

    spi_lock();
    HAL_StatusTypeDef s = vnf9q20f_spi_write_register(hw, reg_addr, data, NULL);
    spi_unlock();

    return (s == HAL_OK) ? 0 : -1;
}

int vnf9q20f_read_register(const vnf9q20f_hw_t *hw, uint8_t reg_addr,
                           uint16_t *data, uint8_t *gsb)
{
    if (!g_driver_initialized || data == NULL)
    {
        return -1;
    }

    spi_lock();
    HAL_StatusTypeDef s = vnf9q20f_spi_read_register(hw, reg_addr, data, gsb);
    spi_unlock();

    return (s == HAL_OK) ? 0 : -1;
}
