#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <zephyr/types.h>

#include "defines.h"

// Encoder device struct
typedef struct {
    struct device *m_device;
    struct gpio_callback m_device_cb;
    u8_t initiated;
} encoder_device_t;

static u32_t encoder_pins[]          = {BUTTON, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B};
static const u8_t encoder_diretion[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

int encoder_init_device(encoder_device_t *encoder_device);

int encoder_configure(encoder_device_t *encoder_device);

void encoder_init_and_configure(encoder_device_t *encoder_device);

void encoder_configure_callback(encoder_device_t *encoder_device);

void encoder_callback(struct device *encoder_device, struct gpio_callback *callback,
                      u32_t encoder_pin_mask);

#endif  // _ENCODER_H_
