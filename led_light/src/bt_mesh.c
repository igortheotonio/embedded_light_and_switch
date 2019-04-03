/**
 * @file mesh.c
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#include "bt_mesh.h"

#include <logging/log.h>

#include "pwm_driver.h"

LOG_MODULE_REGISTER(BT_MESH, 4);

static const u8_t dev_uuid[16] = {0xcf, 0xa0, 0xea, 0x7e, 0x17, 0xd9, 0x11, 0xe8,
                                  0x85, 0xd1, 0x5f, 0x1c, 0xe2, 0x8a, 0xde, 0xa7};

static int output_number(bt_mesh_output_action_t action, u32_t number)
{
    LOG_INF("OOB Number: %u\n", number);
    blink_light(&pwm, number);
    LOG_INF("OOB sent\n");
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
    .output_actions = BT_MESH_DISPLAY_NUMBER,
    .output_number  = output_number,
    .complete       = prov_complete,
    .reset          = prov_reset,
};

void bt_ready(int err)
{
    if (err) {
        LOG_INF("bt_enable init failed with err %d\n", err);
        return;
    }

    LOG_INF("Bluetooth initialized.\n");
    err = bt_mesh_init(&prov, &comp);
    if (err) {
        LOG_INF("bt_mesh init failed with err %d\n", err);
        return;
    }

    if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
        settings_load();
    }

    /* This will be a no-op if settings_load() loaded provisioning info */
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
    LOG_INF("Mesh initialized.\n");
}
