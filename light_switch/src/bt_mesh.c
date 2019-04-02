#include "bt_mesh.h"

LOG_MODULE_REGISTER(BT_MESH);

static u8_t dev_uuid[16] = {0xda, 0xda};

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
    LOG_WRN("OOB Number: %u", number);
    printk("OOB Number: %u\n", number);
    return 0;
}

void prov_complete(u16_t net_idx, u16_t addr)
{
    LOG_INF("PROVISIONING WAS COMPLETED");
}

void prov_reset(void)
{
    LOG_INF("PROVISION RESET");
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

void bt_ready(int err)
{
    LOG_INF("BT MESH INIT");

    if (err) {
        LOG_ERR("BT MESH INIT FAILD ERR: %d", err);
        return;
    }

    err = bt_mesh_init(&prov, &comp);
    if (err) {
        LOG_ERR("BT MESH INIT FAILED WITH ERR: %d", err);
        return;
    }

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }

    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
    LOG_INF("FINISH BT MESH INIT");
}
