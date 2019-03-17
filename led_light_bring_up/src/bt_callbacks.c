/**
 * @file ic_bt.c
 * @Lucas Peixoto (lpdac@ic.ufal.br)
 * @brief
 * @version 0.1
 * @date 2018-12-25
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "bt_callbacks.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(BT_CALLBACKS, 4);

const struct bt_mesh_model_op generic_onoff_srv_op[] = {
    {BT_MESH_MODEL_OP_GENERIC_ONOFF_GET, 0, generic_onoff_get},
    {BT_MESH_MODEL_OP_GENERIC_ONOFF_SET, 2, generic_onoff_set},
    {BT_MESH_MODEL_OP_GENERIC_ONOFF_SET_UNACK, 2, generic_onoff_set_unack},
    BT_MESH_MODEL_OP_END,
};

void generic_onoff_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                       struct net_buf_simple *buf)
{
    LOG_DBG("[0x%04x]: Received a get msg from group address 0x%04x, sended by 0x%04x.\n",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);

    struct net_buf_simple *msg = NET_BUF_SIMPLE(2 + 2 + 4);
    u8_t *state                = model->user_data;

    // 2 bytes for the opcode
    // 1 bytes parameters: present onoff value
    // 2 optional bytes for target onoff and remaining time
    // 4 additional bytes for the TransMIC


    LOG_DBG("Sending onoff status msg, value -> %d\n", *state);

    // Building msg

    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS);
    net_buf_simple_add_u8(msg, *state);

    if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
        LOG_ERR("Unable to send onoff status response\n");
    }
}


void generic_onoff_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                       struct net_buf_simple *buf)
{
    generic_onoff_set_unack(model, ctx, buf);
    generic_onoff_get(model, ctx, buf);
}


void generic_onoff_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                             struct net_buf_simple *buf)
{
    LOG_DBG("[0x%04x]: Received a set msg from group address 0x%04x, sended by 0x%04x.\n",
            bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);

    u8_t *state          = model->user_data;
    u8_t buflen          = buf->len;
    u8_t new_onoff_state = net_buf_simple_pull_u8(buf);
    u8_t tid             = net_buf_simple_pull_u8(buf);

    if (buflen > 2) {
        LOG_WRN("[wrn]: message contains transition_time field - processing not implemented\n");
    }

    if (buflen > 3) {
        LOG_WRN("[wrn]: message contains delay field - processing not implemented\n");
    }

    LOG_DBG("[0x%04x]: Value msg -> %d/ TID -> %d\n", bt_mesh_model_elem(model)->addr,
            new_onoff_state, tid);
    *state = new_onoff_state;
}
