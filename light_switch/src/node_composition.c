#include "node_composition.h"

LOG_MODULE_REGISTER(NODE_COMPOSITION);

struct lightness_cli light_lightness_cli[] = {
    {
        .m_linear     = 0x0000,
        .m_last       = 0xFFFF,
        .m_default    = 0x0000,
        .m_min_range  = 0x0001,
        .m_max_range  = 0xFFFF,
        .m_tid        = 0,
        .received_msg = 0,
    },
};

u16_t lightness_cli_state = 0;

struct bt_mesh_cfg_srv cfg_srv = {
    .relay            = BT_MESH_RELAY_ENABLED,
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