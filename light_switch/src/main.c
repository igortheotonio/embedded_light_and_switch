#include <logging/log.h>
#include <misc/printk.h>
#include <zephyr.h>

int main(void)
{
    while (1) {
        printk("Run...: ");
        k_sleep(K_SECONDS(1));
    }
    return 0;
}