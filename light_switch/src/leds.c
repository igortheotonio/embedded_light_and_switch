#include "leds.h"

int leds_init(leds_device_t *leds_device)
{
    if (!leds_device) {
        return -ENODEV;
    }
    leds_device->m_device = device_get_binding(PWM_DRIVER);
    if (!leds_device->m_device) {
        return -EPERM;
    }
    leds_device->initiated = 1;

    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        leds_brightness(leds_device, MAX_BRIGHTNESS);
        k_sleep(SLEEP_TIME);
        leds_brightness(leds_device, MIN_BRIGHTNESS);
        k_sleep(SLEEP_TIME);
    }

    return 0;
}

int leds_brightness(leds_device_t *leds_device, u16_t brightness)
{
    if (!leds_device->initiated) {
        return -EPERM;
    }
    if (brightness > MAX_BRIGHTNESS) {
        return -EINVAL;
    }
    u8_t leds_brightness   = 100 * (brightness / MAX_BRIGHTNESS);
    u8_t number_of_leds_on = (leds_brightness / LED_BRIGHTNESS_PART);
    for (int i = 0; i < number_of_leds_on; i++) {
        pwm_pin_set_usec(leds_device->m_device, leds_pins[i], MAX_PWM_PERIOD, MIN_PWM_PERIOD);
    }


    if (number_of_leds_on != NUMBER_OF_LEDS && (leds_brightness % LED_BRIGHTNESS_PART)) {
        pwm_pin_set_usec(leds_device->m_device, leds_pins[number_of_leds_on], MAX_PWM_PERIOD,
                         (MAX_PWM_PERIOD - ((leds_brightness / 100.0) * MAX_PWM_PERIOD)));
    }

    for (int i = number_of_leds_on + 1; i < NUMBER_OF_LEDS; i++) {
        pwm_pin_set_usec(leds_device->m_device, leds_pins[i], MAX_PWM_PERIOD, MAX_PWM_PERIOD);
    }

    return 0;
}