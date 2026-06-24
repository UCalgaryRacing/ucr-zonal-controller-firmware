#ifndef DRIVERS_PDM_DRV_INA229_H_
#define DRIVERS_PDM_DRV_INA229_H_

#include "pdm_drv_ina229_regs.h"
#include "pdm_config.h"


/* ADC LSB (Resolution) Values */

// Shunt Voltage LSB (nanovolts)
#define SHUNT_VOLTAGE_LSB_RANGE0_NV    312.5f      // ADCRANGE = 0
#define SHUNT_VOLTAGE_LSB_RANGE1_NV    78.125f     // ADCRANGE = 1

// Bus Voltage LSB (microvolts)
#define BUS_VOLTAGE_LSB_UV             195.3125f

// Temperature LSB (milli-degrees Celsius)
#define TEMPERATURE_LSB_MC             7.8125f


extern float INA229_getVSHUNT_mV(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_getVBUS_V ========
 *  Get VBUS value (V)
 */
extern float INA229_getVBUS_V(INA229_Shadow_regs_t* Ptr_INA229);


/*
 *  ======== INA229_getCURRENT_A ========
 *  Get CURRENT value (A)
 */
extern float INA229_getCURRENT_A(INA229_Shadow_regs_t* Ptr_INA229);




/*
 *  ======== INA229_getTEMPERATURE_C ========
 *  Get TEMPERATURE value (degrees Celsius)
 */
float INA229_getTEMPERATURE_C(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_getPOWER_W ========
 *  Get POWER value (Watts)
 */
float INA229_getPOWER_W(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_getCHARGE_Ah ========
 *  Get CHARGE value (Amp-hours)
 */
float INA229_getCHARGE_Ah(INA229_Shadow_regs_t* Ptr_INA229);


/*
 *  ======== INA229_getEnergy_Wh ========
 *  Get ENERGY value (Watt-hours)
 */
float INA229_getEnergy_Wh(INA229_Shadow_regs_t* Ptr_INA229);

/*
 *  ======== INA229_configure ========
 *  Configure the INA229 device, set the values of the shadow registers and write them to the device.
 */

void INA229_configure(ina229_hw_t* ina229_hw);

/*
 *  ======== INA229_check_hw_status ========
 *  Check the hardware status of the INA229 device and update the enum in the data type.
 */
void INA229_check_hw_status(ina229_hw_t* ina229_hw);


#endif /* DRIVERS_PDM_DRV_INA229_H_ */

