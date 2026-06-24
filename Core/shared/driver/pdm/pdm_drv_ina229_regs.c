#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "pdm_drv_ina229_regs.h"
#include "pdm_config.h"
#include "pdm_config_pinout.h"



#define MSB(u16) (((u16) & 0xFF00U) >> 8)   // most significant byte.
#define LSB(u16) ((u16) & 0xFFU)            // least significant byte.
#define maxRegAddress 0x3F

const uint8_t INA229_regSize[maxRegAddress + 1] = {
    2, 2, 2, 2, 3, 3, 2, 3,\
    3, 5, 5, 2, 2, 2, 2, 2,\
    2, 2, 0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0, 2, 2
};



static void MX_SPI5_Init_revert_CLKphase(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;  // this has to be changed to accomodate for the INA229s clock phase requirement.
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 0x0;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

void SPI_configure_default(void){
    MX_SPI5_Init_revert_CLKphase();
}



static void MX_SPI5_Init_change_CLKphase(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;  // this has to be changed to accomodate for the INA229s clock phase requirement.
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 0x0;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}


void SPI_configure_for_INA229(void){
    MX_SPI5_Init_change_CLKphase();
}

HAL_StatusTypeDef SPI_transmit_receive_INA229(uint8_t* txBuf, uint8_t* rxBuf, uint16_t size){
    SPI_configure_for_INA229();
    HAL_GPIO_WritePin(PDM_INA229_GLV_CS_GPIO_Port, PDM_INA229_GLV_CS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef s = HAL_SPI_TransmitReceive(PDM_SPI_HANDLE, txBuf,rxBuf,size,HAL_MAX_DELAY);
    HAL_GPIO_WritePin(PDM_INA229_GLV_CS_GPIO_Port, PDM_INA229_GLV_CS_Pin, GPIO_PIN_SET);
    SPI_configure_default();
    //HAL_Delay(1); // small delay to prevent sending out too many SPI messages at once, had issues with configuring in the past so this might be necessary.
    return s;
}



void INA229_writeReg(INA229_Shadow_regs_t* Ptr_INA229, uint8_t regAddr, uint16_t value) {
    uint8_t txBuf[3] = {0}; //All writable registers are 2 bytes, 1st byte needed for address.
    uint8_t rxBuf[3] = {0};

    txBuf[0] = regAddr << 2; //Address + write bit (ending in 0)
    txBuf[1] = MSB(value);
    txBuf[2] = LSB(value);

    SPI_transmit_receive_INA229(txBuf, rxBuf, 3);

    //check for change in ADCRANGE
    if (regAddr == INA229_config_register) {
        Ptr_INA229->adcrange = value & INA229_config_register_adcrange_4096mV;
        // this line ensures the addrange value in the struct is correct incase it was changed through this function
    }
}

uint64_t INA229_readReg(INA229_Shadow_regs_t* Ptr_INA229, uint8_t regAddr) {
    uint64_t value;
    int i;

    uint8_t txBuf[6] = {0};
    uint8_t rxBuf[6] = {0}; //max buffer size

    txBuf[0] = (regAddr << 2) | 0x01; //Address + read bit (ending in 1)
   
    SPI_transmit_receive_INA229(txBuf, rxBuf, 6);

    //Combine bytes
    value = 0; // initialize to 0, toss rxBuf[0];

    for (i = 1; i < INA229_regSize[regAddr] + 1; i++) {
        value = (value << 8) | rxBuf[i];
    }

    return value;
}

void INA229_Set_SHUNTCAL(INA229_Shadow_regs_t* Ptr_INA229) {

    const float datasheet_const = 13107.2e6;

    float SHUNTCAL = PDM_INA229_SHUNT_RESISTOR_OHMS * datasheet_const * PDM_INA229_CURRENT_LSB_A;
    // section 8.12 of the datasheet.

    // multiply by 4 if using the 4096 range. (ADC mode 1).

    if (Ptr_INA229->adcrange == INA229_config_register_adcrange_4096mV)
        SHUNTCAL *= 4;

    Ptr_INA229->shuntCalRegister = (uint16_t)SHUNTCAL;
}


void INA229_Set_SOVL(INA229_Shadow_regs_t* Ptr_INA229) {
    float Lsb = 5e-6;

    if (Ptr_INA229->adcrange == INA229_config_register_adcrange_4096mV)
        Lsb = 1.25e-6;
    

    uint16_t SOVL_REG_VAL = (uint16_t) round((PDM_GLV_OVERCURRENT_A * PDM_INA229_SHUNT_RESISTOR_OHMS) / Lsb);

    Ptr_INA229->sovlRegister = SOVL_REG_VAL;
}

void INA229_Set_SUVL(INA229_Shadow_regs_t* Ptr_INA229) {
    float Lsb = 5e-6;

    if (Ptr_INA229->adcrange == INA229_config_register_adcrange_4096mV)
        Lsb = 1.25e-6;

    uint16_t SUVL_REG_VAL = (uint16_t) round((PDM_GLV_UNDERCURRENT_A * PDM_INA229_SHUNT_RESISTOR_OHMS) / Lsb);
    Ptr_INA229->suvlRegister = SUVL_REG_VAL;
}

void INA229_Set_BOVL(INA229_Shadow_regs_t* Ptr_INA229){
    float Lsb = 3.125e-3;
    uint16_t BOVL_REG_VAL = (uint16_t) round(PDM_GLV_OVERVOLTAGE_V / Lsb);
    Ptr_INA229->bovlRegister = BOVL_REG_VAL;
}

void INA229_Set_BUVL(INA229_Shadow_regs_t* Ptr_INA229){
    float Lsb = 3.125e-3;
    uint16_t BUVL_REG_VAL = (uint16_t) round(PDM_GLV_UNDERVOLTAGE_V / Lsb);
    Ptr_INA229->buvlRegister = BUVL_REG_VAL;
}

float INA229_getCURRENT_signedLSB(INA229_Shadow_regs_t* Ptr_INA229) {
    uint64_t value = INA229_readReg(Ptr_INA229, INA229_current_register);
    float data;

    //Remove reserved bits
    value = value >> 4;

    //Convert for 2's compliment and signed value
    if (value > 0x7FFFF) {
        data = (float) value - 0x100000;
    } else {
        data = (float) value;
    }

    return data;
}


