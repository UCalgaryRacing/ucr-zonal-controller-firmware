#ifndef DRIVER_GPS_TYP_COMMON_H_
#define DRIVER_GPS_TYP_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

/*============================================================================*/
/* Status / Error Codes                                                       */
/*============================================================================*/

typedef enum
{
    GPS_OK = 0,
    GPS_ERROR,
    GPS_ERROR_INVALID_PARAM,
    GPS_ERROR_NOT_INITIALIZED,
    GPS_ERROR_BUSY,
    GPS_ERROR_TIMEOUT,
    GPS_ERROR_COMMUNICATION,
    GPS_ERROR_OVERFLOW,
    GPS_ERROR_BAD_CRC,
    GPS_ERROR_NO_DATA,
} gps_status_t;

#endif /* DRIVER_GPS_TYP_COMMON_H_ */
