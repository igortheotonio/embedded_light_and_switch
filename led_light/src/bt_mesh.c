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

static u8_t dev_uuid[16] = {0xdd, 0xdd};

static const struct bt_mesh_prov prov = {
    .uuid           = dev_uuid,
    .output_size    = 1,
    .output_actions = BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING,
    .output_number  = output_number,
    .output_string  = output_string,
    .complete       = prov_complete,
    .reset          = prov_reset,
};

void bt_ready(int err)
{
    if (err) {
        printk("bt_enable init failed with err %d\n", err);
        return;
    }

    printk("Bluetooth initialized.\n");
    err = bt_mesh_init(&prov, &comp);
    if (err) {
        printk("bt_mesh init failed with err %d\n", err);
        return;
    }

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }

    /* This will be a no-op if settings_load() loaded provisioning info */
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
    printk("Mesh initialized.\n");
}
