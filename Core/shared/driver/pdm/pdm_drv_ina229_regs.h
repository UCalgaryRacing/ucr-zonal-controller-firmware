#ifndef DRIVERS_PDM_DRV_INA229_REGS_H_
#define DRIVERS_PDM_DRV_INA229_REGS_H_

#include "main.h"
#include <math.h>
#include <stdint.h>

/*============================================================================*/
/* Configuration Register                                                     */
/*============================================================================*/
/* Configures how the device operates, i.e., the measurement mode and whether temperature compensation is allowed */
#define INA229_config_register 0x00U
#define INA229_config_register_rst_NormalOperation 0x0000U
#define INA229_config_register_rst_SystemReset 0x8000U
#define INA229_config_register_rstacc_NormalOperation 0x0000U
#define INA229_config_register_rstacc_ClearENERGYandCHARGEregisters 0x4000U
#define INA229_config_register_tempcomp_Shunttemperaturecompensationdisabled 0x0000U
#define INA229_config_register_tempcomp_Shunttemperaturecompensationenabled 0x0020U
#define INA229_config_register_adcrange_16384mV 0x0000U
#define INA229_config_register_adcrange_4096mV 0x0010U


/*============================================================================*/
/* ADC Configuration Register                                                 */
/*============================================================================*/
/* Configures the ADC operation, including conversion time and averaging, should opt for the highest waiting time if data will be updated evry 10ms in fast loop */
#define INA229_adc_config_register 0x01U
#define INA229_adc_config_register_mode_Shutdown0 0x0000U
#define INA229_adc_config_register_mode_Triggeredbusvoltagesingleshot 0x1000U
#define INA229_adc_config_register_mode_Triggeredshuntvoltagesingleshot 0x2000U
#define INA229_adc_config_register_mode_Triggeredshuntvoltageandbusvoltagesingleshot 0x3000U
#define INA229_adc_config_register_mode_Triggeredtemperaturesingleshot 0x4000U
#define INA229_adc_config_register_mode_Triggeredtemperatureandbusvoltagesingleshot 0x5000U
#define INA229_adc_config_register_mode_Triggeredtemperatureandshuntvoltagesingleshot 0x6000U
#define INA229_adc_config_register_mode_Triggeredbusvoltageshuntvoltageandtemperaturesingleshot 0x7000U
#define INA229_adc_config_register_mode_Shutdown1 0x8000U
#define INA229_adc_config_register_mode_Continuousbusvoltageonly 0x9000U
#define INA229_adc_config_register_mode_Continuousshuntvoltageonly 0xA000U
#define INA229_adc_config_register_mode_Continuousshuntandbusvoltage 0xB000U
#define INA229_adc_config_register_mode_Continuoustemperatureonly 0xC000U
#define INA229_adc_config_register_mode_Continuousbusvoltageandtemperature 0xD000U
#define INA229_adc_config_register_mode_Continuoustemperatureandshuntvoltage 0xE000U
#define INA229_adc_config_register_mode_Continuousbusvoltageshuntvoltageandtemperature 0xF000U
#define INA229_adc_config_register_vbusct_50us 0x0000U
#define INA229_adc_config_register_vbusct_84us 0x0200U
#define INA229_adc_config_register_vbusct_150us 0x0400U
#define INA229_adc_config_register_vbusct_280us 0x0600U
#define INA229_adc_config_register_vbusct_540us 0x0800U
#define INA229_adc_config_register_vbusct_1052us 0x0A00U
#define INA229_adc_config_register_vbusct_2074us 0x0C00U
#define INA229_adc_config_register_vbusct_4120us 0x0E00U
#define INA229_adc_config_register_vshct_50us 0x0000U
#define INA229_adc_config_register_vshct_84us 0x0040U
#define INA229_adc_config_register_vshct_150us 0x0080U
#define INA229_adc_config_register_vshct_280us 0x00C0U
#define INA229_adc_config_register_vshct_540us 0x0100U
#define INA229_adc_config_register_vshct_1052us 0x0140U
#define INA229_adc_config_register_vshct_2074us 0x0180U
#define INA229_adc_config_register_vshct_4120us 0x01C0U
#define INA229_adc_config_register_vtct_50us 0x0000U
#define INA229_adc_config_register_vtct_84us 0x0008U
#define INA229_adc_config_register_vtct_150us 0x0010U
#define INA229_adc_config_register_vtct_280us 0x0018U
#define INA229_adc_config_register_vtct_540us 0x0020U
#define INA229_adc_config_register_vtct_1052us 0x0028U
#define INA229_adc_config_register_vtct_2074us 0x0030U
#define INA229_adc_config_register_vtct_4120us 0x0038
#define INA229_adc_config_register_avg_1 0x0000U
#define INA229_adc_config_register_avg_4 0x0001U
#define INA229_adc_config_register_avg_16 0x0002U
#define INA229_adc_config_register_avg_64 0x0003U
#define INA229_adc_config_register_avg_128 0x0004U
#define INA229_adc_config_register_avg_256 0x0005U
#define INA229_adc_config_register_avg_512 0x0006U
#define INA229_adc_config_register_avg_1024 0x0007U




/*============================================================================*/
/* Shunt Calibration Register                                                 */
/*============================================================================*/
/* Used to enter the value of the shunt resistance attched to the GLV bus and setermines the value of the current measurement */
#define INA229_shunt_cal_register 0x02U
#define INA229_shunt_cal_register_reserved0_ENABLE 0x8000U
#define INA229_shunt_cal_register_reserved0_DISABLE 0x0000U


/*============================================================================*/
/* Addresses for read-only registers                                          */
/*============================================================================*/
/*These register are only meant to be read and return information such as charge, voltage and charge.*/
#define INA229_shunt_tempco_register 0x03U

#define INA229_vshunt_register 0x04U

#define INA229_vbus_register 0x05U

#define INA229_dietemp_register 0x06U

#define INA229_current_register 0x07U

#define INA229_power_register 0x08U

#define INA229_energy_register 0x09U

#define INA229_charge_register 0x0AU

/*============================================================================*/
/* Diagnostics alert register                                                 */
/*============================================================================*/
/* reading this register informs us whether some fault was detected in the system relating to the registers defined below */
#define INA229_diag_alrt_register 0x0BU
#define INA229_diag_alrt_register_alatch_Transparent 0x0000U
#define INA229_diag_alrt_register_alatch_LatchedAlertpin 0x8000U
#define INA229_diag_alrt_register_cnvr_DisableconversionreadyflagonALERTpin 0x0000U
#define INA229_diag_alrt_register_cnvr_EnablesconversionreadyflagonALERTpin 0x4000U
#define INA229_diag_alrt_register_slowalert_ALERTcomparisononnonaveragedADCvalue 0x0000U
#define INA229_diag_alrt_register_slowalert_ALERTcomparisononaveragedvalue 0x2000U
#define INA229_diag_alrt_register_apol_Normalactivelowopendrain 0x0000U
#define INA229_diag_alrt_register_apol_Invertedactivehighopendrain 0x1000U
#define INA229_diag_alrt_register_energyof_ENABLE 0x0800U
#define INA229_diag_alrt_register_energyof_DISABLE 0x0000U
#define INA229_diag_alrt_register_chargeof_ENABLE 0x0400U
#define INA229_diag_alrt_register_chargeof_DISABLE 0x0000U
#define INA229_diag_alrt_register_mathof_ENABLE 0x0200U
#define INA229_diag_alrt_register_mathof_DISABLE 0x0000U
#define INA229_diag_alrt_register_reserved0_ENABLE 0x0100U
#define INA229_diag_alrt_register_reserved0_DISABLE 0x0000U
#define INA229_diag_alrt_register_tmpol_ENABLE 0x0080U
#define INA229_diag_alrt_register_tmpol_DISABLE 0x0000U
#define INA229_diag_alrt_register_shntol_ENABLE 0x0040U
#define INA229_diag_alrt_register_shntol_DISABLE 0x0000U
#define INA229_diag_alrt_register_shntul_ENABLE 0x0020U
#define INA229_diag_alrt_register_shntul_DISABLE 0x0000U
#define INA229_diag_alrt_register_busol_ENABLE 0x0010U
#define INA229_diag_alrt_register_busol_DISABLE 0x0000U
#define INA229_diag_alrt_register_busul_ENABLE 0x0008U
#define INA229_diag_alrt_register_busul_DISABLE 0x0000U
#define INA229_diag_alrt_register_pol_ENABLE 0x0004U
#define INA229_diag_alrt_register_pol_DISABLE 0x0000U
#define INA229_diag_alrt_register_cnvrf_ENABLE 0x0002U
#define INA229_diag_alrt_register_cnvrf_DISABLE 0x0000U
#define INA229_diag_alrt_register_memstat_ENABLE 0x0001U
#define INA229_diag_alrt_register_memstat_DISABLE 0x0000U




/*============================================================================*/
/* Limit setting register addresses                                           */
/*============================================================================*/
/* Allows the setting of user-defined limits for several paramters relating to the GLV bus that is being monitored.*/

#define INA229_sovl_register 0x0CU

#define INA229_suvl_register 0x0DU

#define INA229_bovl_register 0x0EU
#define INA229_bovl_register_reserved0_ENABLE 0x8000U
#define INA229_bovl_register_reserved0_DISABLE 0x0000U

#define INA229_buvl_register 0x0FU
#define INA229_buvl_register_reserved0_ENABLE 0x8000U
#define INA229_buvl_register_reserved0_DISABLE 0x0000U

#define INA229_temp_limit_register 0x10U

#define INA229_pwr_limit_register 0x11U

#define INA229_manufacturer_id_register 0x3EU

#define INA229_device_id_register 0x3FU


typedef struct INA229_Shadow_regs_t {

    uint16_t configRegister;
    uint16_t adcConfigRegister;
    uint16_t shuntCalRegister;
    uint16_t shuntTempcoRegister;
    uint16_t diagAlrtRegister;
    uint16_t sovlRegister;
    uint16_t suvlRegister;
    uint16_t bovlRegister;
    uint16_t buvlRegister;
    uint16_t tempLimitRegister;
    uint16_t pwrLimitRegister;
    uint16_t adcrange;

} INA229_Shadow_regs_t;

/*
 *  ======== SPI_configure_default ========
 *  Configure SPI for default settings
 */
void SPI_configure_default(void);

/*
 *  ======== SPI_configure_for_INA229 ========
 *  Configure SPI for INA229 specific requirements
 */
void SPI_configure_for_INA229(void);

HAL_StatusTypeDef SPI_transmit_receive_INA229(uint8_t* txBuf, uint8_t* rxBuf, uint16_t size);

/*
 *  ======== INA229_writeReg ========
 *  Write register
 */
 void INA229_writeReg(INA229_Shadow_regs_t* Ptr_INA229, uint8_t regAddr, uint16_t value);

/*
 *  ======== INA229_readReg ========
 *  Read register
 */
 uint64_t INA229_readReg(INA229_Shadow_regs_t* Ptr_INA229, uint8_t regAddr);

// /*
//  *  ======== INA229_config ========
//  *  Configure device with current settings
//  */
//  void INA229_config(INA229_Shadow_regs_t* Ptr_INA229);


void INA229_Set_SHUNTCAL(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_SHUNTCAL(INA229_Shadow_regs_t* Ptr_INA229);

/*
sets the shuntcal value, depends on:
MAX CURRENT
SHUNT RESISTANCE
ADC MODE
 */
void INA229_Set_SOVL(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_SOVL(INA229_Shadow_regs_t* Ptr_INA229);

/*
sets/gets the Shunt under-voltage value, depends on:
MIN CURRENT
SHUNT RESISTANCE
ADC MODE
 */

void INA229_Set_SUVL(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_SUVL(INA229_Shadow_regs_t* Ptr_INA229);

/*
sets/gets the Bus Over-voltage value, depends on:
MAX_BUS_VOLTAGE
 */

void INA229_Set_BOVL(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_BOVL(INA229_Shadow_regs_t* Ptr_INA229);

/*
sets/gets the Bus Over-voltage value, depends on:
MIN_BUS_VOLTAGE
 */
void INA229_Set_BUVL(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_BUVL(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_Set_adcrange ========
 *  Set the ADC range (depends on )
 */
void INA229_Set_adcrange(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_adcrange(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_Set_ADCmode ========
 *  Set the ADC mode
 */
void INA229_Set_ADCmode(INA229_Shadow_regs_t* Ptr_INA229);
void INA229_Get_ADCmode(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_getCURRENT_LSB ========
 *  Get the CURRENT_LSB value used for calculations
 */
float INA229_getCURRENT_signedLSB(INA229_Shadow_regs_t* Ptr_INA229);





#endif /* DRIVERS_PDM_DRV_INA229_REGS_H_ */

