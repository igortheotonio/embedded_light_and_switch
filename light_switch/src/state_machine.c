#include "state_machine.h"

int transition_machine(event_e event)
{
    static state m_states[] = {
        {"S0", 0, 0, 0, 0, S0_action, S0_next_state}, {"S1", 1, 0, 0, 0, S1_action, S1_next_state},
        {"S2", 1, 1, 0, 0, S2_action, S2_next_state}, {"S3", 0, 1, 0, 0, S3_action, S3_next_state},
        {"S4", 1, 0, 0, 0, S4_action, S4_next_state}, {"S5", 1, 0, 0, 0, S5_action, S5_next_state}};
    static state_e m_current_state = S0;

    if (event < number_of_events) {
        printk("Event %s happened\n", event_string[event]);
        state_e next_state = m_states[m_current_state].next_state(event);
        printk("Next state %s\n", m_states[next_state].m_name);

        m_states[next_state].action();
        m_current_state = next_state;
        return 0;
    }

    return -1;
}