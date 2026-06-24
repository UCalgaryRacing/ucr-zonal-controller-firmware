#ifndef MCU_ANALOG_CHANNELS_H_
#define MCU_ANALOG_CHANNELS_H_

/* MCU analog input indices: shared by front and rear images (table + APIs). */

typedef enum
{
    MCU_AIN1 = 0,
    MCU_AIN2,
    MCU_AIN3,
    MCU_AIN4,
    MCU_AIN5,
    MCU_AIN6,
    MCU_AIN7,
    MCU_AIN8,
    MCU_ANALOG_SENSOR_COUNT
} mcu_analog_channel_id_t;

#endif /* MCU_ANALOG_CHANNELS_H_ */
