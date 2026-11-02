#include <string.h>
#include "task_timing_data.h"



/*============================================================================*/
/* Private Data Storage                                                       */
/*============================================================================*/

static task_timing_data_t g_task_timing_data;
static bool g_initialized;

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

status_t task_timing_data_init(void)
{
    memset(&g_task_timing_data, 0, sizeof(g_task_timing_data));

    g_initialized = true;

    return OK;
}
/*============================================================================*/
/* Generic Data Access                                                    */
/*============================================================================*/

task_timing task_timing_data_get_task(task_timing_index_t index)
{

    /*
    if (index < 0 || index >= 5)
    {
        // Handle invalid index case, maybe return a default value or error
        task_timing default_timing = {0};
        return default_timing;
    }
    */
    return g_task_timing_data.tasks[index];
}

void task_timing_data_set_task(task_timing_index_t index, task_timing timing)
{
    /*
    if (index < 0 || index >= 5)
    {
        // Handle invalid index case, maybe return or log an error
        return;
    }
    */
    g_task_timing_data.tasks[index] = timing;
}

/*============================================================================*/
/* Fast Task Data Access                                                    */
/*============================================================================*/

void task_timing_data_set_fast_average(float average)
{
    g_task_timing_data.task_fast.average = average;
}

void task_timing_data_set_fast_max(float max)
{
    g_task_timing_data.task_fast.max = max;
}

void task_timing_data_set_fast_min(float min)
{
    g_task_timing_data.task_fast.min = min;
}

float task_timing_data_get_fast_average(void)
{
    return g_task_timing_data.task_fast.average;
}

float task_timing_data_get_fast_max(void)
{
    return g_task_timing_data.task_fast.max;
}

float task_timing_data_get_fast_min(void)
{
    return g_task_timing_data.task_fast.min;
}

/*============================================================================*/
/* Very Fast Task Data Access                                                    */
/*============================================================================*/

void task_timing_data_set_very_fast_average(float average)
{
    g_task_timing_data.task_very_fast.average = average;
}

void task_timing_data_set_very_fast_max(float max)
{
    g_task_timing_data.task_very_fast.max = max;
}

void task_timing_data_set_very_fast_min(float min)
{
    g_task_timing_data.task_very_fast.min = min;
}

float task_timing_data_get_very_fast_average(void)
{
    return g_task_timing_data.task_very_fast.average;
}

float task_timing_data_get_very_fast_max(void)
{
    return g_task_timing_data.task_very_fast.max;
}

float task_timing_data_get_very_fast_min(void)
{
    return g_task_timing_data.task_very_fast.min;
}

/*============================================================================*/
/* Task Manager Data Access                                                    */
/*============================================================================*/

void task_timing_data_set_manager_average(float average)
{
    g_task_timing_data.task_manager.average = average;
}

void task_timing_data_set_manager_max(float max)
{
    g_task_timing_data.task_manager.max = max;
}

void task_timing_data_set_manager_min(float min)
{
    g_task_timing_data.task_manager.min = min;
}

float task_timing_data_get_manager_average(void)
{
    return g_task_timing_data.task_manager.average;
}

float task_timing_data_get_manager_max(void)
{
    return g_task_timing_data.task_manager.max;
}

float task_timing_data_get_manager_min(void)
{
    return g_task_timing_data.task_manager.min;
}
/*============================================================================*/
/* Slow Task Manager Data Access                                                    */
/*============================================================================*/

void task_timing_data_set_slow_average(float average)
{
    g_task_timing_data.task_slow.average = average;
}

void task_timing_data_set_slow_max(float max)
{
    g_task_timing_data.task_slow.max = max;
}

void task_timing_data_set_slow_min(float min)
{
    g_task_timing_data.task_slow.min = min;
}

float task_timing_data_get_slow_average(void)
{
    return g_task_timing_data.task_slow.average;
}

float task_timing_data_get_slow_max(void)
{
    return g_task_timing_data.task_slow.max;
}

float task_timing_data_get_slow_min(void)
{
    return g_task_timing_data.task_slow.min;
}
/*============================================================================*/
/* TCU State Manager Data Access                                                    */
/*============================================================================*/

void task_timing_data_set_tcu_state_manager_average(float average)
{
    g_task_timing_data.tcu_state_manager.average = average;
}

void task_timing_data_set_tcu_state_manager_max(float max)
{
    g_task_timing_data.tcu_state_manager.max = max;
}

void task_timing_data_set_tcu_state_manager_min(float min)
{
    g_task_timing_data.tcu_state_manager.min = min;
}

float task_timing_data_get_tcu_state_manager_average(void)
{
    return g_task_timing_data.tcu_state_manager.average;
}

float task_timing_data_get_tcu_state_manager_max(void)
{
    return g_task_timing_data.tcu_state_manager.max;
}

float task_timing_data_get_tcu_state_manager_min(void)
{
    return g_task_timing_data.tcu_state_manager.min;
}