/**
 * @file main.c
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */


#include <logging/log.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include "bt_mesh.h"
#include "firmware_version.h"

#define SLEEP_TIME 100

LOG_MODULE_REGISTER(MAIN, 4);


void main(void)
{
    LOG_INF("Firmware version: %d.%d.%d\n", version_get_major(), version_get_minor(),
            version_get_build());

    int err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("Error log.");
    }
    while (1) {
        k_sleep(SLEEP_TIME);
    }
}
