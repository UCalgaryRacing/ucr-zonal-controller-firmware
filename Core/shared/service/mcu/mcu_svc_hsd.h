#include "com_typ_common.h"

typedef enum
{
    /* VNF9Q20F Device 0 - Channels 1-4 */
    DOUT_1 = 0,
    DOUT_2,
    DOUT_3,
    DOUT_4
} mcu_channel_id_t;


status_t mcu_svc_hsd_channel_enable(mcu_channel_id_t channel_id, bool enable);