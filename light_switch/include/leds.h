#ifndef _IC_LEDS_H_
#define _IC_LEDS_H_

#include <device.h>
#include <gpio.h>
#include <logging/log.h>
#include <pwm.h>
#include <zephyr/types.h>

#include "defines.h"

// LOG_MODULE_REGISTER(LEDS);

typedef struct {
    struct device *m_device;
    u16_t m_brightness;
    u8_t m_initiated;
    u8_t m_state;
} leds_device_t;

int leds_init(leds_device_t *leds_device);

int leds_brightness(leds_device_t *leds_device, u16_t brightness);

int leds_change_state(leds_device_t *leds_device);

int leds_set_brightness(leds_device_t *leds_device, u16_t brightness);

#endif  // _IC_LEDS_H_