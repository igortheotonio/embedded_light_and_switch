#include "bt_callbacks.h"

#include <logging/log.h>

#include "light_lightness_state.h"
#include "pwm_driver.h"


LOG_MODULE_REGISTER(BT_CALLBACKS, 4);

// Operations
const struct bt_mesh_model_op light_lightness_srv_op[] = {
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_GET, 0, light_lightness_actual_get},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET, 3, light_lightness_actual_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK, 3, light_lightness_actual_set_unack},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET, 0, light_lightness_linear_get},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET, 3, light_lightness_linear_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET_UNACK, 3, light_lightness_linear_set_unack},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET, 0, light_lightness_default_get},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET, 0, light_lightness_range_get},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET, 0, light_lightness_last_get},
    BT_MESH_MODEL_OP_END,
};

const struct bt_mesh_model_op light_lightness_setup_srv_op[] = {
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET, 2, light_lightness_default_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK, 2, light_lightness_default_set_unack},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET, 5, light_lightness_range_set},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK, 5, light_lightness_range_set_unack},
    BT_MESH_MODEL_OP_END,
};

u16_t actual_to_pulse_width(u16_t actual)
{
    return (u16_t)((actual - U16_MIN) * (PERIOD - 0) / (U16_MAX - U16_MIN) + 0);
}

void light_lightness_linear_broadcast(struct bt_mesh_model *model)
{
    int err;
    struct net_buf_simple *msg          = model->pub->msg;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS);
    net_buf_simple_add_le16(msg, state->linear);

    err = bt_mesh_model_publish(model);
    if (err) {
        LOG_ERR("bt_mesh_model_publish err %d, sending msg to 0x%04x\n", err, model->pub->addr);
    } else {
        LOG_INF("[0x%04x]: Broadcasting linear state: 0x%04x", bt_mesh_model_elem(model)->addr,
                state->linear);
    }
}


void light_lightness_linear_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    LOG_INF("[LINEAR_GET - 0x%04x]: Received a get msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct light_lightness_state *state = model->user_data;
    struct net_buf_simple *msg          = NET_BUF_SIMPLE(2 + 5 + 4);

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS);
    net_buf_simple_add_le16(msg, state->linear);

    if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
        printk("Unable to send LightLightnessAct Status response\n");
    } else {
        LOG_INF("[LINEAR_GET - 0x%04x]: status msg value 0x%04x will be send to 0x%04x",
                bt_mesh_model_elem(model)->addr, state->linear, ctx->addr);
    }
}

void light_lightness_linear_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                      struct net_buf_simple *buf)
{
    LOG_INF("[LINEAR_SET - 0x%04x]: Received a set msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);

    struct light_lightness_state *state = model->user_data;
    u16_t lightness                     = net_buf_simple_pull_le16(buf);
    u8_t tid                            = net_buf_simple_pull_u8(buf);


    s64_t now = k_uptime_get();
    if (state->last_tid == tid && state->last_src_addr == ctx->addr
        && state->last_dst_addr == ctx->recv_dst
        && (now - state->last_msg_timestamp <= K_SECONDS(6))) {
        return;
    }

    blink_light(&pwm, 3);

    state->last_tid           = tid;
    state->last_src_addr      = ctx->addr;
    state->last_dst_addr      = ctx->recv_dst;
    state->last_msg_timestamp = now;

    LOG_INF("[LINEAR_SET - 0x%04x]: Value msg: 0x%04x\n", bt_mesh_model_elem(model)->addr,
            lightness);

    if (state->set_attribute(lightness, U16_NULL, BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET_UNACK)) {
        LOG_INF("Invalid lightness");
    } else {
        change_pulse_width(&pwm, actual_to_pulse_width(state->actual));
        set_pulse_width(&pwm);
    }
}

void light_lightness_linear_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    light_lightness_linear_set_unack(model, ctx, buf);
    light_lightness_linear_get(model, ctx, buf);
    light_lightness_linear_broadcast(model);
}


void light_lightness_actual_broadcast(struct bt_mesh_model *model)
{
    int err;
    struct net_buf_simple *msg          = model->pub->msg;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_STATUS);
    net_buf_simple_add_le16(msg, state->actual);

    err = bt_mesh_model_publish(model);
    if (err) {
        LOG_ERR("bt_mesh_model_publish err %d, sending msg to 0x%04x\n", err, model->pub->addr);
    } else {
        LOG_INF("[ACTUAL_SET - 0x%04x]: Broadcasting actual state: 0x%04x",
                bt_mesh_model_elem(model)->addr, state->actual);
    }
}

void light_lightness_actual_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    LOG_INF("[ACTUAL_GET - 0x%04x]: Received a get msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct net_buf_simple *msg          = NET_BUF_SIMPLE(2 + 5 + 4);
    struct light_lightness_state *state = model->user_data;

    printk("0x%04x \n", model->pub->addr);

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_STATUS);
    net_buf_simple_add_le16(msg, state->actual);

    if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
        printk("Unable to send LightLightnessAct Status response\n");
    } else {
        LOG_INF("[ACTUAL_GET - 0x%04x]: status msg value 0x%04x will be send to 0x%04x",
                bt_mesh_model_elem(model)->addr, state->actual, ctx->addr);
    }
}

void light_lightness_actual_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                      struct net_buf_simple *buf)
{
    LOG_INF("[ACTUAL_SET - 0x%04x]: Received a set msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);

    struct light_lightness_state *state = model->user_data;
    s64_t now;
    u8_t tid;
    u16_t lightness = net_buf_simple_pull_le16(buf);
    tid             = net_buf_simple_pull_u8(buf);

    now = k_uptime_get();
    if (state->last_tid == tid && state->last_src_addr == ctx->addr
        && state->last_dst_addr == ctx->recv_dst
        && (now - state->last_msg_timestamp <= K_SECONDS(6))) {
        return;
    }

    state->last_tid           = tid;
    state->last_src_addr      = ctx->addr;
    state->last_dst_addr      = ctx->recv_dst;
    state->last_msg_timestamp = now;
    LOG_INF("[ACTUAL_SET - 0x%04x]: Value msg: 0x%04x\n", bt_mesh_model_elem(model)->addr,
            lightness);

    if (state->set_attribute(lightness, U16_NULL, BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET_UNACK)) {
        LOG_INF("Invalid lightness");
    } else {
        change_pulse_width(&pwm, actual_to_pulse_width(state->actual));
        set_pulse_width(&pwm);
    }
}

void light_lightness_actual_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                struct net_buf_simple *buf)
{
    light_lightness_actual_set_unack(model, ctx, buf);
    light_lightness_actual_get(model, ctx, buf);
    light_lightness_actual_broadcast(model);
}


void light_lightness_default_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    LOG_INF("[DEFAULT_GET - 0x%04x]: Received a get msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct bt_mesh_model_pub *pub;
    pub                                 = model->pub;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(pub->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS);
    net_buf_simple_add_le16(pub->msg, state->def);

    if (bt_mesh_model_send(model, ctx, pub->msg, NULL, NULL)) {
        printk("Unable to send LightLightnessDef Status response\n");
    } else {
        LOG_INF("[DEFAULT_GET - 0x%04x]: status msg value 0x%04x will be send to 0x%04x",
                bt_mesh_model_elem(model)->addr, state->def, ctx->addr);
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
    } else {
        LOG_INF("[DEFAULT_SET - 0x%04x]: Broadcasting...", bt_mesh_model_elem(model)->addr);
    }
}

void light_lightness_default_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                       struct net_buf_simple *buf)
{
    LOG_INF("[DEFAULT_SET - 0x%04x]: Received a set msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    u16_t lightness;
    struct light_lightness_state *state = model->user_data;

    lightness = net_buf_simple_pull_le16(buf);

    state->def = lightness;
}


void light_lightness_range_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                               struct net_buf_simple *buf)
{
    LOG_INF("[RANGE_GET - 0x%04x]: Received a get msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct bt_mesh_model_pub *pub;
    pub                                 = model->pub;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(pub->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS);
    net_buf_simple_add_u8(pub->msg, state->status_code);
    net_buf_simple_add_le16(pub->msg, state->range_min);
    net_buf_simple_add_le16(pub->msg, state->range_max);

    if (bt_mesh_model_send(model, ctx, pub->msg, NULL, NULL)) {
        printk("Unable to send LightLightnessRange Status response\n");
    } else {
        LOG_INF("[RANGE_GET - 0x%04x]: Value msg: 0x%04x/0x%04x will be send to 0x%04x",
                bt_mesh_model_elem(model)->addr, state->range_min, state->range_max, ctx->addr);
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
    } else {
        LOG_INF("[RANGE_SET - 0x%04x]: Broadcasting...", bt_mesh_model_elem(model)->addr);
    }
}

void light_lightness_range_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                     struct net_buf_simple *buf)
{
    u16_t min, max;
    struct light_lightness_state *state = model->user_data;

    min                = net_buf_simple_pull_le16(buf);
    max                = net_buf_simple_pull_le16(buf);
    state->status_code = 0x00;
    if (state->set_attribute(min, max, BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK)) {
        state->status_code = 0x01;
    }
}


void light_lightness_last_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                              struct net_buf_simple *buf)
{
    LOG_INF("[LAST_GET - 0x%04x]: Received a get msg from group address 0x%04x, sent by 0x%04x",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct bt_mesh_model_pub *pub;
    pub                                 = model->pub;
    struct light_lightness_state *state = model->user_data;

    bt_mesh_model_msg_init(pub->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_STATUS);
    net_buf_simple_add_le16(pub->msg, state->last);

    if (bt_mesh_model_send(model, ctx, pub->msg, NULL, NULL)) {
        printk("Unable to send LightLightnessAct Status response\n");
    } else {
        LOG_INF("[LAST_GET - 0x%04x]: status msg value 0x%04x will be send to 0x%04x",
                bt_mesh_model_elem(model)->addr, state->last, ctx->addr);
    }
}
