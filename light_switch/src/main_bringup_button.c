#include <misc/printk.h>
#include <zephyr.h>
#include <misc/util.h>
#include <device.h>
#include <gpio.h>

/* Sleep time */
#define SLEEP_TIME    250

//Button device instance DO NOT DELETE IT

#ifdef SW0_GPIO_FLAGS
#define EDGE    (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#else

#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

#ifdef SW0_GPIO_PIN_PUD
#define PULL_UP SW0_GPIO_PIN_PUD
#else
#define PULL_UP 0
#endif

//Buttons GPIO communication PORT
#define BUTTONS_PORT SW0_GPIO_CONTROLLER

//Defining buttons pins
#define BUTTON0 SW0_GPIO_PIN
#define BUTTON1 SW1_GPIO_PIN
#define BUTTON2 SW2_GPIO_PIN
#define BUTTON3 SW3_GPIO_PIN

void ic_buttons_callback(struct device *buttons_device,
						 struct gpio_callback *callback,
						 u32_t button_pin_mask);

void main(void) {

  /*
   * ######## BUTTONS #########
   */

  //Initiating device
  struct device *bdev;
  struct gpio_callback gpio_cb;
  printk("Press the user defined button on the board\n");
  bdev = device_get_binding(BUTTONS_PORT);
  if (! bdev) {
	printk("error\n");
	return;
  }

  //Configure buttons
  gpio_pin_configure(bdev, BUTTON0,
					 GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);

  gpio_pin_configure(bdev, BUTTON1,
					 GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);

  gpio_pin_configure(bdev, BUTTON2,
					 GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE);

  while (1) {

	u32_t val0 = 0;
	u32_t val1 = 0;
	u32_t val2 = 0;
	gpio_pin_read(bdev, BUTTON0, &val0);
	gpio_pin_read(bdev, BUTTON1, &val1);
	gpio_pin_read(bdev, BUTTON2, &val2);
	printk("Botões : %d %d %d\n", val0, val1, val2);
	k_sleep(SLEEP_TIME);
  }
}