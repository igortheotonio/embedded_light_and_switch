#include <misc/printk.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <device.h>
#include <gpio.h>

#define HIGH 1
#define LOW 0


//Configure LED GPIO communication port
#define LED_PORT	LED0_GPIO_CONTROLLER

//Configure LED GPIO PIN
#define LED0	LED0_GPIO_PIN
#define LED1	LED1_GPIO_PIN
#define LED2	LED2_GPIO_PIN
#define LED3	LED3_GPIO_PIN
#define LED4	LED4_GPIO_PIN

/* Sleep time */
#define SLEEP_TIME	1000

void main(void) {

  /*
   * ######## LEDS #########
   */

  int cnt = 0;
  struct device *dev;

  /* Initiate led device */
  dev = device_get_binding(LED_PORT);

  /* Configure leds */
  gpio_pin_configure(dev, LED0, GPIO_DIR_OUT |
	  GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH);

  gpio_pin_configure(dev, LED1, GPIO_DIR_OUT |
	  GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH);

  gpio_pin_configure(dev, LED2, GPIO_DIR_OUT |
	  GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH);

  gpio_pin_configure(dev, LED3, GPIO_DIR_OUT |
	  GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH);

  gpio_pin_configure(dev, LED4, GPIO_DIR_OUT |
	  GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH);

  while (1) {

	//Turn leds on (1) and off (0)
	gpio_pin_write(dev, LED0, cnt % 2);
	gpio_pin_write(dev, LED1, cnt % 2);
	gpio_pin_write(dev, LED2, cnt % 2);
	gpio_pin_write(dev, LED3, cnt % 2);
	gpio_pin_write(dev, LED4, cnt % 2);

	u32_t val[] = {0,0,0,0,0};

	gpio_pin_read(dev, LED0, &val[0]);
	gpio_pin_read(dev, LED1, &val[1]);
	gpio_pin_read(dev, LED2, &val[2]);
	gpio_pin_read(dev, LED3, &val[3]);
	gpio_pin_read(dev, LED4, &val[4]);

	printk("LEDS:");
	for ( int i = 0 ; i < 5 ; ++i ){
	  printk(" %d", val[i]);
	}
	printk("\n");
	cnt++;

	k_sleep(SLEEP_TIME);
  }
}
