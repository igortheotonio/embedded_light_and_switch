#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <device.h>
#include <gpio.h>
#include <pwm.h>
#include <zephyr/types.h>

#define PWM_DRIVER DT_NORDIC_NRF_PWM_PWM_0_LABEL

#define PWM_CHANNEL0 LED0_GPIO_PIN

#define PERIOD 20  // 50kHz
#define FADESTEP 2

typedef struct {
    u8_t pin;
    u16_t pulse_width;
    u16_t period;
    u8_t initiated;
    struct device *dev;

} pwm_driver_t;

extern pwm_driver_t pwm;

int init_pwm_driver(pwm_driver_t *pwm, const char *pwm_label, u8_t pin, u16_t period);
int change_pulse_width(pwm_driver_t *pwm, u16_t pulse_width);
int set_pulse_width(pwm_driver_t *pwm);
void blink_light(pwm_driver_t *pwm_driver, u32_t number);

#endif
