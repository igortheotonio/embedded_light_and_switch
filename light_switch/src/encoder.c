#include "encoder.h"

static u32_t time, button_last_time;
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
    button_last_time            = k_uptime_get_32();
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

int encoder_init_and_configure(encoder_device_t *encoder_device)
{
    encoder_init(encoder_device);
    encoder_configure(encoder_device);
    encoder_configure_callback(encoder_device);
    return 0;
}


void callback_function(struct device *encoder_device, struct gpio_callback *callback,
                       u32_t encoder_pin_mask)
{
    u32_t a_state = 0;
    u32_t b_state = 0;
    time          = k_uptime_get_32();
    switch (encoder_pin_mask) {
    case BIT(BUTTON):
        if (time < button_last_time + BUTTON_DEBOUNCE_DELAY) {
            button_last_time = time;
            return;
        }

        get_all_data();

        if (light_lightness_cli[0].m_actual == 0) {
            if (light_lightness_cli[0].m_default == 0) {
                leds.m_brightness = light_lightness_cli[0].m_last;
                send_light_lightness_actual_set(&light_lightness_cli[0],
                                                BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET);
            } else if (light_lightness_cli[0].m_default == 0xFFFF) {
                leds.m_brightness = 0xFFFF;
                send_light_lightness_actual_set(&light_lightness_cli[0],
                                                BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET);
            } else {
                leds.m_brightness = light_lightness_cli[0].m_default;
                send_light_lightness_actual_set(&light_lightness_cli[0],
                                                BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET);
            }
        } else {
            leds.m_brightness = 0;
            send_light_lightness_actual_set(&light_lightness_cli[0],
                                            BT_MESH_MODEL_LIGHT_LIGHTNESS_ACTUAL_SET);
        }
        printk("Button pressed!\n");
        button_last_time = time;
        break;
    case BIT(ENCODER_CHANNEL_A):
    case BIT(ENCODER_CHANNEL_B):
    case BIT(ENCODER_CHANNEL_A) | BIT(ENCODER_CHANNEL_B):
        gpio_pin_read(encoder_device, ENCODER_CHANNEL_A, &a_state);
        gpio_pin_read(encoder_device, ENCODER_CHANNEL_B, &b_state);
        u8_t new_state = a_state | (b_state << SIZE_ONE_STATE);
        if (encoder.m_state != new_state) {
            encoder.m_position += encoder_diretion[new_state | (encoder.m_state << SIZE_TWO_STATE)];
            encoder.m_state = new_state;
        }
        printk("Position %d\n", encoder.m_position);
        break;
    default:
        printk("Invalid state\n");
    }
}

void encoder_configure_callback(encoder_device_t *encoder_device)
{
    gpio_init_callback(&encoder_device->m_device_cb, callback_function,
                       BIT(ENCODER_CHANNEL_A) | BIT(BUTTON));
    gpio_add_callback(encoder_device->m_device, &encoder_device->m_device_cb);
    for (u8_t i = 0; i < NUMBER_OF_INPUTS; i++) {
        gpio_pin_enable_callback(encoder_device->m_device, encoder_pins[i]);
    }
}