#include "task_timing_svc.h"
#include "task_timing_data.h"

// Helper functions
void task_timing_update_average(float new_sample, task_timing_index_t index)
{
    // Implementation for updating average
    task_timing timing_data = task_timing_data_get_task(index);

    if (timing_data.count < WINDOW_SIZE) {
        timing_data.buffer[timing_data.head] = new_sample;
        timing_data.running_sum += new_sample;
        timing_data.head = (timing_data.head + 1) % WINDOW_SIZE;
        timing_data.count++;
    } else {
        // Remove the oldest sample from the running sum
        timing_data.running_sum -= timing_data.buffer[timing_data.head];
        // Add the new sample to the buffer and running sum
        timing_data.buffer[timing_data.head] = new_sample;
        timing_data.running_sum += new_sample;
        timing_data.head = (timing_data.head + 1) % WINDOW_SIZE;
    }

    timing_data.average = (float)timing_data.running_sum / (float)timing_data.count;

    if (new_sample > timing_data.max) {
        timing_data.max = new_sample;
    }

    if (new_sample < timing_data.min) {
        timing_data.min = new_sample;
    }

    task_timing_data_set_task(index, timing_data);
    
}



