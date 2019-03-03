#include "state.h"

state_e S0_next_state(event_e event)
{
    static state_e next_state[] = {S1, S0, S0, S0, S0};
    return next_state[event];
}

state_e S1_next_state(event_e event)
{
    static state_e next_state[] = {S2, S1, S1, S1, S1};
    return next_state[event];
}

state_e S2_next_state(event_e event)
{
    static state_e next_state[] = {S3, S2, S2, S2, S2};
    return next_state[event];
}

state_e S3_next_state(event_e event)
{
    static state_e next_state[] = {S4, S3, S3, S3, S3};
    return next_state[event];
}

state_e S4_next_state(event_e event)
{
    static state_e next_state[] = {S5, S4, S4, S4, S4};
    return next_state[event];
}

state_e S5_next_state(event_e event)
{
    static state_e next_state[] = {S0, S5, S5, S5, S5};
    return next_state[event];
}

void S0_action()
{
}

void S1_action()
{
}

void S2_action()
{
}

void S3_action()
{
}

void S4_action()
{
}

void S5_action()
{
}