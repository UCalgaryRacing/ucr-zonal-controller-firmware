#include "gps_config.h"
#include "gps_config_pinout.h"

/*============================================================================*/
/* Hardware Configuration Table                                               */
/*============================================================================*/

const gps_hw_t gps_hw_config =
{
    .uart_handle = GPS_UART_HANDLE,
    .dma_handle  = GPS_DMA_HANDLE,
};

/*============================================================================*/
/* Log/receiver Configuration Tables                                          */
/*============================================================================*/
const char * const gps_oem7_setup_commands[] = {
    "UNLOGALL\r\n",

    /* Track TerraStar's L-band beam for GNSS corrections (and to receive
     * over-the-air subscription activation if it hasn't auto-activated).
     * AUTO selects the best beam for the current location. */
    "ASSIGNLBANDBEAM AUTO\r\n",

    "CONNECTIMU SPI EPSON_G320\r\n",
    /* Antenna offset from the IMU, expressed in the IMU body frame as
     * labelled on the enclosure (IMU +X = vehicle left, IMU +Y = vehicle
     * rear, IMU +Z = vehicle up).  Antenna sits 91 cm rear, 12 cm left,
     * 86 cm above the IMU in the vehicle frame; in IMU-body axes that's
     * (0.12, +0.91, +0.86) m.  Stdevs are the OEM7 default of 10% of
     * each offset magnitude. */
    "SETINSTRANSLATION ANT1 0.12 0.91 0.86\r\n",

    /**
     * The definition of the vehicle frame is as follows:
     * z-axis – points up through the roof of the vehicle perpendicular to the ground
     * y-axis – points out the front of the vehicle in the direction of travel
     * x-axis – completes the right-handed system (out the right-hand side of the vehicle when facing forward)
     *
     * On the UCR03, the IMU is mounted so +X points to the left, +Y Points to the rear,
     * and +Z points up
     *
     * Novatel applies corrections in the order of Z, X, Y, and all rotations are right handed.
     */
    "SETINSROTATION RBV -2.7 0 180\r\n",
    NULL,
};

/* Add or remove entries to change which logs the OEM7600 streams at runtime.
 * The service sends UNLOGALL before iterating this array, so only logs listed
 * here will be active.  Terminate the array with { NULL, 0 }. */
const gps_oem7_log_config_t gps_oem7_log_configs[] =
{
    {
        .command = "LOG THISPORT BESTGNSSPOSB ONTIME 0.05\r\n",
        .msg_id  = GPS_OEM7_BESTGNSSPOS_ID,
    },
    {
        .command = "LOG THISPORT RAWIMUSXB ONNEW\r\n",
        .msg_id  = GPS_OEM7_RAWIMUSX_ID,
    },
    {
        .command = "LOG THISPORT INSPVASB ONTIME 0.05\r\n",
        .msg_id  = GPS_OEM7_INSPVAS_ID,
    },

    /* Sentinel — do not remove */
    { NULL, 0U },
};
