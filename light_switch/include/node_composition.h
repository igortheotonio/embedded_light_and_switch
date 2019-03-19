#ifndef _NODE_COMPOSITION_H
#define _NODE_COMPOSITION_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "leds.h"

struct lightness_cli {
    struct bt_mesh_model *model_cli;
    // struct leds_device_t *leds_cli;
    u16_t def;
    u16_t act;
    u8_t tid;
};

extern struct lightness_cli light_lightness_cli[];
extern const struct bt_mesh_comp comp;

/* Light Lightness Client message handlers */
void light_lightness_actual_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf);

void light_lightness_linear_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf);

void light_lightness_last_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf);

void light_lightness_default_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                    struct net_buf_simple *buf);

void light_lightness_range_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf);

void send_light_lightness_actual_set(struct lightness_cli *bt_cli, u16_t message_type);

void send_light_lightness_actual_get(struct lightness_cli *bt_cli, u16_t message_type);

#endif  // _NODE_COMPOSITION_H