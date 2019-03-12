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

int leds_change_state(leds_device_t *leds_device)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }
    if (leds_device->m_state) {
        leds_brightness(leds_device, MIN_BRIGHTNESS);
    } else {
        if (!leds_device->m_brightness) {
            leds_set_brightness(leds_device, MAX_BRIGHTNESS);
        }
        leds_brightness(leds_device, leds_device->m_brightness);
    }


    return 0;
}

int leds_set_brightness(leds_device_t *leds_device, u16_t brightness)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }

    leds_device->m_brightness = brightness;
    return 0;
}


int leds_brightness(leds_device_t *leds_device, u16_t brightness)
{
    if (!leds_device->m_initiated) {
        return -EPERM;
    }
    if (brightness > MAX_BRIGHTNESS || brightness < MIN_BRIGHTNESS) {
        return -EINVAL;
    }

    u8_t leds_brightness   = 100 * (brightness / MAX_BRIGHTNESS);
    u8_t number_of_leds_on = (leds_brightness / LED_BRIGHTNESS_PART);

    for (int i = 0; i < number_of_leds_on; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_ON);
    }

    for (int i = number_of_leds_on; i < NUMBER_OF_LEDS; i++) {
        gpio_pin_write(leds_device->m_device, leds_pins[i], LED_OFF);
    }

    if (brightness == 0) {
        leds_device->m_state = OFF;
    } else {
        leds_device->m_state = ON;
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
