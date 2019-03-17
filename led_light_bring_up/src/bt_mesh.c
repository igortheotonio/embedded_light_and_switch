#include "bt_mesh.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(BT_MESH, 4);

// Bluetooth mesh initialization

static uint8_t dev_uuid[16] = {0xdd, 0xdd};

static int output_number(bt_mesh_output_action_t action, u32_t number)
{
    LOG_WRN("OOB Number: %u\n", number);
    return 0;
}

static int output_string(const char *str)
{
    LOG_WRN("%s string\n", str);
    return 0;
}


static void prov_complete(u16_t net_idx, u16_t addr)
{
    LOG_WRN("Provisioning was completed.\n");
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

void bt_ready(int err)
{
    struct bt_le_oob oob;

    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)\n", err);
        return;
    }

    LOG_WRN("Bluetooth initialized\n");

    err = bt_mesh_init(&prov, &comp);
    if (err) {
        LOG_ERR("Initializing mesh failed (err %d)\n", err);
        return;
    }

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }

    /* Use identity address as device UUID */
    if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob)) {
        LOG_ERR("Identity Address unavailable\n");
    } else {
        memcpy(dev_uuid, oob.addr.a.val, 6);
    }

    bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

    LOG_WRN("Mesh initialized\n");
}
