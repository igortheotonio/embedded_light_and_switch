#include "bt_callbacks.h"

LOG_MODULE_REGISTER(BT_CALLBACKS, 4);

void light_lightness_linear_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf)
{
    u16_t linear                    = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_linear = linear;
    leds.m_brightness               = linear;
    leds_brightness(&leds);
    light_lightness_cli[0].received_msg = 2;
    LOG_INF("PRESENT LIGHTNESS = %04x", linear);
}

void light_lightness_last_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    u16_t actual_last                   = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_last       = actual_last;
    light_lightness_cli[0].received_msg = 2;
    LOG_INF("LAST LIGHTNESS = %04x", actual_last);
}

void light_lightness_default_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                    struct net_buf_simple *buf)
{
    u16_t actual_default                = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_default    = actual_default;
    light_lightness_cli[0].received_msg = 2;
    LOG_INF("DEFAULT LIGHTNESS = %04x", actual_default);
}

void light_lightness_range_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf)
{
    u8_t status_code                   = net_buf_simple_pull_u8(buf);
    u16_t actual_min_range             = net_buf_simple_pull_le16(buf);
    u16_t actual_max_range             = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_min_range = actual_min_range;
    light_lightness_cli[0].m_max_range = actual_max_range;

    light_lightness_cli[0].received_msg = 2;
    LOG_INF("STATUS CODE = %02x", status_code);
    LOG_INF("RANGE MIN = %04x", actual_min_range);
    LOG_INF("RANGE MAX = %04x", actual_max_range);
}


void send_light_lightness_get(struct lightness_cli *bt_cli, u32_t message_type)
{
    struct bt_mesh_model_pub *pub_cli;
    pub_cli = bt_cli->m_model_cli->pub;

    switch (message_type) {
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET:
        LOG_INF("SENDING RANGE GET MESSAGE TO 0x%04x", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET:
        LOG_INF("SENDING DEFAULT GET MESSAGE TO 0x%04x", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET:
        LOG_INF("SENDING LINEAR GET MESSAGE TO 0x%04x", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET:
        LOG_INF("SENDING LAST GET MESSAGE TO 0x%04x", pub_cli->addr);
        break;
    default:
        return;
    }

    bt_mesh_model_msg_init(pub_cli->msg, message_type);
    int err = bt_mesh_model_publish(bt_cli->m_model_cli);
    if (err) {
        LOG_ERR("BT MESH MODEL PUB ERR %d MESSAGE TO 0x%04x", err, pub_cli->addr);
    }
}

void send_light_lightness_linear_set(struct lightness_cli *bt_cli)
{
    struct bt_mesh_model_pub *pub_cli;
    pub_cli = bt_cli->m_model_cli->pub;
    LOG_INF("SENDING SET LINEAR MESSAGE 0x%04x TO 0x%04x", leds.m_brightness, pub_cli->addr);

    bt_mesh_model_msg_init(pub_cli->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET);
    net_buf_simple_add_le16(pub_cli->msg, leds.m_brightness);
    net_buf_simple_add_u8(pub_cli->msg, bt_cli->m_tid++);

    int err = bt_mesh_model_publish(bt_cli->m_model_cli);
    if (err) {
        LOG_ERR("BT MESH MODEL PUB ERR %d MESSAGE TO 0x%04x", err, pub_cli->addr);
    }
}
