#include "bt_callbacks.h"

#include <logging/log.h>


LOG_MODULE_REGISTER(BT_CALLBACKS, 4);

// Operations
const struct bt_mesh_model_op light_lightness_srv_op[] = {
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_GET, 0, light_lightness_actual_get},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET, 3, light_lightness_actual_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK, 3, light_lightness_actual_set_unack},
    BT_MESH_MODEL_OP_END,
};

const struct bt_mesh_model_op light_lightness_setup_srv_op[] = {
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET, 2, light_lightness_default_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK, 2, light_lightness_default_set_unack},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET, 4, light_lightness_range_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK, 4, light_lightness_range_set_unack},
    BT_MESH_MODEL_OP_END,
};

void light_lightness_actual_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    struct net_buf_simple *msg          = NET_BUF_SIMPLE(2 + 5 + 4);
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_STATUS);
    net_buf_simple_add_le16(msg, state->actual);

    if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
        printk("Unable to send LightLightnessAct Status response\n");
    }
}

void light_lightness_actual_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                      struct net_buf_simple *buf)
{
    LOG_INF("chegou um set unack");
}

void light_lightness_actual_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    light_lightness_actual_set_unack(model, ctx, buf);
    light_lightness_actual_get(model, ctx, buf);
}


void light_lightness_default_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    struct bt_mesh_model_pub *pub;
    pub                                 = model->pub;
    struct light_lightness_state *state = model->user_data;

    u16_t value = 0;

    if (state->def == 0) {
        value = state->last;
    } else if (state->def == U16_MAX) {
        value = state->range_max;
    } else {
        value = state->actual;
    }

    bt_mesh_model_msg_init(pub->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS);
    net_buf_simple_add_le16(pub->msg, value);

    if (bt_mesh_model_send(model, ctx, pub->msg, NULL, NULL)) {
        printk("Unable to send LightLightnessDef Status response\n");
    }
}

void light_lightness_default_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    light_lightness_default_set_unack(model, ctx, buf);
    light_lightness_default_get(model, ctx, buf);
    int err = bt_mesh_model_publish(model);
    if (err) {
        printk("bt_mesh_model_publish err %d, sending msg to 0x%04x\n", err, model->pub->addr);
    }
}

void light_lightness_default_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                       struct net_buf_simple *buf)
{
    u16_t lightness;
    struct light_lightness_state *state = model->user_data;

    lightness = net_buf_simple_pull_le16(buf);

    state->def = lightness;
}


void light_lightness_range_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                               struct net_buf_simple *buf)
{
    struct bt_mesh_model_pub *pub;
    pub                                 = model->pub;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(pub->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS);
    net_buf_simple_add_le16(pub->msg, state->range_min);
    net_buf_simple_add_le16(pub->msg, state->range_max);

    if (bt_mesh_model_send(model, ctx, pub->msg, NULL, NULL)) {
        printk("Unable to send LightLightnessRange Status response\n");
    }
}

void light_lightness_range_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                               struct net_buf_simple *buf)
{
    light_lightness_range_set_unack(model, ctx, buf);
    light_lightness_range_get(model, ctx, buf);
    int err = bt_mesh_model_publish(model);
    if (err) {
        printk("bt_mesh_model_publish err %d, sending msg to 0x%04x\n", err, model->pub->addr);
    }
}

void light_lightness_range_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                     struct net_buf_simple *buf)
{
    u16_t min, max;
    struct light_lightness_state *state = model->user_data;

    min = net_buf_simple_pull_le16(buf);
    max = net_buf_simple_pull_le16(buf);

    if (min != 0 && max != 0) {
        if (min <= max) {
            if (state->range_min != min || state->range_max != max) {
                state->range_min = min;
                state->range_max = max;
                // TODO: IMPLEMENTAR FUNÇAO PARA LIGAR/DESLIGAR LEDS
            }
        }
    }
}
