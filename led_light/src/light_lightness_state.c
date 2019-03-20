#include "light_lightness_state.h"

#include "bt_callbacks.h"

static float sqrt(float square)
{
    float root, last, diff;

    root = square / 3.0;
    diff = 1;

    if (square <= 0) {
        return 0;
    }

    do {
        last = root;
        root = (root + square / root) / 2.0;
        diff = root - last;
    } while (diff > MINDIFF || diff < -MINDIFF);

    return root;
}

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

    tmp = ((float) actual / 65535);

    return (u16_t) ceiling(65535 * tmp * tmp);
}

static u16_t linear_to_actual(u16_t linear)
{
    return (u16_t)(65535 * sqrt(((float) linear / 65535)));
}


int set_attribute(u16_t x, u16_t y, u16_t message_type)
{
    int err = 0;
    ;
    switch (message_type) {
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK:
        if ((x >= light_lightness_state_data.range_min && x <= light_lightness_state_data.range_max)
            || x == LIGHT_OFF) {
            if (light_lightness_state_data.actual) {
                light_lightness_state_data.last = light_lightness_state_data.actual;
            }
            light_lightness_state_data.actual = x;
            light_lightness_state_data.linear = actual_to_linear(x);
        } else {
            err = -EINVAL;
        }
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET_UNACK:
        if ((x >= light_lightness_state_data.range_min && x <= light_lightness_state_data.range_max)
            || x == LIGHT_OFF) {
            if (light_lightness_state_data.actual) {
                light_lightness_state_data.last = light_lightness_state_data.actual;
            }
            light_lightness_state_data.linear = x;
            light_lightness_state_data.actual = linear_to_actual(x);
        } else {
            err = -EINVAL;
        }
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK:
        if (x != 0 && y != 0) {
            if (x <= y) {
                if (light_lightness_state_data.range_min != x
                    || light_lightness_state_data.range_max != y) {
                    light_lightness_state_data.range_min = x;
                    light_lightness_state_data.range_max = y;
                }
            } else {
                err = -EINVAL;
            }
        } else {
            err = -EINVAL;
        }
    default:
        break;
    };
    return 0;
}

struct light_lightness_state light_lightness_state_data = {
    .set_attribute = set_attribute,
    .actual        = 0x000B,
    .linear        = 0,
    .last          = 0x00AA,
    .range_max     = U16_MAX / 2,
    .range_min     = 0x0001,
    .def           = 0x0000,
    .status_code   = 0x00,
};

