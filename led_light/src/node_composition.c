#include "node_composition.h"


BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

// Bytes distribution
// 4 msg
// 2 opcode
// 1 tt
BT_MESH_MODEL_PUB_DEFINE(light_lightness_srv_pub, NULL, 4 + 2 + 1);

// Comp

static struct bt_mesh_cfg_srv cfg_srv = {
    .relay            = BT_MESH_RELAY_DISABLED,
    .beacon           = BT_MESH_BEACON_ENABLED,
    .frnd             = BT_MESH_FRIEND_NOT_SUPPORTED,
    .gatt_proxy       = BT_MESH_GATT_PROXY_ENABLED,
    .default_ttl      = 7,
    .net_transmit     = BT_MESH_TRANSMIT(2, 20),
    .relay_retransmit = BT_MESH_TRANSMIT(2, 20),
};

static struct bt_mesh_health_srv health_srv = {};
static struct bt_mesh_cfg_cli cfg_cli       = {};

struct bt_mesh_model root_models[] = {
    BT_MESH_MODEL_CFG_SRV(&cfg_srv),
    BT_MESH_MODEL_CFG_CLI(&cfg_cli),
    BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
};

struct bt_mesh_model srv_models[] = {
    BT_MESH_MODEL(BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV, light_lightness_setup_srv_op,
                  &light_lightness_srv_pub, &light_lightness_state_data),
};

static struct bt_mesh_elem elements[] = {
    BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
    BT_MESH_ELEM(0, srv_models, BT_MESH_MODEL_NONE),
};

const struct bt_mesh_comp comp = {
    .cid        = BT_COMP_ID_LF,
    .elem       = elements,
    .elem_count = ARRAY_SIZE(elements),
};
