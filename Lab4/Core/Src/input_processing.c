#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "task.h"
#include <stdio.h>
//#include "fsm_traffic.h"
uint8_t mode = 1;



enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};  // Per-button

void fsm_for_input_processing(void) {
    for (uint8_t i = 0; i < 1; i++) {  // Loop xử lý từng button riêng
        switch (buttonState[i])
        {
        case BUTTON_RELEASED:
            if (is_button_pressed(i))
            {
                buttonState[i] = BUTTON_PRESSED;
                seg_counter++;

                // Xử lý short press cho từng button
//                if (i == 0)
//                {  // Button 0: Tăng mode
//                    mode++;
//
//                }
//                else if (i == 1)
//                {  // Button 1: Tăng timer_temp
//                	switch (mode)
//                	{
//                	    case 2: // RED
//                	        if (red_timer_temp0 < 99000)
//                	        {
//                	            red_timer_temp0 += 1000;
//                	        } else
//                	        {
//                	            red_timer_temp0 = 1000;
//                	            green_timer_temp0 = 1000;
//                	            yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//                	        }
//                	        break;
//
//                	    case 3: // YELLOW
//
//                	        if (yellow_timer_temp0 < 99000) {
//                	            yellow_timer_temp0 += 1000;
//
//                	        } else {
//                	            yellow_timer_temp0 = 1000;
//                	            green_timer_temp0 = red_timer_temp0 - yellow_timer_temp0;
//                	        }
//
//                	        break;
//
//                	    case 4: // GREEN
//                	        if (green_timer_temp0 < 99000) {
//                	            green_timer_temp0 += 1000;
//                	        } else {
//                	            green_timer_temp0 = 1000;
//                	            yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//                	        }
//                	        break;
//                	}
//                }
//                else if (i == 2)
//                {  // Button 2: Apply temp và reset mode
//                    mode = 1;
//                    red_timer0 += red_timer_temp0 - red_timer0;
//                    green_timer0 += green_timer_temp0 - green_timer0;
//                    yellow_timer0 += yellow_timer_temp0 - yellow_timer0;
//                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(i))
            {
                buttonState[i] = BUTTON_RELEASED;
            }
            else
            {
                if (is_button_pressed_1s(i))
                {
                    buttonState[i] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(i)) {
                buttonState[i] = BUTTON_RELEASED;
            }
            else
            {
                // Xử lý long press auto-increase per button
                if (flagForIncreasingCounter[i] == 1)
                {
                	seg_counter++;
                	flagForIncreasingCounter[i] = 0;
//                	if (i == 0)
//					{  // Button 0: Tăng mode
//						mode++;
//						if (mode == 2)
//						{
//							red_timer_temp0 = red_timer0;
//						}
//						else
//						if (mode == 3)
//						{
//							yellow_timer_temp0 = yellow_timer0;
//						}
//						else
//						if (mode == 4)
//						{
//							green_timer_temp0 = green_timer0;
//						}
//					}
//                	else if (i == 1)
//					{  // Button 1: Tăng timer_temp
//                		switch (mode)
//						{
//							case 2: // RED
//								if (red_timer_temp0 < 99000)
//								{
//									red_timer_temp0 += 1000;
//									green_timer_temp0 += 1000;
//								} else
//								{
//									red_timer_temp0 = 1000;
//									green_timer_temp0 = 1000;
//									yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//								}
//								// yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//								break;
//
//							case 3: // YELLOW
//
//								if (yellow_timer_temp0 < 99000) {
//									yellow_timer_temp0 += 1000;
//									red_timer_temp0 += 1000;
//								} else {
//									yellow_timer_temp0 = 1000;
//									green_timer_temp0 = red_timer_temp0 - yellow_timer_temp0;
//									//red_timer_temp0 = yellow_timer_temp0 + green_timer_temp0;
//								}
//								//
//								break;
//
//							case 4: // GREEN
//								if (green_timer_temp0 < 99000) {
//									green_timer_temp0 += 1000;
//									red_timer_temp0 += 1000;
//								} else {
//									green_timer_temp0 = 1000;
//									yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//									// red_timer_temp0 = yellow_timer_temp0 + green_timer_temp0;
//								}
//								//yellow_timer_temp0 = red_timer_temp0 - green_timer_temp0;
//								break;
//						}
//					}

                }
            }
            break;
        }
    }
    if (mode == 5)
    {
        mode = 1;
    }
    if (seg_counter > 9)
    {
    	seg_counter = 0;
    }
}
