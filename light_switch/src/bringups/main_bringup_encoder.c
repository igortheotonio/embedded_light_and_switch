#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <zephyr.h>

/* Sleep time */
#define SLEEP_TIME 250

// Button device instance DO NOT DELETE IT

#ifdef SW0_GPIO_FLAGS
#define EDGE (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#else

#define EDGE (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

#ifdef SW0_GPIO_PIN_PUD
#define PULL_UP SW0_GPIO_PIN_PUD
#else
#define PULL_UP 0
#endif

// Buttons GPIO communication PORT
#define BUTTONS_PORT SW0_GPIO_CONTROLLER

// Defining buttons pins
#define BUTTON0 SW0_GPIO_PIN
#define BUTTON1 SW1_GPIO_PIN
#define BUTTON2 SW2_GPIO_PIN
#define BUTTON3 SW3_GPIO_PIN

void ic_buttons_callback(struct device *buttons_device, struct gpio_callback *callback,
                         u32_t button_pin_mask);

void main(void)
{
    const u8_t KNOBDIR[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    struct device *bdev;
    s8_t old_state = 3;
    s8_t position  = 0;
    u32_t a_state  = 0;
    u32_t b_state  = 0;
    printk("Press the user defined button on the board\n");
    bdev = device_get_binding(BUTTONS_PORT);
    if (!bdev) {
        printk("error\n");
        return;
    }

    // Configure buttons

    gpio_pin_configure(bdev, BUTTON1, GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);

    gpio_pin_configure(bdev, BUTTON2, GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);


    while (1) {
        gpio_pin_read(bdev, BUTTON1, &a_state);
        gpio_pin_read(bdev, BUTTON2, &b_state);
        u8_t this_state = a_state | (b_state << 1);
        if (old_state != this_state) {
            position += KNOBDIR[this_state | (old_state << 2)];
        }
        printk("%d \n", position);
        old_state = this_state;
    }
}