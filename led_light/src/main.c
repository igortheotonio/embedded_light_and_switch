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
#include "light_lightness_state.h"
#include "pwm_driver.h"

#define SLEEP_TIME 250

LOG_MODULE_REGISTER(MAIN, 4);

pwm_driver_t pwm = {0};


int configure_board()
{
    int err = init_pwm_driver(&pwm, PWM_DRIVER, PWM_CHANNEL0, PERIOD);
    if (err) {
        return err;
    }
    err = change_pulse_width(&pwm, 0x0000);
    if (err) {
        return err;
    }
    return 0;
}


void main(void)
{
    LOG_INF("Firmware version: %d.%d.%d\n", version_get_major(), version_get_minor(),
            version_get_build());

    int err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("Error on bt enable");
    }

    err = configure_board();
    if (err) {
        LOG_ERR("Error configure board");
    }

    while (1) {
        k_sleep(SLEEP_TIME);
    }
}
