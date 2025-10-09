#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "fsm_traffic.h"

volatile uint8_t mode = 1;






enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState = BUTTON_RELEASED;

void fsm_for_input_processing(void){
    switch (buttonState)
    {
    case BUTTON_RELEASED:
        if (is_button_pressed(0)) {
            buttonState = BUTTON_PRESSED;
            // INCREASE VALUE OF PORT A BY ONE UNIT
            mode++;
            isButton0Pressed = 1;
        }
        else
        if (is_button_pressed(1))
        {
        	if (red_timer < 99000)
        	switch (mode)
        	{
				case 2:
				green_timer_temp++;
				red_timer_temp++;
				break;

				case 3:
				yellow_timer_temp++;
				red_timer_temp++;
				break;

				case 4:
				green_timer_temp++;
				red_timer_temp++;
				break;
        	}

        }
        else
        if (is_button_pressed(2))
        {
        	mode = 1;
        	red_timer = red_timer_temp;
        	green_timer = green_timer_temp;
        	yellow_timer = yellow_timer_temp;
        }

        break;

    case BUTTON_PRESSED:
        if (!is_button_pressed(0)) {
            buttonState = BUTTON_RELEASED;
        } else {
            if (is_button_pressed_1s(0)) {
                buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
        }
        break;

    case BUTTON_PRESSED_MORE_THAN_1_SECOND:
        if (!is_button_pressed(0)) {
            buttonState = BUTTON_RELEASED;
        }
        // todo
        if (flagForIncreasingCounter == 1)
        {
        	mode++;
        	flagForIncreasingCounter = 0;
        }

        break;
    }
    if (mode == 5)
    {
    	mode = 1;
    }
}
