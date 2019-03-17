/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2018-12-25
 *
 * @copyright Copyright (c) 2018
 *
 */

#include <logging/log.h>
#include <zephyr/types.h>

#include "bt_mesh.h"
#include "firmware_version.h"

#define SLEEP_TIME 250


LOG_MODULE_REGISTER(MAIN, 4);

// Initializing BLE

void main(void)
{
    LOG_WRN("Firmware version: %d.%d.%d\n", version_get_major(), version_get_minor(),
            version_get_build());

    int err = 0;

    if (err) {
        LOG_ERR("The board configuration failed with err %d", err);
        return;
    }

    err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("bt_enable failed with err %d\n", err);
        return;
    }
    while (1) {
        k_sleep(SLEEP_TIME);
    }
}
