#include <logging/log.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "encoder.h"
#include "leds.h"

LOG_MODULE_REGISTER(MAIN);

static encoder_device_t encoder = {0};
static leds_device_t leds       = {0};
encoder_device_t *encoder_p     = &encoder;
leds_device_t *leds_p           = &leds;

void main(void)
{
    leds_init_and_configure(&leds);
    encoder_init_and_configure(&encoder);
    u16_t brightness = 0;
    while (1) {
        printk("");
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
        }
    }
}