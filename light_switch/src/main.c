#include <logging/log.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "bt_mesh.h"
#include "encoder.h"
#include "firmware_version.h"
#include "leds.h"
#include "node_composition.h"

LOG_MODULE_REGISTER(MAIN);

encoder_device_t encoder = {0};
leds_device_t leds       = {0};

u8_t index             = 0;
u32_t messages_types[] = {
    BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET, BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET,
    BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET, BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET};

struct k_timer timer_counter;


void counter_handler(struct k_timer *timer_id)
{
    send_light_lightness_get(&light_lightness_cli[0], messages_types[index++]);
    if (index > 3) {
        k_timer_stop(timer_id);
    }
}


void main(void)
{
    printk("Firmware version: %d.%d.%d\n", version_get_major(), version_get_minor(),
           version_get_build());
    int err;
    leds_init_and_configure(&leds);

    light_lightness_cli[0].m_model_cli = &change_model[0];

    err = bt_enable(bt_ready);
    if (err) {
        printk("bt_enable failed with err %d\n", err);
        return;
    }

    k_timer_init(&timer_counter, counter_handler, NULL);
    k_timer_start(&timer_counter, 200, 200);

    encoder_init_and_configure(&encoder);

    u16_t brightness = 0;


    while (1) {
        if (encoder.m_position) {
            if (leds.m_brightness + 2500 * encoder.m_position > 65535) {
                brightness = 65535;
            } else if (leds.m_brightness + 2500 * encoder.m_position < 0) {
                brightness = 0;
            } else {
                brightness = leds.m_brightness + 2500 * encoder.m_position;
            }
            printk("Change value to %d\n", brightness);
            encoder.m_position = 0;
            leds_brightness(&leds);
        }
        k_sleep(250);
    }
}
