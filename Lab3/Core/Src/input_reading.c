#include "main.h"
#include "timer.h"
#include "input_reading.h"

// We aim to work with more than one button
#define NO_OF_BUTTONS                 3

// Timer interrupt duration is 10ms, so to pass 1 second,
// we need to jump to the interrupt service routine 100 times
#define DURATION_FOR_AUTO_INCREASING  100

#define BUTTON_IS_PRESSED             GPIO_PIN_RESET
#define BUTTON_IS_RELEASED            GPIO_PIN_SET

// Buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};

// We define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};

// We define a flag for a button pressed more than 1 second
static uint8_t flagForButtonPress1s[NO_OF_BUTTONS];

// We define a counter that automatically increases the value
// after the button is pressed more than 1 second
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS];
static BUTTON_Pins[NO_OF_BUTTONS] = {BUTTON_0_Pin, BUTTON_1_Pin, BUTTON_2_Pin};
static uint16_t increasingCounterPeriod = 0;
uint8_t flagForIncreasingCounter = 0;
void button_reading(void)
{
    for (char i = 0; i < NO_OF_BUTTONS; i++)
    {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, BUTTON_Pins[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
        {
            buttonBuffer[i] = debounceButtonBuffer1[i];

            if (buttonBuffer[i] == BUTTON_IS_PRESSED)
            {
                // If a button is pressed, we start counting
                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING)
                {
                    counterForButtonPress1s[i]++;
                }
                else
                {
                    // The flag is turned on when 1 second has passed since the button is pressed
                    flagForButtonPress1s[i] = 1;
                    // TODO: handle long press here
                    increasingCounterPeriod += TIMER_PERIOD_MS;
                    if (increasingCounterPeriod >= 500)
                    {
                    	increasingCounterPeriod = 0;
                    	flagForIncreasingCounter = 1;
                    }



                }
            }
            else
            {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(uint8_t index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0xff;
    return (flagForButtonPress1s[index] == 1);
}

