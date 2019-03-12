/*
    # Defines of inputs
*/

// Inputs
#define BUTTON_CONTROLLER SW0_GPIO_CONTROLLER
#define BUTTON SW0_GPIO_PIN
#define ENCODER_CHANNEL_A SW1_GPIO_PIN
#define ENCODER_CHANNEL_B SW2_GPIO_PIN
#define NUMBER_OF_INPUTS 3

// Flags
#define EDGE (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#define PULL_UP SW0_GPIO_FLAGS
#define STANDARD_BUTTON GPIO_DIR_IN | GPIO_INT | PULL_UP | EDGE

/*
    # Defines of outputs
*/

// Outputs
#define LED_PORT LED0_GPIO_CONTROLLER
#define LED0 LED0_GPIO_PIN
#define LED1 LED1_GPIO_PIN
#define LED2 LED2_GPIO_PIN
#define LED3 LED3_GPIO_PIN
#define LED4 LED4_GPIO_PIN

// Flags
#define MAX_PWM_PERIOD (USEC_PER_SEC / 50)
#define MIN_PWM_PERIOD 0

/*
    # Defines of leds
*/

#define STANDARD_LED GPIO_DIR_OUT | GPIO_PUD_PULL_UP | GPIO_INT_ACTIVE_HIGH
#define NUMBER_OF_BLINKS 5
#define MAX_BRIGHTNESS 65535.0
#define MIN_BRIGHTNESS 0.0
#define NUMBER_OF_LEDS 5
#define LED_BRIGHTNESS_PART (100 / NUMBER_OF_LEDS)
#define LED_ON 0
#define LED_OFF 1
#define ON 1
#define OFF 0

/*
    # Defines of encoder
*/

#define INITIAL_STATE 3
#define INITIAL_POSITION 0
#define SIZE_ONE_STATE 1
#define SIZE_TWO_STATE 2
/*
    # Defines of timers
*/

#define SLEEP_TIME 500
#define BUTTON_DEBOUNCE_DELAY 250