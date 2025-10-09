#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

extern uint8_t segments[10];
extern uint8_t led_buffer0[2], led_buffer1[2];


void displayMode();
void displayTimer();

#endif
