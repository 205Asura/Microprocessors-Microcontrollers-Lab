#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

extern uint8_t led_index_buffer0;

extern uint8_t segments[10];
extern uint8_t led_buffer0[2], led_buffer1[2], led_buffer2[2];
extern uint16_t SEG_Pins[7];
extern uint16_t SEG_TIMER0[7];
extern uint16_t SEG_TIMER1[7];
extern uint16_t EN_Pins[2];

void displayMode();
void displayTimer();

#endif
