#include "pdm_config_pinout.h"
#include "pdm_config.h"
#include "pdm_drv_ina229.h"
//TODO move these into static definitions in vnf9 driver file, similar to analog adc dma and CAN service
static vnf9q20f_shadow_t vnf9q20f_1_shadow = {0};
static vnf9q20f_shadow_t vnf9q20f_2_shadow = {0};

/*============================================================================*/
/* INA229 configuration                                                       */
/*============================================================================*/

static INA229_Shadow_regs_t rco_pdm_INA229 = {0};
static ina229_hw_t ina229_hw = {
    .hw_status = INA229_START_UP,
    .INA229_Shadow_regs_handle = &rco_pdm_INA229
};

// all the other registers can be set using simple parameters like bus voltage/current limits, these 2 have to be set depending on desired
// status of INA229. (adc range and conversion types and conversion type/averaging amounts.)

void PDM_INA229_Init(void) 
{
    INA229_configure(&ina229_hw);
    INA229_check_hw_status(&ina229_hw);
}

void PDM_INA229_update_glv_voltage(float* voltage_v){
    *voltage_v = INA229_getVBUS_V(ina229_hw.INA229_Shadow_regs_handle);
}

void PDM_INA229_update_glv_current(float* current_a){
    *current_a = INA229_getCURRENT_A(ina229_hw.INA229_Shadow_regs_handle);
}

void PDM_INA229_update_glv_power(float* power_w){
    *power_w = INA229_getPOWER_W(ina229_hw.INA229_Shadow_regs_handle);
}

void PDM_INA229_update_glv_temperature(float* temperature_c){
    *temperature_c = INA229_getTEMPERATURE_C(ina229_hw.INA229_Shadow_regs_handle);
}

void PDM_INA229_update_glv_charge(float* charge_c){
    *charge_c = INA229_getCHARGE_Ah(ina229_hw.INA229_Shadow_regs_handle);
}

void PDM_INA229_update_glv_energy(float* energy_wh){
    *energy_wh = INA229_getEnergy_Wh(ina229_hw.INA229_Shadow_regs_handle);
}


/*============================================================================*/
/* Channel Configuration Table                                                */
/*============================================================================*/

const pdm_channel_config_t pdm_default_config[PDM_TOTAL_CHANNELS] =
{
    /*========================================================================*/
    /* VNF9Q20F Device 0 - Channels 1-4                                       */
    /*========================================================================*/

    /*------------------------------------------------------------------------*/
    /* Channel 1: VNF9Q20F device 0, IC channel 0                             */
    /*------------------------------------------------------------------------*/

    [VBATT_SW_1] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                //add pwm and all other configurable settings in same style as CCM / MODE, look at vnf9q20f_regs
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_0_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_0_CSN_Pin,
                .ic     = 0,
                .channel = 0,
                .shadow = &vnf9q20f_1_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 2: VNF9Q20F device 0, IC channel 1                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_2] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_0_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_0_CSN_Pin,
                .ic     = 0,
                .channel = 1,
                .shadow = &vnf9q20f_1_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 3: VNF9Q20F device 0, IC channel 2                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_3] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_0_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_0_CSN_Pin,
                .ic     = 0,
                .channel = 2,
                .shadow = &vnf9q20f_1_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 4: VNF9Q20F device 0, IC channel 3                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_4] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_0_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_0_CSN_Pin,
                .ic     = 0,
                .channel = 3,
                .shadow = &vnf9q20f_1_shadow
            }
        }
    },

    /*========================================================================*/
    /* VNF9Q20F Device 1 - Channels 5-8                                       */
    /*========================================================================*/

    /*------------------------------------------------------------------------*/
    /* Channel 5: VNF9Q20F device 1, IC channel 0                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_5] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_1_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_1_CSN_Pin,
                .ic     = 1,
                .channel = 0,
                .shadow = &vnf9q20f_2_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 6: VNF9Q20F device 1, IC channel 1                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_6] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_1_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_1_CSN_Pin,
                .ic     = 1,
                .channel = 1,
                .shadow = &vnf9q20f_2_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 7: VNF9Q20F device 1, IC channel 2                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_7] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_1_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_1_CSN_Pin,
                .ic     = 1,
                .channel = 2,
                .shadow = &vnf9q20f_2_shadow
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 8: VNF9Q20F device 1, IC channel 3                             */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_8] = {
        .ic_type     = PDM_CHANNEL_TYPE_VNF9Q20F,
        .is_critical = true,
        .enabled     = true,
        .device_settings = {
            .vnf9q20f_config = {
                .mode = PDM_CHANNEL_MODE_RESISTIVE,
                .ccm  = PDM_CHANNEL_CCM_DISABLED,
                
                .i2t = {
                    .inom_a  = 10.0f,
                    .tnom_ms = 100
                },
            }
        },
        .hw = {
            .vnf9q20f = {
                .spi_handle       = PDM_SPI_HANDLE,
                .cs_port = PDM_VNF9Q20F_1_CSN_GPIO_Port,
                .cs_pin = PDM_VNF9Q20F_1_CSN_Pin,
                .ic     = 1,
                .channel = 3,
                .shadow = &vnf9q20f_2_shadow
            }
        }
    },

    /*========================================================================*/
    /* TPS4H160 Device 0 - Channels 9-12                                      */
    /*========================================================================*/

    /*------------------------------------------------------------------------*/
    /* Channel 9: TPS4H160 device 0, IC channel 0 (IN1)                       */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_9] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS
            }
        },
        .hw = {
            .tps4xxxx = {
                .adc_handle   = PDM_ADC_HANDLE,
                .input_port   = PDM_TPS4XXXX_0_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_0_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_0_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_0_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_0_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_0_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_0_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_0_SEL_Pin, 
                .fault_port = PDM_TPS4XXXX_0_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_0_FAULT_Pin
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 10: TPS4H160 device 0, IC channel 1 (IN2)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_10] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_0_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_0_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_0_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_0_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_0_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_0_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_0_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_0_SEL_Pin,  
                .fault_port = PDM_TPS4XXXX_0_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_0_FAULT_Pin               
            }
        }
    },


    /*------------------------------------------------------------------------*/
    /* Channel 11: TPS4H160 device 0, IC channel 2 (IN3)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_11] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS 
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_0_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_0_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_0_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_0_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_0_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_0_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_0_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_0_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_0_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_0_FAULT_Pin  
            }
        }
    },


    /*------------------------------------------------------------------------*/
    /* Channel 12: TPS4H160 device 0, IC channel 3 (IN4)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_12] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_0_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_0_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_0_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_0_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_0_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_0_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_0_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_0_SEL_Pin,    
                .fault_port = PDM_TPS4XXXX_0_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_0_FAULT_Pin
            }
        }
    },


    /*========================================================================*/
    /* TPS4H000 Device 1 - Channels 13-16                                     */
    /*========================================================================*/

    /*------------------------------------------------------------------------*/
    /* Channel 13: TPS4H000 device 1, IC channel 0 (IN1)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_13] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H000,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H000_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H000_CL_RESISTOR_OHMS 
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_1_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_1_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_1_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_1_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_1_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_1_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_1_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_1_SEL_Pin,  
                .fault_port = PDM_TPS4XXXX_1_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_1_FAULT_Pin
            }
        }
    },


    /*------------------------------------------------------------------------*/
    /* Channel 14: TPS4H000 device 1, IC channel 1 (IN2)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_14] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H000,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H000_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H000_CL_RESISTOR_OHMS  
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_1_IN2_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_1_IN2_Pin,
                .diag_port = PDM_TPS4XXXX_1_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_1_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_1_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_1_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_1_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_1_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_1_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_1_FAULT_Pin   
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 15: TPS4H000 device 1, IC channel 2 (IN3)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_15] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H000,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H000_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H000_CL_RESISTOR_OHMS   
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_1_IN3_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_1_IN3_Pin,
                .diag_port = PDM_TPS4XXXX_1_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_1_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_1_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_1_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_1_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_1_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_1_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_1_FAULT_Pin  
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 16: TPS4H000 device 1, IC channel 3 (IN4)                      */
    /*------------------------------------------------------------------------*/
    [VBATT_SW_16] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H000,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H000_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H000_CL_RESISTOR_OHMS
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_1_IN4_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_1_IN4_Pin,
                .diag_port = PDM_TPS4XXXX_1_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_1_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_1_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_1_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_1_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_1_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_1_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_1_FAULT_Pin
            }
        }
    },

    /*========================================================================*/
    /* TPS4H160 Device 2 - Regulated 12V Channels 17-20                       */
    /*========================================================================*/

    /*------------------------------------------------------------------------*/
    /* Channel 17: TPS4H160 device 2, IC channel 0 (IN1) - Regulated 12V      */
    /*------------------------------------------------------------------------*/
    [VBATT_REG_SW_1] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS 
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_2_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_2_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_2_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_2_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_2_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_2_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_2_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_2_SEL_Pin,  
                .fault_port = PDM_TPS4XXXX_2_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_2_FAULT_Pin 
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 18: TPS4H160 device 2, IC channel 1 (IN2) - Regulated 12V      */
    /*------------------------------------------------------------------------*/
    [VBATT_REG_SW_2] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS 
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_2_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_2_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_2_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_2_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_2_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_2_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_2_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_2_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_2_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_2_FAULT_Pin
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 19: TPS4H160 device 2, IC channel 2 (IN3) - Regulated 12V      */
    /*------------------------------------------------------------------------*/
    [VBATT_REG_SW_3] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS 
            } 
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_2_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_2_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_2_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_2_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_2_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_2_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_2_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_2_SEL_Pin,
                .fault_port = PDM_TPS4XXXX_2_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_2_FAULT_Pin
            }
        }
    },

    /*------------------------------------------------------------------------*/
    /* Channel 20: TPS4H160 device 2, IC channel 3 (IN4) - Regulated 12V      */
    /*------------------------------------------------------------------------*/
    [VBATT_REG_SW_4] = {
        .ic_type     = PDM_CHANNEL_TYPE_TPS4H160,
        .is_critical = false,
        .enabled     = true,
        .device_settings = {
            .tps4xxxx_config = {
                .r_cs       = TPS4H160_CS_RESISTOR_OHMS,
                .r_cl       = TPS4H160_CL_RESISTOR_OHMS  
            }
        },
        .hw = {
            .tps4xxxx = {
                .input_port   = PDM_TPS4XXXX_2_IN1_GPIO_Port,
                .input_pin    = PDM_TPS4XXXX_2_IN1_Pin,
                .diag_port = PDM_TPS4XXXX_2_DIAG_EN_GPIO_Port,
                .diag_pin  = PDM_TPS4XXXX_2_DIAG_EN_Pin,
                .seh_port  = PDM_TPS4XXXX_2_SEH_GPIO_Port,
                .seh_pin   = PDM_TPS4XXXX_2_SEH_Pin,
                .sel_port  = PDM_TPS4XXXX_2_SEL_GPIO_Port,
                .sel_pin   = PDM_TPS4XXXX_2_SEL_Pin, 
                .fault_port = PDM_TPS4XXXX_2_FAULT_GPIO_Port,
                .fault_pin = PDM_TPS4XXXX_2_FAULT_Pin
            }
        }
    },
};
