#include "encoder.h"

LOG_MODULE_REGISTER(ENCONDER, 4);

static u32_t time, button_last_time;
static struct k_timer timer_enconder;
static const u32_t encoder_pins[] = {BUTTON, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B};

void enconder_handler(struct k_timer *timer_id)
{
    int new_brightness = leds.m_brightness + STEP_ENC * encoder.m_position;
    new_brightness     = max(MIN_BRIGHTNESS, new_brightness);
    new_brightness     = min(MAX_BRIGHTNESS, new_brightness);

    if (leds.m_brightness != new_brightness) {
        leds.m_brightness = new_brightness;
        LOG_WRN("CHANGE BRIGHTNESS TO %d", leds.m_brightness);
        send_light_lightness_linear_set(&light_lightness_cli[0]);
    }
    encoder.m_position = 0;
    k_timer_stop(timer_id);
}

int encoder_init(encoder_device_t *encoder_device)
{
    if (!encoder_device) {
        LOG_ERR("ENODEV");
        return -ENODEV;
    }
    struct gpio_callback gpio_cb;
    encoder_device->m_device = device_get_binding(BUTTON_CONTROLLER);

    if (!encoder_device->m_device) {
        LOG_ERR("EPERM");
        return -EPERM;
    }

    encoder_device->m_device_cb = gpio_cb;
    encoder_device->m_initiated = 1;
    button_last_time            = k_uptime_get_32();
    encoder_device->m_position  = INITIAL_POSITION;
    encoder_device->m_state     = INITIAL_STATE;

    k_timer_init(&timer_enconder, enconder_handler, NULL);
    k_timer_start(&timer_enconder, 500, 500);

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
    int err = 0;
    LOG_INF("ENCODER INIT");

    err = encoder_init(encoder_device);
    if (err) {
        return err;
    }

    err = encoder_configure(encoder_device);
    if (err) {
        return err;
    }

    encoder_configure_callback(encoder_device);

    LOG_INF("FINISH ENCODER INIT");
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
            LOG_DBG("DEBOUNCING IN BUTTON");
            return;
        }

        LOG_DBG("BUTTON PRESSED");

        if (light_lightness_cli[0].m_linear == 0) {
            if (light_lightness_cli[0].m_default == 0) {
                leds.m_brightness = actual_to_linear(light_lightness_cli[0].m_last);
                send_light_lightness_linear_set(&light_lightness_cli[0]);
            } else if (light_lightness_cli[0].m_default == 0xFFFF) {
                leds.m_brightness = 0xFFFF;
                send_light_lightness_linear_set(&light_lightness_cli[0]);
            } else {
                leds.m_brightness = light_lightness_cli[0].m_default;
                send_light_lightness_linear_set(&light_lightness_cli[0]);
            }
        } else {
            leds.m_brightness = 0;
            send_light_lightness_linear_set(&light_lightness_cli[0]);
        }
        button_last_time = time;
        break;
    case BIT(ENCODER_CHANNEL_A):
    case BIT(ENCODER_CHANNEL_B):
    case BIT(ENCODER_CHANNEL_A) | BIT(ENCODER_CHANNEL_B):

        gpio_pin_read(encoder_device, ENCODER_CHANNEL_A, &a_state);
        gpio_pin_read(encoder_device, ENCODER_CHANNEL_B, &b_state);
        encoder.m_position += encoder_diretion[(a_state << SIZE_ONE_STATE) | b_state];

        LOG_DBG("ENCODER SPIN");

        if (k_timer_status_get(&timer_enconder) > 0) {
            LOG_DBG("NEW THREAD");
            k_timer_start(&timer_enconder, 500, 500);
        }

        break;
    default:
        LOG_ERR("INVALID STATE");
    }
}

void encoder_configure_callback(encoder_device_t *encoder_device)
{
    LOG_INF("ENCODER CONFIGURE CALLBACKS");

    gpio_init_callback(&encoder_device->m_device_cb, callback_function,
                       BIT(ENCODER_CHANNEL_A) | BIT(BUTTON));
    gpio_add_callback(encoder_device->m_device, &encoder_device->m_device_cb);
    for (u8_t i = 0; i < NUMBER_OF_INPUTS; i++) {
        gpio_pin_enable_callback(encoder_device->m_device, encoder_pins[i]);
    }
}
