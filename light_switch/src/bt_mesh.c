#include "bt_mesh.h"

LOG_MODULE_REGISTER(BT_MESH, 4);

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

void counter_handler(struct k_timer *timer_id)
{
    static u8_t cnt   = 0;
    static u8_t index = 0;

    u32_t messages_types[] = {
        BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET, BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET,
        BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET, BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET};

    if (!light_lightness_cli[0].received_msg) {
        send_light_lightness_get(&light_lightness_cli[0], messages_types[index]);
        light_lightness_cli[0].received_msg = 1;
    } else if (light_lightness_cli[0].received_msg == 1) {
        if (cnt == 3) {
            cnt                                 = 0;
            light_lightness_cli[0].received_msg = 0;
        }
        cnt++;
    } else if (light_lightness_cli[0].received_msg == 2) {
        index++;
        light_lightness_cli[0].received_msg = 0;
    }
    if (index > 3) {
        k_timer_stop(timer_id);
    }
}

int bt_init_and_configure()
{
    light_lightness_cli[0].m_model_cli  = &change_model[0];
    light_lightness_cli[0].received_msg = 0;

    int err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("BLUETOOTH ERROR: %d", err);
        return err;
    }

    // Get status in mesh
    k_timer_init(&timer_counter, counter_handler, NULL);
    k_timer_start(&timer_counter, 200, 200);

    return 0;
}
