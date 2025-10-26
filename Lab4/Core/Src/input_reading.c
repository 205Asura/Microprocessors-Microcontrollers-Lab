#include "main.h"
#include "timer.h"
#include "input_reading.h"

// We aim to work with more than one button
#define NO_OF_BUTTONS                 1
#define DURATION_FOR_AUTO_INCREASING  100 // ms
#define BUTTON_IS_PRESSED             GPIO_PIN_RESET
#define BUTTON_IS_RELEASED            GPIO_PIN_SET

// Buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};

// We define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED};

// We define a flag for a button pressed more than 1 second
static uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = {0};

// We define a counter that automatically increases the value after the button is pressed more than 1 second
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0};

static uint16_t BUTTON_Pins[NO_OF_BUTTONS] = {BUTTON_0_Pin};  // Thêm type uint16_t nếu thiếu

uint8_t flagForIncreasingCounter[NO_OF_BUTTONS] = {0};  // Thêm volatile và array

static uint16_t increasingCounterPeriod[NO_OF_BUTTONS] = {50};  // Per-button để tránh conflict

void button_reading(void)
{
    for (uint8_t i = 0; i < NO_OF_BUTTONS; i++)
    {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, BUTTON_Pins[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
        {
            buttonBuffer[i] = debounceButtonBuffer1[i];

            if (buttonBuffer[i] == BUTTON_IS_PRESSED)
            {
                // If a button is pressed, we start counting
                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING)
                {
                    counterForButtonPress1s[i] += TIMER_PERIOD_MS;
                    increasingCounterPeriod[i] = 50;
                }
                else
                {
                    // The flag is turned on when 1 second has passed since the button is pressed
                    flagForButtonPress1s[i] = 1;
                    // Handle long press auto-increase per button
                    increasingCounterPeriod[i] += TIMER_PERIOD_MS;
                    printf("increasingCounterPeriod = %d\r\n", increasingCounterPeriod[i]);
                    if (increasingCounterPeriod[i] >= 50)
                    {  // Auto-increase every 500ms after 1s
                        increasingCounterPeriod[i] = 0;
                        flagForIncreasingCounter[i] = 1;
                    }
                }
            }
            else
            {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
//                increasingCounterPeriod[i] = 0;  // Reset per button
                flagForIncreasingCounter[i] = 0;
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
