#include "pdm_config.h"
#include "pdm_drv_ina229.h"
#include "pdm_drv_ina229_regs.h"
#include <stdbool.h>


bool g_INA229_configured = false;


float INA229_getVSHUNT_mV(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_vshunt_register);
    float data;

    //Remove reserved bits
    value = value >> 4;

    //Convert for 2's compliment and signed value
    if (value > 0x7FFFF) {
        data = (float) value - 0x100000;
    } else {
        data = (float) value;
    }

    //Convert to mV

    if (Ptr_INA229->adcrange == INA229_config_register_adcrange_4096mV) {
        data = (data * SHUNT_VOLTAGE_LSB_RANGE1_NV) / 1000000;
    } else {
        data = (data * SHUNT_VOLTAGE_LSB_RANGE0_NV) / 1000000;
    }

    return data;
}

/*
 *  ======== INA229_getVBUS_V ========
 *  Get VBUS value (V)
 */
float INA229_getVBUS_V(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_vbus_register);
    float data;

    //Remove reserved bits
    value = value >> 4;

    //Convert for 2's compliment and signed value (though always positive)
    if (value > 0x7FFFF) {
        data = (float) value - 0x100000; //left for redundancy and error checking, should never get used
    } else {
        data = (float) value;
    }

    //Convert to V
    data = (data * BUS_VOLTAGE_LSB_UV) / 1000000;

    return data;
}


/*
 *  ======== INA229_getCURRENT_signedLSB ========
 *  Get CURRENT value (signed value in LSBs)
 */


/*
 *  ======== INA229_getCURRENT_A ========
 *  Get CURRENT value (A)
 */
float INA229_getCURRENT_A(INA229_Shadow_regs_t* Ptr_INA229) {
    float data = INA229_getCURRENT_signedLSB(Ptr_INA229);

    data = data * PDM_INA229_CURRENT_LSB_A;

    return data;
}

float INA229_getTEMPERATURE_C(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_dietemp_register);
    float data;

    //Convert to degrees Celsius
    data = (value * TEMPERATURE_LSB_MC) / 1000;

    return data;
}

float INA229_getPOWER_W(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_power_register);

    float datasheet_constant = 3.2f; // from section 8.12 of the datasheet.
    float data;

    //Convert to W
    data = (value * PDM_INA229_CURRENT_LSB_A *datasheet_constant);

    return data;
}

float INA229_getCHARGE_Ah(INA229_Shadow_regs_t* Ptr_INA229) {
    
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_charge_register);

    float data = (PDM_INA229_CURRENT_LSB_A * value * (1/3600.0f)); // convert from coulombs to amp-hours

    return data;

}

float INA229_getEnergy_Wh(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_energy_register);
    float datasheet_constant = 3.2f * 16.0f; // from section 8.12 of the datasheet.

    float data = PDM_INA229_CURRENT_LSB_A * value * datasheet_constant*(1/3600.0f); // convert from jouls to Watt-hours

    return data;
}

void INA229_configure(ina229_hw_t* ina229_hw){

    INA229_Shadow_regs_t* Ptr_INA229 = ina229_hw->INA229_Shadow_regs_handle;

    Ptr_INA229->adcrange = INA229_config_register_adcrange_16384mV;

    Ptr_INA229->configRegister =
          INA229_config_register_tempcomp_Shunttemperaturecompensationdisabled |
          INA229_config_register_rst_NormalOperation|
          INA229_config_register_tempcomp_Shunttemperaturecompensationenabled|
		  INA229_config_register_adcrange_16384mV;

          //default config reg value. adc range 1 selected, normal operation and temp compensation disabled

    Ptr_INA229->adcConfigRegister =
          INA229_adc_config_register_avg_128 |
          INA229_adc_config_register_mode_Continuousshuntandbusvoltage |
          INA229_adc_config_register_vshct_1052us |
          INA229_adc_config_register_vbusct_1052us;
          // default adc config reg value, set to 128 averages, continuous shunt and bus measurement, 1.1ms conversion time for both shunt and bus voltage.


    Ptr_INA229->diagAlrtRegister =
          INA229_diag_alrt_register_alatch_Transparent |
          INA229_diag_alrt_register_cnvr_DisableconversionreadyflagonALERTpin |
          INA229_diag_alrt_register_apol_Normalactivelowopendrain |
          INA229_diag_alrt_register_slowalert_ALERTcomparisononaveragedvalue;
          
    //TODO maybe have a more user-friendly way of adjusting these settings, perhaps through an enum.

    //INA229_writeReg(Ptr_INA229, INA229_config_register, Ptr_INA229->configRegister);
    
    INA229_writeReg(Ptr_INA229, INA229_adc_config_register, Ptr_INA229->adcConfigRegister);
    
    INA229_writeReg(Ptr_INA229, INA229_shunt_cal_register, Ptr_INA229->shuntCalRegister);
    
    INA229_Set_SHUNTCAL(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_shunt_cal_register, Ptr_INA229->shuntCalRegister);
    
    INA229_Set_SOVL(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_sovl_register, Ptr_INA229->sovlRegister);

    INA229_Set_SUVL(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_suvl_register, Ptr_INA229->suvlRegister);

    INA229_Set_BOVL(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_bovl_register, Ptr_INA229->bovlRegister);

    INA229_Set_BUVL(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_buvl_register, Ptr_INA229->buvlRegister);

    //INA229_Set_DIAG_ALERT(Ptr_INA229);
    INA229_writeReg(Ptr_INA229, INA229_diag_alrt_register, Ptr_INA229->diagAlrtRegister);

    // small delay to prevent sending out too many SPI messages at once, had issues with configuring in the past so this might be necessary.





    //TODO: add a function for reading the spi messages of the INA229 and use that to verify that the configuration was successful.

}


void INA229_check_hw_status(ina229_hw_t* ina229_hw){

    INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_sovl_register);
    INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_suvl_register);
    INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_bovl_register); 
    INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_buvl_register);

    //TODO slightly modify function to utilize other enum value for different states of the chip.
    if (
        INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_adc_config_register) != ina229_hw->INA229_Shadow_regs_handle->adcConfigRegister ||
        INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_shunt_cal_register) != ina229_hw->INA229_Shadow_regs_handle->shuntCalRegister ||
        INA229_readReg(ina229_hw->INA229_Shadow_regs_handle, INA229_config_register) != ina229_hw->INA229_Shadow_regs_handle->configRegister
    ){
            ina229_hw->hw_status = INA229_NOT_CONFIGURED_CORRECTLY;
    } 
    
    else {
        ina229_hw->hw_status = INA229_OPERATIONAL;
    }

}




