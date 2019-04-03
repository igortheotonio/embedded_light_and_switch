#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <zephyr.h>

#include "defines.h"

static s32_t ceiling(float num)
{
    s32_t inum;

    inum = (s32_t) num;
    if (num == (float) inum) {
        return inum;
    }

    return inum + 1;
}

static u16_t actual_to_linear(u16_t actual)
{
    float tmp;

    tmp = ((float) actual / MAX_BRIGHTNESS);

    return (u16_t) ceiling(MAX_BRIGHTNESS * tmp * tmp);
}

static int max(int a, int b)
{
    return (a > b) ? a : b;
}

static int min(int a, int b)
{
    return (a < b) ? a : b;
}

#endif  // _FUNCTIONS_H