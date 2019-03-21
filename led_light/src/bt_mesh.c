/**
 * @file mesh.c
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#include "bt_mesh.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(BT_MESH, 4);

#define ROOT_ADDR 0x00e1
#define SETUP_SERVER_MODEL_ADDR 0x00e2
#define SERVER_MODEL_ADDR 0x00e3

#define LIGHT_LIGHTNESS_SRV_GROUP_ADDR 0xcb01
#define LIGHT_LIGHTNESS_SETUP_SRV_GROUP_ADDR 0xbb01
#define SWITCHS_GROUP_ADDR 0xcb10

static const u8_t dev_uuid[16] = {0xcf, 0xa0, 0xea, 0x7e, 0x17, 0xd9, 0x11, 0xe8,
                                  0x86, 0xd1, 0x5f, 0x1c, 0xe2, 0x8a, 0xde, 0xa7};

// Security keys
static const u8_t net_key[16] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};
static const u8_t dev_key[16] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};
static const u8_t app_key[16] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};

// Variables for configuration and self provisioning
static const u16_t net_idx;
static const u16_t app_idx;
static const u32_t iv_index;
static u8_t flags;


static int output_number(bt_mesh_output_action_t action, u32_t number)
{
    printk("OOB Number: %u\n", number);
    return 0;
}

static int output_string(const char *str)
{
    printk("OOB String: %s\n", str);
    return 0;
}

static void prov_complete(u16_t net_idx, u16_t addr)
{
}

static void prov_reset(void)
{
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

static const struct bt_mesh_prov prov = {
    .uuid           = dev_uuid,
    .output_size    = 1,
    .output_actions = BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING,
    .output_number  = output_number,
    .output_string  = output_string,
    .complete       = prov_complete,
    .reset          = prov_reset,
};

static int self_provision()
{
    // now we provision ourselves... this is not how it would normally be done!
    int err = bt_mesh_provision(net_key, net_idx, flags, iv_index, ROOT_ADDR, dev_key);
    if (err) {
        LOG_ERR("Provisioning failed (err %d)\n", err);
        return err;
    }
    LOG_INF("Provisioning completed\n");
    return 0;
}

static int self_configure()
{
    int err;
    LOG_INF("configuring...\n");

    /* Add Application Key to root models*/
    err = bt_mesh_cfg_app_key_add(net_idx, ROOT_ADDR, net_idx, app_idx, app_key, NULL);
    if (err) {
        LOG_ERR("ERROR adding appkey (err %d)\n", err);
        return err;
    } else {
        LOG_INF("added appkey\n");
    }

    /* Bind to Health model */
    err = bt_mesh_cfg_mod_app_bind(net_idx, ROOT_ADDR, ROOT_ADDR, app_idx,
                                   BT_MESH_MODEL_ID_HEALTH_SRV, NULL);
    if (err) {
        LOG_ERR("ERROR binding to health server model (err %d)\n", err);
        return err;
    } else {
        LOG_INF("bound appkey to health server model\n");
    }


    /* Bind to light lightness setup server model */
    err = bt_mesh_cfg_mod_app_bind(net_idx, ROOT_ADDR, SETUP_SERVER_MODEL_ADDR, app_idx,
                                   BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV, NULL);
    if (err) {
        LOG_ERR("ERROR binding to light lightness setup server model (err %d)\n", err);
        return err;
    } else {
        LOG_INF("bound appkey to light lightness setup server model\n");
    }

    // subscribe srv model to the group address
    err = bt_mesh_cfg_mod_sub_add(net_idx, ROOT_ADDR, SETUP_SERVER_MODEL_ADDR,
                                  LIGHT_LIGHTNESS_SETUP_SRV_GROUP_ADDR,
                                  BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV, NULL);
    if (err) {
        LOG_ERR("ERROR subscribing to group address (err %d)\n", err);
        return err;
    } else {
        LOG_INF("subscribed setup server to group address 0x%04x",
                LIGHT_LIGHTNESS_SETUP_SRV_GROUP_ADDR);
    }

    /* Bind to light lightness server model */
    err = bt_mesh_cfg_mod_app_bind(net_idx, ROOT_ADDR, SERVER_MODEL_ADDR, app_idx,
                                   BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV, NULL);
    if (err) {
        LOG_ERR("ERROR binding to light lightness server model (err %d)\n", err);
        return err;
    } else {
        LOG_INF("bound appkey to light lightness server model\n");
    }

    // subscribe srv model to the group address
    err = bt_mesh_cfg_mod_sub_add(net_idx, ROOT_ADDR, SERVER_MODEL_ADDR,
                                  LIGHT_LIGHTNESS_SRV_GROUP_ADDR,
                                  BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV, NULL);
    if (err) {
        LOG_ERR("ERROR subscribing to group address (err %d)\n", err);
        return err;
    } else {
        LOG_INF("subscribed light lightness server to group address 0x%04x",
                LIGHT_LIGHTNESS_SRV_GROUP_ADDR);
    }

    return 0;
}


void bt_ready(int err)
{
    if (err) {
        LOG_ERR("bt_enable init failed with err %d\n", err);
        return;
    }

    LOG_INF("Bluetooth initialised OK\n");

    // self-provision and initialise with node composition data
    err = bt_mesh_init(&prov, &comp);
    if (err) {
        LOG_ERR("bt_mesh_init failed with err %d\n", err);
        return;
    }

    LOG_INF("Mesh initialized\n");

    /*if (IS_ENABLED(CONFIG_BT_SETTINGS)) {*/
    /*LOG_INF("Loading stored settings\n");*/
    /*settings_load();*/
    /*}*/

    err = self_provision();
    if (err) {
        LOG_ERR("ERROR: SELF-PROVISIONING FAILED");
        return;
    } else {
        LOG_INF("self-provisioned OK\n");
    }

    err = self_configure();

    if (err) {
        LOG_ERR("ERROR: INITIALISATION FAILED");
        return;
    } else {
        LOG_INF("Node configure OK\n");
    }
}
