#ifndef _NODE_COMPOSITION_H
#define _NODE_COMPOSITION_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <logging/log.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "bt_callbacks.h"

struct lightness_cli {
    struct bt_mesh_model *m_model_cli;
    u8_t received_msg;
    u16_t m_linear;
    u16_t m_last;
    u16_t m_default;
    u16_t m_min_range;
    u16_t m_max_range;
    u8_t m_tid;
};

extern struct lightness_cli light_lightness_cli[];
extern struct bt_mesh_model change_model[];
extern const struct bt_mesh_comp comp;

#endif  // _NODE_COMPOSITION_H
