#include "encoder.h"

// extern encoder_device_t *encoder_p;
static const u32_t encoder_pins[] = {BUTTON, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B};

int encoder_init(encoder_device_t *encoder_device)
{
    if (!encoder_device) {
        return -ENODEV;
    }
    struct gpio_callback gpio_cb;
    encoder_device->m_device = device_get_binding(BUTTON_CONTROLLER);

    if (!encoder_device->m_device) {
        return -EPERM;
    }

    encoder_device->m_device_cb = gpio_cb;
    encoder_device->m_initiated = 1;
    encoder_device->m_position  = INITIAL_POSITION;
    encoder_device->m_state     = INITIAL_STATE;
    return 0;
}

int encoder_configure(encoder_device_t *encoder_device)
{
    if (encoder_device->m_initiated) {
        for (u8_t i = 0; i < NUMBER_OF_INPUTS; i++) {
            gpio_pin_configure(encoder_device->m_device, encoder_pins[i], STANDARD_BUTTON);
        }

        return 0;
    }
    return -EPERM;
}

int encoder_init_and_configure(encoder_device_t *encoder_device,
                               gpio_callback_handler_t callback_function)
{
    encoder_init(encoder_device);
    encoder_configure(encoder_device);
    encoder_configure_callback(encoder_device, callback_function);

    return 0;
}

void encoder_configure_callback(encoder_device_t *encoder_device,
                                gpio_callback_handler_t callback_function)
{
    gpio_init_callback(&encoder_device->m_device_cb, callback_function,
                       BIT(ENCODER_CHANNEL_A) | BIT(BUTTON));
    gpio_add_callback(encoder_device->m_device, &encoder_device->m_device_cb);
    for (u8_t i = 0; i < NUMBER_OF_INPUTS; i++) {
        gpio_pin_enable_callback(encoder_device->m_device, encoder_pins[i]);
    }
}