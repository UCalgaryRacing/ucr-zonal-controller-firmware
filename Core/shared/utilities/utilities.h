#ifndef CORE_UTILITIES_H
#define CORE_UTILITIES_H

#include <stdint.h>
#include <stdbool.h>

static inline bool check_float_limits(float value, float upper, float lower)
{
    if (value > upper || value < lower)
    {
        return false;
    }

    return true;
}

static inline float clamp_float(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

static inline int16_t clamp_int16(int16_t value, int16_t min, int16_t max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

#endif /* CORE_UTILITIES_H */
