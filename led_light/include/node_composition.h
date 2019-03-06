/**
 * @file node_composition.h
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#ifndef _NODE_COMPOSITION_H_
#define _NODE_COMPOSITION_H_


#include "mesh.h"
#include "node_composition.h"
#include "transition.h"

struct light_lightness_state {
    u16_t linear;
    u16_t target_linear;

    u16_t actual;
    u16_t target_actual;

    u16_t last;
    u16_t def;

    u8_t status_code;
    u16_t light_range_min;
    u16_t light_range_max;
    u32_t lightness_range;

    u8_t last_tid;
    u16_t last_src_addr;
    u16_t last_dst_addr;
    s64_t last_msg_timestamp;

    s32_t tt_delta_actual;
    s32_t tt_delta_linear;

    struct transition *transition;
};

extern struct light_lightness_state light_lightness_state_data;

extern struct bt_mesh_model root_models[];
extern struct bt_mesh_model node_models[];

extern const struct bt_mesh_comp comp;

#endif
