#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

#include "encoder.h"
#include "leds.h"

static encoder_device_t encoder = {0};
static leds_device_t leds       = {0};

void main(void)
{
    encoder_init_and_configure(&encoder);
    leds_init(&leds);
    while (1) {
    }
}