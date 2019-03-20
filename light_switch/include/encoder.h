#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <device.h>
#include <gpio.h>
#include <logging/log.h>
#include <misc/util.h>
#include <zephyr/types.h>

#include "defines.h"
#include "leds.h"
#include "node_composition.h"

static const u8_t encoder_diretion[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

// Encoder device struct
typedef struct {
    struct device *m_device;
    struct gpio_callback m_device_cb;
    u8_t m_initiated;
    u8_t m_state;
    s8_t m_position;
} encoder_device_t;

extern encoder_device_t encoder;

int encoder_init(encoder_device_t *encoder_device);

int encoder_configure(encoder_device_t *encoder_device);

int encoder_init_and_configure(encoder_device_t *encoder_device);

void encoder_configure_callback(encoder_device_t *encoder_device);

#endif  // _ENCODER_H_
