#include "node_composition.h"

struct lightness_cli light_lightness_cli[] = {
    {
        .m_linear    = 0x0000,
        .m_last      = 0xFFFF,
        .m_default   = 0x0000,
        .m_min_range = 0x0001,
        .m_max_range = 0xFFFF,
        .m_tid       = 0,
    },
};

u16_t lightness_cli_state = 0;

struct bt_mesh_cfg_srv cfg_srv = {
    .relay            = BT_MESH_RELAY_DISABLED,
    .beacon           = BT_MESH_BEACON_DISABLED,
    .frnd             = BT_MESH_FRIEND_NOT_SUPPORTED,
    .gatt_proxy       = BT_MESH_GATT_PROXY_ENABLED,
    .default_ttl      = 7,
    .net_transmit     = BT_MESH_TRANSMIT(1, 20),
    .relay_retransmit = BT_MESH_TRANSMIT(1, 20),
};

static struct bt_mesh_health_srv health_srv = {};
static struct bt_mesh_cfg_cli cfg_cli       = {};

const struct bt_mesh_model_op light_lightness_cli_op[] = {
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS, 2, light_lightness_linear_status},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_STATUS, 2, light_lightness_last_status},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS, 2, light_lightness_default_status},
    {BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS, 5, light_lightness_range_status},
    BT_MESH_MODEL_OP_END,
};

BT_MESH_MODEL_PUB_DEFINE(light_lightness_cli_pub, NULL, 2 + 5 + 4);
BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

struct bt_mesh_model change_model[] = {
    BT_MESH_MODEL(BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI, light_lightness_cli_op,
                  &light_lightness_cli_pub, &light_lightness_cli[0]),
};

struct bt_mesh_model root_models[] = {
    BT_MESH_MODEL_CFG_SRV(&cfg_srv),
    BT_MESH_MODEL_CFG_CLI(&cfg_cli),
    BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
};

struct bt_mesh_elem elements[] = {
    BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
    BT_MESH_ELEM(0, change_model, BT_MESH_MODEL_NONE),
};

const struct bt_mesh_comp comp = {
    .cid        = BT_COMP_ID_LF,
    .elem       = elements,
    .elem_count = ARRAY_SIZE(elements),
};

void light_lightness_linear_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf)
{
    u16_t linear                    = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_linear = linear;
    leds.m_brightness               = linear;
    leds_brightness(&leds);
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Linear)\n");
    printk("Present Lightness = %04x\n", linear);
}

void light_lightness_last_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    u16_t actual_last             = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_last = actual_last;
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Last)\n");
    printk("Last Lightness = %04x\n", actual_last);
}

void light_lightness_default_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                    struct net_buf_simple *buf)
{
    u16_t actual_default             = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_default = actual_default;
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Default)\n");
    printk("Default Lightness = %04x\n", actual_default);
}

void light_lightness_range_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf)
{
    u8_t status_code                   = net_buf_simple_pull_u8(buf);
    u16_t actual_min_range             = net_buf_simple_pull_le16(buf);
    u16_t actual_max_range             = net_buf_simple_pull_le16(buf);
    light_lightness_cli[0].m_min_range = actual_min_range;
    light_lightness_cli[0].m_max_range = actual_max_range;

    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Lightness Range)\n");
    printk("Status Code = %02x\n", status_code);
    printk("Range Min = %04x\n", actual_min_range);
    printk("Range Max = %04x\n", actual_max_range);
}


void send_light_lightness_get(struct lightness_cli *bt_cli, u32_t message_type)
{
    struct bt_mesh_model_pub *pub_cli;
    pub_cli = bt_cli->m_model_cli->pub;

    switch (message_type) {
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET:
        printk("Sending range get message to 0x%04x\n", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET:
        printk("Sending default get message to 0x%04x\n", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET:
        printk("Sending linear get message to 0x%04x\n", pub_cli->addr);
        break;
    case BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET:
        printk("Sending last get message to 0x%04x\n", pub_cli->addr);
        break;
    default:
        return;
    }

    bt_mesh_model_msg_init(pub_cli->msg, message_type);
    int err = bt_mesh_model_publish(bt_cli->m_model_cli);
    if (err) {
        printk("BT MESH MODEL PUB ERR %d message to 0x%04x\n", err, pub_cli->addr);
    }
}

void send_light_lightness_linear_set(struct lightness_cli *bt_cli)
{
    struct bt_mesh_model_pub *pub_cli;
    pub_cli = bt_cli->m_model_cli->pub;
    printk("Sending set linear message to 0x%04x\n", pub_cli->addr);
    bt_mesh_model_msg_init(pub_cli->msg, BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET);
    net_buf_simple_add_le16(pub_cli->msg, leds.m_brightness);
    net_buf_simple_add_u8(pub_cli->msg, bt_cli->m_tid++);
    int err = bt_mesh_model_publish(bt_cli->m_model_cli);
    if (err) {
        printk("BT MESH MODEL PUB ERR %d message to 0x%04x\n", err, pub_cli->addr);
    }
}
