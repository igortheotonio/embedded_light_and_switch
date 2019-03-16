#include <logging/log.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "bt_mesh.h"
#include "encoder.h"
#include "leds.h"

LOG_MODULE_REGISTER(MAIN);

encoder_device_t encoder = {0};
leds_device_t leds       = {0};

void main(void)
{
    // printk("Firmware version: %d.%d.%d\n", ic_version_get_major(), ic_version_get_minor(),
    //        ic_version_get_build());
    int err;
    leds_init_and_configure(&leds);
    encoder_init_and_configure(&encoder);
    err = bt_enable(bt_ready);
    if (err) {
        printk("bt_enable failed with err %d\n", err);
        return;
    }

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
            leds_brightness(&leds, brightness);
            leds_set_brightness(&leds, brightness);
        }
        k_sleep(1);
    }
}
