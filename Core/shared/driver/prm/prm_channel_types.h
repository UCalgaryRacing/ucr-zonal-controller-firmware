#ifndef PRM_CHANNEL_TYPES_H_
#define PRM_CHANNEL_TYPES_H_

typedef enum
{
    INTERNAL_3V3_2 = 0,         // to GPS, IMU, Instrumentation
    INTERNAL_5V_2,              // to Instrumentation
    INTERNAL_12V,               // to MCU
    
    EXTERNAL_5V_1,              // to harness endpoint BSE
    EXTERNAL_5V_2,              // none written
    EXTERNAL_12V_1,             // to MCU, harness endpoint APPS
    EXTERNAL_12V_2,             // to Front Wheel Susp

    PRM_CHANNEL_COUNT
} prm_channel_id_t;

#endif /* PRM_CHANNEL_TYPES_H_ */