#include "ic_buttons.h"

int ic_buttons_init_device(ic_buttons_device_t *buttons_device)
{
    struct gpio_callback gpio_cb;
    buttons_device->ic_device    = device_get_binding(BUTTONS_PORT);
    buttons_device->ic_device_cb = gpio_cb;
    buttons_device->initiated    = 1;
    return 0;
}

void ic_buttons_configure(ic_buttons_device_t *buttons_device)
{
    if (buttons_device->initiated) {
        gpio_pin_configure(buttons_device->ic_device, BUTTON0,
                           GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);
    }
}

void ic_buttons_configure_callback(ic_buttons_device_t *buttons_device,
                                   gpio_callback_handler_t callback_function)
{
    if (buttons_device->initiated) {
        gpio_init_callback(&buttons_device->ic_device_cb, callback_function, BIT(BUTTON0));
        gpio_add_callback(buttons_device->ic_device, &buttons_device->ic_device_cb);
        gpio_pin_enable_callback(buttons_device->ic_device, BUTTON0);
    }
}