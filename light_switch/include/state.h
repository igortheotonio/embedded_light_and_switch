#ifndef _STATE_H
#define _STATE_H

#include <logging/log.h>
#include <misc/printk.h>
#include <zephyr.h>

typedef enum { S0, S1, S2, S3, S4, S5, number_of_states } state_e;
typedef enum { timeout, zero, encoder_right, encoder_left, button_pressed, number_of_events } event_e;

static char *event_string[] = {"Timeout", "Zero", "Encoder_Right", "Encoder_Left", "Button_Pressed"};

typedef void (*action_function)();
typedef state_e (*next_state_function)(event_e);

typedef struct {
    char * m_name;
    uint8_t m_switch_state;
    uint8_t m_light_state;
    uint16_t m_pulse_width;
    uint16_t m_leds;

    action_function action;
    next_state_function next_state;
} state;

state_e S0_next_state(event_e event);
state_e S1_next_state(event_e event);
state_e S2_next_state(event_e event);
state_e S3_next_state(event_e event);
state_e S4_next_state(event_e event);
state_e S5_next_state(event_e event);

void S0_action();
void S1_action();
void S2_action();
void S3_action();
void S4_action();
void S5_action();

#endif  // _STATE_H