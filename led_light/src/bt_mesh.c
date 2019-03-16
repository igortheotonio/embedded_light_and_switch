/**
 * @file mesh.c
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#include "bt_mesh.h"


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
    struct bt_le_oob oob;

    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    err = bt_mesh_init(&prov, &comp);
    if (err) {
        printk("Initializing mesh failed (err %d)\n", err);
        return;
    }

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }

    /* Use identity address as device UUID */
    if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob)) {
        printk("Identity Address unavailable\n");
    } else {
        memcpy(dev_uuid, oob.addr.a.val, 6);
    }

    bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

    printk("Mesh initialized\n");
}