#include "bt_mesh.h"

static u8_t dev_uuid[16] = {0xdd, 0xdd};

const struct bt_mesh_prov prov = {
    .uuid           = dev_uuid,
    .output_size    = 1,
    .output_actions = BT_MESH_DISPLAY_NUMBER,
    .output_number  = output_number,
    .complete       = prov_complete,
    .reset          = prov_reset,
};

int output_number(bt_mesh_output_action_t action, u32_t number)
{
    printk("OOB Number: %u\n", number);
    return 0;
}

void prov_complete(u16_t net_idx, u16_t addr)
{
    printk("Provisioning was completed.\n");
}

void prov_reset(void)
{
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

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