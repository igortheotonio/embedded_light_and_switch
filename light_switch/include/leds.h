#ifndef _IC_LEDS_H_
#define _IC_LEDS_H_

#include <device.h>
#include <gpio.h>
#include <logging/log.h>
#include <pwm.h>
#include <zephyr/types.h>

#include "defines.h"
#include "node_composition.h"

typedef struct {
    struct device *m_device;
    u16_t m_brightness;
    u8_t m_initiated;
    u8_t m_state;
} leds_device_t;

extern leds_device_t leds;

int leds_init(leds_device_t *leds_device);

int leds_configure(leds_device_t *leds_device);

int turn_all_leds_on(leds_device_t *leds_device);

int turn_all_leds_off(leds_device_t *leds_device);

int leds_brightness(leds_device_t *leds_device);

int leds_init_and_configure(leds_device_t *leds_device);

#endif  // _IC_LEDS_H_