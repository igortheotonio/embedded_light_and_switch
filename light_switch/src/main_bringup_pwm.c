#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <pwm.h>
#include <zephyr.h>

#include "defines.h"

u32_t leds_pin[] = {LED0, LED1, LED2, LED3};
u32_t pwms_pin[] = {PWM_CHANNEL0, PWM_CHANNEL1, PWM_CHANNEL2, PWM_CHANNEL3, PWM_CHANNEL4};

u32_t pwm_pulse_hardware = 1;
u32_t pwm_pulse_software = 1;

struct device* leds_dev;
struct device* pwm_dev;

void configure_leds()
{
  // Configuration leds
  // PWM HARDWARE
  u32_t pulse_width = 0U;
  if (!pwm_dev) {
	printk("Cannot find %s!\n", PWM_DRIVER);
	return;
  }
}

void set_leds(u16_t light_bulb_brightness)
{
  u8_t leds_brightness   = 100 * (light_bulb_brightness / 65535.0);
  u8_t number_of_leds_on = (leds_brightness / 20);
  printk("%d %d\n", leds_brightness, number_of_leds_on);
  for (int i = 0; i < number_of_leds_on; i++) {
	pwm_pin_set_usec(pwm_dev, pwms_pin[i], PERIOD_HARDWARE, 0);
  }


  if (number_of_leds_on != 5 && (leds_brightness % 20)) {
	pwm_pin_set_usec(pwm_dev, pwms_pin[number_of_leds_on], PERIOD_HARDWARE,
					 (PERIOD_HARDWARE - ((leds_brightness / 100.0) * PERIOD_HARDWARE)));
  }

  for (int i = number_of_leds_on + 1; i < 5; i++) {
	pwm_pin_set_usec(pwm_dev, pwms_pin[i], PERIOD_HARDWARE, PERIOD_HARDWARE);
  }
}

void main(void)
{
  leds_dev = device_get_binding(LED_PORT);
  pwm_dev  = device_get_binding(PWM_DRIVER);
  configure_leds();
  while (1) {
	for (u16_t i; i <= 65; i++) {
	  set_leds(i * 1000);
	  printk("%d \n", i * 1000);
	  k_sleep(100);
	}
  }
}
