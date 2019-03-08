

/* change this to use another GPIO port */
#ifndef SW0_GPIO_CONTROLLER
#ifdef SW0_GPIO_NAME
#define SW0_GPIO_CONTROLLER SW0_GPIO_NAME
#else
#error SW0_GPIO_NAME or SW0_GPIO_CONTROLLER needs to be set in board.h
#endif
#endif
#define BUTTON_CONTROLLER	SW0_GPIO_CONTROLLER

/* change this to use another GPIO pin */
#ifdef SW0_GPIO_PIN
#define BUTTON0     SW0_GPIO_PIN
#define BUTTON1     SW1_GPIO_PIN
#define BUTTON2     SW2_GPIO_PIN
#define BUTTON3     SW3_GPIO_PIN
#else
#error SW0_GPIO_PIN needs to be set in board.h
#endif

/* change to use another GPIO pin interrupt config */
#ifdef SW0_GPIO_FLAGS
#define EDGE    (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#else
/*
 * If SW0_GPIO_FLAGS not defined used default EDGE value.
 * Change this to use a different interrupt trigger
 */
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

/* change this to enable pull-up/pull-down */
#ifndef SW0_GPIO_FLAGS
#ifdef SW0_GPIO_PIN_PUD
#define SW0_GPIO_FLAGS SW0_GPIO_PIN_PUD
#else
#define SW0_GPIO_FLAGS 0
#endif
#endif
#define PULL_UP SW0_GPIO_FLAGS

#define STANDARD_BUTTON GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE
#define STANDARD_LED GPIO_DIR_OUT | GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH

/* Sleep time */
#define SLEEP_TIME	500

#define LED_PORT LED0_GPIO_CONTROLLER
#define LED0 LED0_GPIO_PIN
#define LED1 LED1_GPIO_PIN
#define LED2 LED2_GPIO_PIN
#define LED3 LED3_GPIO_PIN
#define LED4 LED4_GPIO_PIN

#define PERIOD_HARDWARE (USEC_PER_SEC / 50)
#define PERIOD_SOFTWARE 16
#define FADESTEP_HARDWARE	2000
#define FADESTEP_SOFTWARE	2

#if defined(CONFIG_PWM_NRF5_SW)
#define PWM_DRIVER CONFIG_PWM_NRF5_SW_0_DEV_NAME
#else
#define PWM_DRIVER CONFIG_PWM_0_NAME
#endif  /* CONFIG_PWM_NRF5_SW */
#define PWM_CHANNEL0 LED0_GPIO_PIN
#define PWM_CHANNEL1 LED1_GPIO_PIN
#define PWM_CHANNEL2 LED2_GPIO_PIN
#define PWM_CHANNEL3 LED3_GPIO_PIN
#define PWM_CHANNEL4 LED4_GPIO_PIN