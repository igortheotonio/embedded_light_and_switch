#include "node_composition.h"

u16_t lightness_cli_state = 0;

struct lightness_cli light_lightness_cli[] = {
    {
        // .leds_cli = &leds,
        .def = 0,
        .act = 0,
        .tid = 0,
    },
};

struct bt_mesh_cfg_srv cfg_srv = {
    .relay            = BT_MESH_RELAY_ENABLED,
    .beacon           = BT_MESH_BEACON_ENABLED,
    .frnd             = BT_MESH_FRIEND_NOT_SUPPORTED,
    .gatt_proxy       = BT_MESH_GATT_PROXY_ENABLED,
    .default_ttl      = 7,
    .net_transmit     = BT_MESH_TRANSMIT(2, 20),
    .relay_retransmit = BT_MESH_TRANSMIT(2, 20),
};

static struct bt_mesh_health_srv health_srv = {};
static struct bt_mesh_cfg_cli cfg_cli       = {};

const struct bt_mesh_model_op light_lightness_cli_op[] = {
    {BT_MESH_MODEL_OP_2(0x82, 0x4E), 2, light_lightness_actual_status},
    {BT_MESH_MODEL_OP_2(0x82, 0x52), 2, light_lightness_linear_status},
    {BT_MESH_MODEL_OP_2(0x82, 0x54), 2, light_lightness_last_status},
    {BT_MESH_MODEL_OP_2(0x82, 0x56), 2, light_lightness_default_status},
    {BT_MESH_MODEL_OP_2(0x82, 0x58), 5, light_lightness_range_status},
    BT_MESH_MODEL_OP_END,
};

BT_MESH_MODEL_PUB_DEFINE(light_lightness_cli_pub, NULL, 2 + 5);
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

void light_lightness_actual_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf)
{
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Actual)\n");
    printk("Present Lightness = %04x\n", net_buf_simple_pull_le16(buf));

    if (buf->len == 3) {
        printk("Target Lightness = %04x\n", net_buf_simple_pull_le16(buf));
        printk("Remaining Time = %02x\n", net_buf_simple_pull_u8(buf));
    }
}

void light_lightness_linear_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                   struct net_buf_simple *buf)
{
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Linear)\n");
    printk("Present Lightness = %04x\n", net_buf_simple_pull_le16(buf));

    if (buf->len == 3) {
        printk("Target Lightness = %04x\n", net_buf_simple_pull_le16(buf));
        printk("Remaining Time = %02x\n", net_buf_simple_pull_u8(buf));
    }
}

void light_lightness_last_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                 struct net_buf_simple *buf)
{
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Last)\n");
    printk("Lightness = %04x\n", net_buf_simple_pull_le16(buf));
}

void light_lightness_default_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                    struct net_buf_simple *buf)
{
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Default)\n");
    printk("Lightness = %04x\n", net_buf_simple_pull_le16(buf));
}

void light_lightness_range_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf)
{
    printk("Acknownledgement from LIGHT_LIGHTNESS_SRV (Lightness Range)\n");
    printk("Status Code = %02x\n", net_buf_simple_pull_u8(buf));
    printk("Range Min = %04x\n", net_buf_simple_pull_le16(buf));
    printk("Range Max = %04x\n", net_buf_simple_pull_le16(buf));
}

void lightness_level_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                         struct net_buf_simple *buf)
{
    printk("[0x%04x]: Received a get level msg from group address 0x%04x, sent by 0x%04x.\n",
           bt_mesh_model_elem(model)->addr, ctx->recv_dst, ctx->addr);
    struct bt_mesh_model_pub *pub_cli;
    pub_cli                = model->pub;
    struct level_srv *elem = model->user_data;

    // 2 bytes for the opcode
    // 1 bytes parameters: present onoff value
    // 2 optional bytes for target onoff and remaining time
    // 4 additional bytes for the TransMIC
    printk("Sending level status msg to 0x%04x, value -> %d\n", pub_cli->addr, level_perc);
    bt_mesh_model_msg_init(pub_cli->msg, BT_MESH_MODEL_OP_2(0x82, 0x4E));
    net_buf_simple_add_le16(pub_cli->msg, level);
    int err = bt_mesh_model_publish(model);
    if (err) {
        printk("bt_mesh_model_publish err %d, sending msg to 0x%04x\n", err, pub_cli->addr);
    }
}
