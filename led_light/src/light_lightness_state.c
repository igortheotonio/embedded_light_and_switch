#include "light_lightness_state.h"

#include "bt_callbacks.h"

struct light_lightness_state light_lightness_state_data = {0};

int set_attribute(u16_t x, u16_t y, u16_t message_type)
{
    switch (message_type) {
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK:
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET:

        break;
    default:
        break;
    };
}
