#ifndef TT_DATA_H_
#define TT_DATA_H_

#include <stdint.h>
#include <stdbool.h>
//#include "C:\Users\esall\OneDrive - University of Calgary\UCalgary Racing\UCR-03\Software\Controllers\UCR-03_Front_Controller_Dev\Core\shared\service\common\com_typ_common.h"
#include "com_typ_common.h"

#define WINDOW_SIZE 10

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t task_timing_data_init(void);

/*============================================================================*/
/* Task Timing Data Struct                                                            */
/*============================================================================*/

typedef struct
{
    float average;
    float max;
    float min;
    
    int32_t head;
    int32_t count;
    int32_t buffer[10]; // Example size, adjust as needed;
    int64_t running_sum;

} task_timing;

typedef union
{
    struct
    {
        task_timing task_very_fast;
        task_timing task_fast;
        task_timing task_manager;
        task_timing task_slow;
        task_timing tcu_state_manager;
    };
    task_timing tasks[5];

} task_timing_data_t;

typedef enum
{
    TASK_TIMING_VERY_FAST = 0,
    TASK_TIMING_FAST,
    TASK_TIMING_MANAGER,
    TASK_TIMING_SLOW,
    TASK_TIMING_TCU_STATE_MANAGER
} task_timing_index_t;

/*============================================================================*/
/* Setters                                                                    */
/*============================================================================*/

void task_timing_data_set_task(task_timing_index_t index, task_timing timing);

void task_timing_data_set_very_fast_average(float average);
void task_timing_data_set_fast_average(float average);
void task_timing_data_set_manager_average(float average);
void task_timing_data_set_slow_average(float average);

void task_timing_data_set_very_fast_max(float max);
void task_timing_data_set_fast_max(float max);
void task_timing_data_set_manager_max(float max);
void task_timing_data_set_slow_max(float max);

void task_timing_data_set_very_fast_min(float min);
void task_timing_data_set_fast_min(float min);
void task_timing_data_set_manager_min(float min);
void task_timing_data_set_slow_min(float min);


/*============================================================================*/
/* Getters                                                                    */
/*============================================================================*/

task_timing task_timing_data_get_task(task_timing_index_t index);

float task_timing_data_get_very_fast_average(void);
float task_timing_data_get_fast_average(void);
float task_timing_data_get_manager_average(void);
float task_timing_data_get_slow_average(void);

float task_timing_data_get_very_fast_max(void);
float task_timing_data_get_fast_max(void);
float task_timing_data_get_manager_max(void);
float task_timing_data_get_slow_max(void);

float task_timing_data_get_very_fast_min(void);
float task_timing_data_get_fast_min(void);
float task_timing_data_get_manager_min(void);
float task_timing_data_get_slow_min(void);

#endif /* RCO_DATA_H_ */
