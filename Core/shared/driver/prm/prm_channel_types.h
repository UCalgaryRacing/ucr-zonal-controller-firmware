#ifndef PRM_CHANNEL_TYPES_H_
#define PRM_CHANNEL_TYPES_H_

typedef enum
{
    INTERNAL_3V3_2 = 0,
    INTERNAL_5V_2,
    INTERNAL_12V,
    
    EXTERNAL_5V_1,
    EXTERNAL_5V_2,
    EXTERNAL_12V_1,
    EXTERNAL_12V_2,

    PRM_CHANNEL_COUNT
} prm_channel_id_t;

#endif /* PRM_CHANNEL_TYPES_H_ */