
 
 #ifndef CONFIG_PDM_CONFIG_H_
 #define CONFIG_PDM_CONFIG_H_
 
 
 #include <stdint.h>
 #include <stdbool.h>
 #include "com_typ_common.h"
 #include "pdm_channel_types.h"
 #include "stm32h7xx_hal.h"
#include "pdm_drv_ina229_regs.h"

/*============================================================================*/
 /* Front Controler Mapping                                                    */
 /*============================================================================*/
 #define SDC_START          VBATT_SW_3 
 #define STEERING_WHEEL     VBATT_SW_10 
 #define KVASER             VBATT_SW_11
 #define IMD_LIGHT          VBATT_SW_14
 #define BMS_LIGHT          VBATT_SW_16
 #define RTD_LIGHT          VBATT_SW_15

/*============================================================================*/
 /* Current Sense ADC Configuration                                            */
/*============================================================================*/
 
 #define TPS4XXXX_ADC_VREF_V              2.5f
 
 #define TPS4XXXX_ADC_RESOLUTION_BITS     12U
 
 #define TPS4XXXX_ADC_MAX_COUNT           ((1U << TPS4XXXX_ADC_RESOLUTION_BITS) - 1U)
 
 
 
 /*============================================================================*/
 /* TPS4XXXX Hardware Values                                                   */
 /*============================================================================*/
 
 
 /* TPS4H160 external current sense resistor R(CS) (ohms) */
 #define TPS4H160_CS_RESISTOR_OHMS    100U
 
 /* TPS4H160 external current limit resistor R(CL) (ohms) */
 #define TPS4H160_CL_RESISTOR_OHMS    499U
 
 
 
 /* TPS4H000 external current sense resistor R(CS) (ohms) */
 #define TPS4H000_CS_RESISTOR_OHMS    100U
 
 /* TPS4H000 external current limit resistor R(CL) (ohms). 0 = use internal limit. */
 
 #define TPS4H000_CL_RESISTOR_OHMS    0U
 
 
 /*============================================================================*/
 /* TPS4H000-Q1 IC Characteristics (from datasheet)                            */
 /*============================================================================*/
 
 /* Typical on-resistance per channel (mΩ) */
 #define TPS4H000_RON_MOHM            1000U
 
 /* Current sense ratio K(CS) */
 #define TPS4H000_K_CS                80U
 
 /* Current limit ratio K(CL) */
 #define TPS4H000_K_CL                300U
 
 /* Internal current limit when CL tied to GND (A) */
 #define TPS4H000_ICL_INT_A           2.0f
 
 
 /*============================================================================*/
 /* TPS4H160-Q1 IC Characteristics (from datasheet)                            */
 /*============================================================================*/
 
 /* Typical on-resistance per channel (mΩ) */
 #define TPS4H160_RON_MOHM            160U
 
 /* Current sense ratio K(CS) */
 #define TPS4H160_K_CS                300U
 
 /* Current limit ratio K(CL) */
 #define TPS4H160_K_CL                2500U
 
 /* Internal current limit when CL tied to GND (A) */
 #define TPS4H160_ICL_INT_A           8.0f
 
 
 
 /*============================================================================*/
 /* Voltage Limits                                                             */
 /*============================================================================*/

 /* GLV bus overvoltage threshold (V) */
 #define PDM_GLV_OVERVOLTAGE_V         16.8f
 
 /* GLV bus undervoltage threshold (V) */
 #define PDM_GLV_UNDERVOLTAGE_V        10.0f

 /* GLV bus overcurrent threshold (A) */
 #define PDM_GLV_OVERCURRENT_A         37.0f // value chosen as this much current isn't expected under normal conditions but won't trip any fuses.
  /* GLV bus undercurrent threshold (A) */
 #define PDM_GLV_UNDERCURRENT_A        0.2f // value chosen as the quiscent draw is nominally around 300mA when nothing else is powered except the backplane controller that the pdm attaches to
 
 /*============================================================================ */
 /* INA229 Hardware Values                                                      */
 /*============================================================================ */

 #define PDM_INA229_SHUNT_RESISTOR_OHMS   0.001f
 #define PDM_INA229_CURRENT_LSB_A         (PDM_GLV_OVERCURRENT_A / (pow(2, 19))) // from section 8 of the datasheet, using the maximum expected current to determine the LSB for the current register.
 
 /*============================================================================*/
 /* Device Topology                                                            */
 /*============================================================================*/
 
 /*
  *  - Device 0: Output channels 9-12 (TPS4H160)
  *  - Device 1: Output channels 13-16 (TPS4H000)
  *  - Device 2: Regulated 12V channels 17-20 (TPS4H160)
  */
 #define TPS4XXXX_NUM_DEVICES         3U
 
 #define TPS4XXXX_CHANNELS_PER_IC     4U
 
 #define TPS4XXXX_TOTAL_CHANNELS      (TPS4XXXX_NUM_DEVICES * TPS4XXXX_CHANNELS_PER_IC)
 
 
 #define VNF9Q20F_NUM_DEVICES          2U
 
 #define VNF9Q20F_CHANNELS_PER_IC      4U
 
 #define VNF9Q20F_TOTAL_CHANNELS       (VNF9Q20F_NUM_DEVICES * VNF9Q20F_CHANNELS_PER_IC)
 
 
 /*============================================================================*/
 /* System Topology                                                            */
 /*============================================================================*/
 
 /*
  * Channel mapping (1-based numbering, 0-based array index):
  *   - Channels 1-4:   VNF9Q20F device 0 (8 channels total from 2 ICs)
  *   - Channels 5-8:   VNF9Q20F device 1
  *   - Channels 9-12:  TPS4H160 device 0 (12 channels total from 3 ICs)
  *   - Channels 13-16: TPS4H160 device 1
  *   - Channels 17-20: TPS4H160 device 2 (Regulated 12V)
  */
 #define PDM_TOTAL_CHANNELS            (VNF9Q20F_TOTAL_CHANNELS + TPS4XXXX_TOTAL_CHANNELS)
 
 
 /*============================================================================*/
 /* VNF9Q20F Settings Configuration                                            */
 /*============================================================================*/
 
 typedef struct {
     pdm_channel_mode_t mode;
     pdm_channel_ccm_t ccm;
     pdm_channel_i2t_params_t i2t;
     pdm_channel_pwm_config_t pwm;
 } vnf9q20f_config_t;
 
 
 
 /*============================================================================*/
 /* TPS4XXXX Settings Configuration                                            */
 /*============================================================================*/
 
 typedef struct {
     pdm_channel_pwm_config_t pwm;
     uint16_t      r_cs;
     uint16_t      r_cl;
 } tps4xxxx_config_t;
 
 
 /*============================================================================*/
 /* Device Handle and Shadow Registers                                         */
 /*============================================================================*/
 
 /*
  * Shadow of VNF9Q20F control registers from section 4.5 of the datasheet.
  * Holds the last known/intended register payloads for efficient
  * read-modify-write operations.
  * 
  * Contains the following control registers (32 bytes total):
  * - OUTCTRCR0-3: Output control configuration registers (channels 0-3)
  * - OUTCFGR0-3: Output configuration registers (channels 0-3)
  * - CHLOFFTCR0-1: Channel latch-off timing control registers
  * - SOCR: Channel control register
  * - CTRL: Control register
  * - FSITCR0-3: Fail-safe and I²t settings registers (channels 0-3)
  */
 typedef struct
 {
     /* Output Control Configuration Registers (Section 4.5.1) */
     uint16_t outctrcr0;
     uint16_t outctrcr1;
     uint16_t outctrcr2;
     uint16_t outctrcr3;
     
     /* Output Configuration Registers (Section 4.5.2) */
     uint16_t outcfgr0;
     uint16_t outcfgr1;
     uint16_t outcfgr2;
     uint16_t outcfgr3;
     
     /* Channel Latch-Off Timer Control Registers (Section 4.5.3) */
     uint16_t chlofftcr0;
     uint16_t chlofftcr1;
     
     /* Channel Control Register (Section 4.5.4) */
     uint16_t socr;
     
     /* Control Register (Section 4.5.5) */
     uint16_t ctrl;
     
     /* Fail-safe and I²t Configuration Registers (Section 4.5.6) */
     uint16_t fsitcr0;
     uint16_t fsitcr1;
     uint16_t fsitcr2;
     uint16_t fsitcr3;
 } vnf9q20f_shadow_t;
 
 
 
 /*============================================================================*/
 /* Channel Hardware Mapping                                                   */
 /*============================================================================*/
 
 typedef struct {
     ADC_HandleTypeDef *adc_handle;
     GPIO_TypeDef *input_port;
     uint16_t      input_pin;       
     GPIO_TypeDef *diag_port;
     uint16_t      diag_pin;         
     GPIO_TypeDef *seh_port;
     uint16_t      seh_pin;  
     GPIO_TypeDef *sel_port;
     uint16_t      sel_pin;     
     GPIO_TypeDef *fault_port;
     uint16_t      fault_pin;    
 } tps4xxxx_hw_t;
 
 typedef struct {
     SPI_HandleTypeDef *spi_handle;
     GPIO_TypeDef *cs_port;
     uint16_t      cs_pin;
     GPIO_TypeDef *stdby_port;
     uint16_t      stdby_pin;
     uint8_t       ic;
     uint8_t       channel;
     vnf9q20f_shadow_t *shadow;
 } vnf9q20f_hw_t;
 
 void PDM_INA229_Init(void);
 void PDM_INA229_update_glv_voltage(float* voltage_v);
 void PDM_INA229_update_glv_current(float* current_a);
 void PDM_INA229_update_glv_power(float* power_w);
 void PDM_INA229_update_glv_temperature(float* temperature_c);
 void PDM_INA229_update_glv_charge(float* charge_c);
 void PDM_INA229_update_glv_energy(float* energy_wh);

 typedef enum{
    INA229_OPERATIONAL = 0,
    INA229_NOT_RESPONDING,
    INA229_NOT_CONFIGURED_CORRECTLY,
    INA229_START_UP
 } ina229_hw_status_t;

  typedef struct{
    ina229_hw_status_t hw_status;
    INA229_Shadow_regs_t* INA229_Shadow_regs_handle;
} ina229_hw_t;


 /*============================================================================*/
 /* Complete Channel Configuration Structure                                   */
 /*============================================================================*/
 
 typedef struct
 {
     pdm_channel_type_t ic_type;
     bool enabled;
     bool is_critical;
     union {
         tps4xxxx_config_t tps4xxxx_config;
         vnf9q20f_config_t vnf9q20f_config;
     } device_settings;
     union {
         tps4xxxx_hw_t tps4xxxx;
         vnf9q20f_hw_t vnf9q20f;
     } hw;

 } pdm_channel_config_t;
 
 
 extern const pdm_channel_config_t pdm_default_config[PDM_TOTAL_CHANNELS];
 
 #endif /* CONFIG_PDM_CONFIG_H_ */
