#ifndef _STATE_H
#define _STATE_H

#include <misc/util.h>
#include <zephyr/types.h>

typedef enum { S0, S1, S2, S3, S4, S5, number_of_states } state_e;
typedef enum { timeout, encoder_right, encoder_left, button_pressed, number_of_events } event_e;

typedef void (*action_function)();

typedef struct {
    uint8_t m_switch_state;
    uint8_t m_light_state;
    uint16_t m_pulse_width;
    uint16_t m_leds;
    action_function action;
} state;

// state S0 = {0, 0, 0, 0};
// state S1 = {1, 0, 0, 0};
// state S2 = {1, 1, 0, 0};
// state S3 = {0, 1, 0, 0};
// state S4 = {1, 0, 0, 0};
// state S5 = {0, 0, 0, 0};

// state states[number_of_states];

void action(state* state);

#endif  // _STATE_H