#include "pwm_driver.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(PWM_LED, 4);

int init_pwm_driver(pwm_driver_t *pwm_driver, const char *pwm_label, u8_t pin, u16_t period)
{
    if (pwm_driver->initiated) {
        LOG_ERR("pwm_driver is not null");
        return -EBUSY;
    }

    struct device *dev = device_get_binding(pwm_label);
    if (dev == NULL) {
        LOG_ERR("Error getting the device.");
        return -EBUSY;
    }

    pwm_driver->dev         = dev;
    pwm_driver->pin         = pin;
    pwm_driver->period      = period;
    pwm_driver->initiated   = 1;
    pwm_driver->pulse_width = 0;
    LOG_WRN("Pwm driver initiated with pin %d", pin);
    return 0;
}


int change_pulse_width(pwm_driver_t *pwm_driver, u16_t pulse_width)
{
    if (pwm_driver->initiated == 0) {
        LOG_ERR("pwm driver was not initiated.");
        return -ENODEV;
    }

    if (pulse_width > pwm_driver->period || pulse_width < 0) {
        LOG_ERR("The pulse width is going over the edge with %d/%d.", pulse_width,
                pwm_driver->period);
        return -EINVAL;
    }

    if (pwm_driver->pulse_width != pulse_width) {
        pwm_driver->pulse_width = pulse_width;
        LOG_DBG("Change pulseWidth to: %d", pulse_width);
    }

    return 0;
}

int set_pulse_width(pwm_driver_t *pwm_driver)
{
    if (pwm_driver->initiated == 0) {
        LOG_ERR("pwm driver was not initiated.");
        return -ENODEV;
    }
    /*LOG_DBG("<Pin %d> - Setting pulseWidth to: %d", pwm_driver->pin, pwm_driver->pulse_width);*/
    int err = pwm_pin_set_usec(pwm_driver->dev, pwm_driver->pin, pwm_driver->period,
                               pwm_driver->pulse_width);
    if (err) {
        LOG_ERR("PWM set failed.\n");
        return err;
    }
    return 0;
}

void blink_light(pwm_driver_t *pwm_driver, u32_t number)
{
    if (pwm_driver->pulse_width) {
        for (int i = 0; i < number; i++) {
            pwm_pin_set_usec(pwm_driver->dev, pwm_driver->pin, pwm_driver->period, 0);
            k_sleep(500);
            pwm_pin_set_usec(pwm_driver->dev, pwm_driver->pin, pwm_driver->period,
                             pwm_driver->period);
            k_sleep(500);
        }
    } else {
        for (int i = 0; i < number; i++) {
            pwm_pin_set_usec(pwm_driver->dev, pwm_driver->pin, pwm_driver->period,
                             pwm_driver->period);
            k_sleep(500);
            pwm_pin_set_usec(pwm_driver->dev, pwm_driver->pin, pwm_driver->period, 0);
            k_sleep(500);
        }
    }
}
