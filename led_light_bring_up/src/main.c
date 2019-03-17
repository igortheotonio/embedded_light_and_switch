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
#include "global_state.h"
#include "pwm_driver.h"

#define SLEEP_TIME 250

pwm_driver_t pwm_driver = {0};
u8_t state              = 0;

LOG_MODULE_REGISTER(MAIN, 4);

// Initializing BLE

int configure_board()
{
    if (init_pwm_driver(&pwm_driver, PWM_DRIVER, PWM_CHANNEL0, PERIOD)) {
        return -EINVAL;
    }
    return 0;
}

void main(void)
{
    LOG_WRN("Firmware version: %d.%d.%d\n", version_get_major(), version_get_minor(),
            version_get_build());

    int err = configure_board();

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
        if (!state) {
            change_pulse_width(&pwm_driver, PERIOD / 2);
            set_pulse_width(&pwm_driver);
        } else {
            change_pulse_width(&pwm_driver, 0);
            set_pulse_width(&pwm_driver);
        }
        k_sleep(SLEEP_TIME);
    }
}
