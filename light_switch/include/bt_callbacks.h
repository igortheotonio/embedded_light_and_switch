#ifndef _BT_CALLBACKS_H
#define _BT_CALLBACKS_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <logging/log.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "defines.h"
#include "leds.h"

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

#endif  // _BT_CALLBACKS_H