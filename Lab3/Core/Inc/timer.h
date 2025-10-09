#ifndef INC_TIMER_H
#define INC_TIMER_H

extern unsigned char timer_flag;

extern volatile uint16_t timer1;
extern volatile uint16_t timer2;

extern volatile uint16_t timer1_duration;
extern volatile uint16_t timer2_duration;

extern volatile uint8_t timer1_flag;
extern volatile uint8_t timer2_flag;

void timer_run();

#define TIMER_PERIOD_MS 10

#endif
