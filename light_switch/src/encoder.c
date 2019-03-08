#include "encoder.h"

int encoder_init_device(encoder_device_t *encoder_device)
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
    encoder_device->initiated   = 1;
    return 0;
}

int encoder_configure(encoder_device_t *encoder_device)
{
    if (encoder_device->initiated) {
        for (u8_t i = 0; i < 3; i++) {
            gpio_pin_configure(encoder_device->m_device, encoder_pins[i], STANDARD_BUTTON);
        }
        encoder_configure_callback(encoder_device);
        return 0;
    }
    return -EPERM;
}

void encoder_init_and_configure(encoder_device_t *encoder_device)
{
    encoder_init_device(encoder_device);
    encoder_configure(encoder_device);
}

void channel_callback(struct device *encoder_device, struct gpio_callback *callback,
                      u32_t encoder_pin_mask)
{
    u32_t a_state         = 0;
    u32_t b_state         = 0;
    static u8_t old_state = 3;
    static s8_t position  = 0;
    gpio_pin_read(encoder_device, ENCODER_CHANNEL_A, &a_state);
    gpio_pin_read(encoder_device, ENCODER_CHANNEL_B, &b_state);
    u8_t this_state = a_state | (b_state << 1);
    if (old_state != this_state) {
        position += encoder_diretion[this_state | (old_state << 2)];
    }
    printk("%d \n", position);
    old_state = this_state;
}

void button_callback(struct device *encoder_device, struct gpio_callback *callback,
                     u32_t encoder_pin_mask)
{
}

void encoder_configure_callback(encoder_device_t *encoder_device)
{
    gpio_init_callback(&encoder_device->m_device_cb, button_callback, BIT(BUTTON));
    gpio_init_callback(&encoder_device->m_device_cb, channel_callback,
                       BIT(ENCODER_CHANNEL_A) | BIT(ENCODER_CHANNEL_B));
    gpio_add_callback(encoder_device->m_device, &encoder_device->m_device_cb);
    for (u8_t i = 0; i < 3; i++) {
        gpio_pin_enable_callback(encoder_device->m_device, encoder_pins[i]);
    }
}