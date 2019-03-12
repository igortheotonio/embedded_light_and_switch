#include <logging/log.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "encoder.h"
#include "leds.h"

LOG_MODULE_REGISTER(MAIN);

static encoder_device_t encoder = {0};
static leds_device_t leds       = {0};
// leds_device_t *leds_p           = &leds;
// encoder_device_t *encoder_p     = &encoder;

void callback_function(struct device *encoder_device, struct gpio_callback *callback,
                       u32_t encoder_pin_mask);

void main(void)
{
    leds_init_and_configure(&leds);
    encoder_init_and_configure(&encoder, callback_function);
    u16_t brightness = 0;
    while (1) {
        if (encoder.m_position) {
            if (leds.m_brightness + 2500 * encoder.m_position > 65535) {
                brightness = 65535;
            } else if (leds.m_brightness + 2500 * encoder.m_position < 0) {
                brightness = 0;
            } else {
                brightness = leds.m_brightness + 2500 * encoder.m_position;
            }
            printk("Change value to %d\n", brightness);
            encoder.m_position = 0;
            leds_brightness(&leds, brightness);
            leds_set_brightness(&leds, brightness);
            encoder_configure(&encoder);
        }
        printk("Run...\n");
    }
}

void callback_function(struct device *encoder_device, struct gpio_callback *callback,
                       u32_t encoder_pin_mask)
{
    u32_t a_state = 0;
    u32_t b_state = 0;

    switch (encoder_pin_mask) {
    case BIT(BUTTON):
        printk("Button pressed!\n");
        leds_change_state(&leds);
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
