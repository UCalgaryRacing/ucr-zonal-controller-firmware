#ifndef MCU_ANALOG_CHANNELS_H_
#define MCU_ANALOG_CHANNELS_H_

/* MCU analog input indices*/

typedef enum
{
    FCO_ADC3_INP1 = 0,
    FCO_ADC3_INP0,
    FCO_ADC2_INP0,

    RCO_ADC3_INP1,
    RCO_ADC3_INP0,
    
    MCU_TOTAL_ADC_INPUT_COUNT
} mcu_analog_channel_id_t;


/*============================================================================*/
/*  DMA Array for ADCs                                                        */
/*============================================================================*/

//TODO: probably switch to adc polling / interrupt instead of dma to use across multiple files?
//without having to extern the dma buffer from this file, but maybe not if this is only analog on MCU board....

/* DMA ARRAY:                                                                */
/* IMPORTANT: Make sure RANK# in CubeMX matches index# here                  */
/* index# = rank# - 1                                                        */
/* num# = adc# - 1     														 */

/*----------------------------   ADC3_INP1 ---------------------------- */

#define ADC_3_1_BUFFER_INDEX        0
#define ADC_3_1_BUFFER_NUMBER       2 

/*----------------------------   ADC3_INP0 ----------------------------*/

#define ADC_3_0_BUFFER_INDEX        1
#define ADC_3_0_BUFFER_NUMBER       2 

/*----------------------------   ADC2_INP0 ----------------------------*/

#define ADC_2_0_BUFFER_INDEX        0
#define ADC_2_0_BUFFER_NUMBER       1 

#endif /* MCU_ANALOG_CHANNELS_H_ */
