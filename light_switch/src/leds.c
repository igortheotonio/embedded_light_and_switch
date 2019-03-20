#include "leds.h"

static const u32_t leds_pins[] = {LED0, LED1, LED2, LED3, LED4};

int leds_init(leds_device_t *leds_device)
{
    if (!leds_device) {
        return -ENODEV;
    }
    leds_device->m_device = device_get_binding(LED_PORT);
    if (!leds_device->m_device) {
        return -EPERM;
    }
    leds_device->m_initiated = 1;
    leds_device->m_state     = OFF;

    return 0;
}

int leds_configure(leds_device_t *leds_device)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }
    for (u8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        gpio_pin_configure(leds_device->m_device, leds_pins[i], STANDARD_LED);
    }
    return 0;
}

int turn_all_leds_on(leds_device_t *leds_device)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }

    for (u8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_ON);
    }

    return 0;
}

int turn_all_leds_off(leds_device_t *leds_device)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }

    for (u8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_OFF);
    }

    return 0;
}

int leds_brightness(leds_device_t *leds_device)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }
    if (leds.m_brightness > light_lightness_cli[0].m_max_range) {
        leds.m_brightness = light_lightness_cli[0].m_max_range;
    } else if (leds.m_brightness < light_lightness_cli[0].m_min_range) {
        leds.m_brightness = light_lightness_cli[0].m_min_range;
    }

    u8_t leds_brightness   = 100 * (leds.m_brightness / MAX_BRIGHTNESS);
    u8_t number_of_leds_on = (leds_brightness / LED_BRIGHTNESS_PART);
    printk("Luminosidade da struct %d Percentual %d \n", leds.m_brightness, leds_brightness);

    for (int i = 0; i < number_of_leds_on; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_ON);
    }

    for (int i = number_of_leds_on; i < NUMBER_OF_LEDS; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_OFF);
    }

    return 0;
}

int leds_init_and_configure(leds_device_t *leds_device)
{
    leds_init(leds_device);
    leds_configure(leds_device);
    for (u8_t i = 0; i < NUMBER_OF_BLINKS; i++) {
        turn_all_leds_on(leds_device);
        k_sleep(SLEEP_TIME);
        turn_all_leds_off(leds_device);
        k_sleep(SLEEP_TIME);
    }

    return 0;
}
