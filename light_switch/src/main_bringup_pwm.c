#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <pwm.h>
#include "defines.h"

u32_t leds_pin[] = {LED0, LED1, LED2, LED3};
u32_t pwms_pin[] = {PWM_CHANNEL0, PWM_CHANNEL1, PWM_CHANNEL2, PWM_CHANNEL3, PWM_CHANNEL4};

u32_t pwm_pulse_hardware = 1;
u32_t pwm_pulse_software = 1;

struct device * leds_dev;
struct device * pwm_dev;

void configure_leds(){
  // Configuration leds
  // PWM HARDWARE
  u32_t pulse_width = 0U;
  if (!pwm_dev) {
	printk("Cannot find %s!\n", PWM_DRIVER);
	return;
  }
}

void main(void)
{
  leds_dev = device_get_binding(LED_PORT);
  pwm_dev = device_get_binding(PWM_DRIVER);
  configure_leds();
  int percentage = 90;
  for (int i = 0; i < int(percentage/(100/5)); i++) {
	pwm_pin_set_usec(pwm_dev, pwms_pin[i], PERIOD_HARDWARE, 0);
  }

  if(percentage != 100) {
	pwm_pin_set_usec(pwm_dev, pwms_pin[int(percentage/(100/5))+1], PERIOD_HARDWARE, PERIOD_HARDWARE/2);
  }
  printk("PWM HARDWARE: %d\n", pwm_pulse_hardware);
  while (1) {
  }
}
