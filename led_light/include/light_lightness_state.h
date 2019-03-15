/**
 * @file light_lightness_state.h
 * @Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-03-13
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef _LIGHT_LIGHTNESS_STATE_H
#define _LIGHT_LIGHTNESS_STATE_H

#include <zephyr/types.h>

struct light_lightness_state {
    u16_t linear;
    u16_t target_linear;

    u16_t actual;
    u16_t target_actual;

    u16_t last;
    u16_t def;

    u8_t status_code;
    u16_t range_min;
    u16_t range_max;
    u32_t lightness_range;

    u8_t last_tid;
    u16_t last_src_addr;
    u16_t last_dst_addr;
    s64_t last_msg_timestamp;
};

extern struct light_lightness_state light_lightness_state_data;

#endif