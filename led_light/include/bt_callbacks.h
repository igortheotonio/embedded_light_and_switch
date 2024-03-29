#ifndef _BT_CALLBACKS_H_
#define _BT_CALLBACKS_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>


// LINEAR MESSAGES
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS BT_MESH_MODEL_OP_2(0x82, 0x52)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET BT_MESH_MODEL_OP_2(0x82, 0x4F)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET BT_MESH_MODEL_OP_2(0x82, 0x50)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x51)

// ACTUAL MESSAGES
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_GET BT_MESH_MODEL_OP_2(0x82, 0x4B)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET BT_MESH_MODEL_OP_2(0x82, 0x4C)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x4D)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_STATUS BT_MESH_MODEL_OP_2(0x82, 0x4E)

// RANGE MESSAGES
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS BT_MESH_MODEL_OP_2(0x82, 0x58)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET BT_MESH_MODEL_OP_2(0x82, 0x5B)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x5C)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET BT_MESH_MODEL_OP_2(0x82, 0x57)

// DEFAULT MESSAGES
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET BT_MESH_MODEL_OP_2(0x82, 0x59)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x5A)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS BT_MESH_MODEL_OP_2(0x82, 0x56)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET BT_MESH_MODEL_OP_2(0x82, 0x55)

// LAST MESSAGES
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_STATUS BT_MESH_MODEL_OP_2(0x82, 0x54)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET BT_MESH_MODEL_OP_2(0x82, 0x53)

extern const struct bt_mesh_model_op light_lightness_srv_op[];
extern const struct bt_mesh_model_op light_lightness_setup_srv_op[];

void light_lightness_default_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf);
void light_lightness_default_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf);
void light_lightness_default_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                       struct net_buf_simple *buf);

void light_lightness_range_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                               struct net_buf_simple *buf);
void light_lightness_range_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                               struct net_buf_simple *buf);
void light_lightness_range_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                     struct net_buf_simple *buf);


void light_lightness_actual_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf);

void light_lightness_actual_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                      struct net_buf_simple *buf);

void light_lightness_actual_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf);


void light_lightness_linear_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf);

void light_lightness_linear_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                      struct net_buf_simple *buf);

void light_lightness_linear_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf);


void light_lightness_last_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                              struct net_buf_simple *buf);


#endif
