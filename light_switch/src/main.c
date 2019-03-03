#include <logging/log.h>
#include <misc/printk.h>
#include <zephyr.h>

#include "state_machine.h"

int main(void)
{
    while (1) {
        printk("Run...: ");
        transition_machine(timeout);
        k_sleep(K_SECONDS(1));
    }
    return 0;
}