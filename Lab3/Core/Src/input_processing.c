#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "fsm_traffic.h"

volatile uint8_t mode = 1;


enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};  // Per-button

void fsm_for_input_processing(void) {
    for (uint8_t i = 0; i < 3; i++) {  // Loop xử lý từng button riêng
        switch (buttonState[i]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(i)) {
                buttonState[i] = BUTTON_PRESSED;
                // Xử lý short press cho từng button
                if (i == 0) {  // Button 0: Tăng mode
                    mode++;
                } else if (i == 1 && red_timer < 99000) {  // Button 1: Tăng timer_temp
                    switch (mode) {
                        case 2: green_timer_temp += 1000; red_timer_temp += 1000; break;
                        case 3: yellow_timer_temp += 1000; red_timer_temp += 1000; break;
                        case 4: green_timer_temp += 1000; red_timer_temp += 1000; break;
                    }
                } else if (i == 2) {  // Button 2: Apply temp và reset mode
                    mode = 1;
                    red_timer = red_timer_temp;
                    green_timer = green_timer_temp;
                    yellow_timer = yellow_timer_temp;
                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(i)) {
                buttonState[i] = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(i)) {
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
                    if (i == 0)
                    {  // Button 0: Tăng mode tự động
                        mode++;
                    }
                    else if (i == 1 && red_timer < 99000)
                    {  // Button 1: Tăng timer_temp tự động
                        switch (mode)
                        {
                            case 2: green_timer_temp += 1000; red_timer_temp += 1000; break;
                            case 3: yellow_timer_temp += 1000; red_timer_temp += 1000; break;
                            case 4: green_timer_temp += 1000; red_timer_temp += 1000; break;
                        }
                    }

                    flagForIncreasingCounter[i] = 0;
                }
            }
            break;
        }
    }
    if (mode == 5) {
        mode = 1;
    }
}
