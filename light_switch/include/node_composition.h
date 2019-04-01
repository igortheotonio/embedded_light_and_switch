#ifndef _NODE_COMPOSITION_H
#define _NODE_COMPOSITION_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "defines.h"
#include "leds.h"

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

/* Light Lightness Client message handlers */
void light_lightness_linear_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf);

void light_lightness_last_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf);

void light_lightness_default_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                    struct net_buf_simple *buf);

void light_lightness_range_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf);

void send_light_lightness_get(struct lightness_cli *bt_cli, u32_t message_type);

void send_light_lightness_linear_set(struct lightness_cli *bt_cli);

void get_all_data();

#endif  // _NODE_COMPOSITION_H
