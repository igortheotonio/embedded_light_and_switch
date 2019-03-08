#ifndef _IC_LEDS_H_
#define _IC_LEDS_H_

#include <device.h>
#include <gpio.h>
#include <logging/sys_log.h>
#include <pwm.h>
#include <zephyr/types.h>

#include "defines.h"

static u32_t leds_pins[] = {PWM_CHANNEL0, PWM_CHANNEL1, PWM_CHANNEL2, PWM_CHANNEL3, PWM_CHANNEL4};

typedef struct {
    struct device *m_device;
    u8_t initiated;
} leds_device_t;

int leds_init(leds_device_t *leds_device);

int leds_brightness(leds_device_t *leds_device, u16_t brightness);

#endif  // _IC_LEDS_H_