#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <zephyr/types.h>

#include "defines.h"

// Buttons device struct
typedef struct {
    struct device *ic_device;
    struct gpio_callback ic_device_cb;
    u8_t initiated;
} ic_buttons_device_t;

int ic_buttons_init_device(ic_buttons_device_t *buttons_device);

void ic_buttons_configure(ic_buttons_device_t *buttons_device);

void ic_buttons_configure_callback(ic_buttons_device_t *buttons_device,
                                   gpio_callback_handler_t callback_function);

#endif  // _IC_BUTTONS_H_
