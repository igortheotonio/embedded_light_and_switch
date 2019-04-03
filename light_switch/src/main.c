#include <logging/log.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "bt_mesh.h"
#include "encoder.h"
#include "firmware_version.h"
#include "leds.h"

LOG_MODULE_REGISTER(MAIN);

encoder_device_t encoder = {0};
leds_device_t leds       = {0};

int setup_peripherals()
{
    int err = 0;

    // Init and configure leds
    err = leds_init_and_configure(&leds);

    if (err) {
        LOG_ERR("LEDS ERROR: %d", err);
        return err;
    }

    // Init and configure bluetooh
    err = bt_init_and_configure();

    if (err) {
        LOG_ERR("BLUETOOTH ERROR: %d", err);
        return err;
    }

    // Init and configure encoder
    err = encoder_init_and_configure(&encoder);

    if (err) {
        LOG_ERR("ENCODER ERROR: %d", err);
        return err;
    }

    return 0;
}

void main(void)
{
    int err = 0;

    // Firmware version
    LOG_WRN("FIRMWARE VERSION: %d.%d.%d\n", version_get_major(), version_get_minor(),
            version_get_build());

    // Inits peripherals
    err = setup_peripherals();

    if (err) {
        LOG_ERR("ERROR IN SETUP");
        return;
    }

    // Loop
    while (1) {
        k_sleep(K_MSEC(SLEEP_TIME));
    }
}
