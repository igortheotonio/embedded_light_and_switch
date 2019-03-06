/**
 * @file transition.h
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#ifndef _TRANSITION_H_
#define _TRANSITION_H_

struct transition {
    bool just_started;
    u8_t tt;
    u8_t rt;
    u8_t delay;
    u32_t quo_tt;
    u32_t counter;
    u32_t total_duration;
    s64_t start_timestamp;

    struct k_timer timer;
};

#endif

