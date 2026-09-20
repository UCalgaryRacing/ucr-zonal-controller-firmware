#include "ins_svc_thermistors.h"
#include "ins_data.h"
#include "ins_svc_ads124s08.h"

// TODO: call this coolant temp instead of thermistors? 

//static float ins_svc_thermistors_voltage_to_temp(float voltage)
//{
//    return; //TODO: create equation or lookup table based on datasheet
//}


void ins_svc_thermistors_update(void)
{
    const ins_channel_id_t *therm_1_channel_id = ins_svc_get_channel_id(THERM_1);
    const ins_channel_id_t *therm_2_channel_id = ins_svc_get_channel_id(THERM_2);
    const ins_channel_id_t *therm_3_channel_id = ins_svc_get_channel_id(THERM_3);
    
    ins_thermistor_data_t thermistor_data;

    float therm_1_temp = 0;
    float therm_2_temp = 0;
    float therm_3_temp = 0;

    ins_svc_ads124s08_get_analog_voltage(*therm_1_channel_id, &therm_1_temp);
    ins_svc_ads124s08_get_analog_voltage(*therm_2_channel_id, &therm_2_temp);
    ins_svc_ads124s08_get_analog_voltage(*therm_3_channel_id, &therm_3_temp);

    // TODO: calculate temperature based on analog voltage using static function above

    thermistor_data.therm_1 = therm_1_temp;
    thermistor_data.therm_2 = therm_2_temp;
    thermistor_data.therm_3 = therm_3_temp;

    ins_data_set_therm_temp(THERM_1, thermistor_data.therm_1);
    ins_data_set_therm_temp(THERM_2, thermistor_data.therm_2);
    ins_data_set_therm_temp(THERM_3, thermistor_data.therm_3);

}
