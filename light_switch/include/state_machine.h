#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include <logging/log.h>
#include <misc/printk.h>
#include <zephyr.h>
#include "state.h"

typedef struct {
} state_machine;

int transition_machine(event_e event);

#endif  // _STATE_MACHINE_H