#include "leds.h"

static const u32_t leds_pins[] = {PWM_CHANNEL0, PWM_CHANNEL1, PWM_CHANNEL2, PWM_CHANNEL3,
                                  PWM_CHANNEL4};

int leds_init(leds_device_t *leds_device)
{
    if (!leds_device) {
        return -ENODEV;
    }
    leds_device->m_device = device_get_binding(PWM_DRIVER);
    if (!leds_device->m_device) {
        return -EPERM;
    }
    leds_device->m_initiated = 1;
    leds_device->m_state     = OFF;
    for (int i = 0; i < NUMBER_OF_BLINK; i++) {
        leds_brightness(leds_device, (i & 1) ? MIN_BRIGHTNESS : MAX_BRIGHTNESS);
        k_sleep(SLEEP_TIME);
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

    leds_device->m_state = !leds_device->m_state;

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
        pwm_pin_set_usec(leds_device->m_device, leds_pins[i], MAX_PWM_PERIOD, MIN_PWM_PERIOD);
    }

    for (int i = number_of_leds_on; i < NUMBER_OF_LEDS; i++) {
        pwm_pin_set_usec(leds_device->m_device, leds_pins[i], MAX_PWM_PERIOD, MAX_PWM_PERIOD);
    }

    if (number_of_leds_on != NUMBER_OF_LEDS && (leds_brightness % LED_BRIGHTNESS_PART)) {
        pwm_pin_set_usec(leds_device->m_device, leds_pins[number_of_leds_on], MAX_PWM_PERIOD,
                         (MAX_PWM_PERIOD - ((leds_brightness / 100.0) * MAX_PWM_PERIOD)));
    }

    return 0;
}