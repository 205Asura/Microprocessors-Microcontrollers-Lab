#ifndef INC_TIMER_H
#define INC_TIMER_H

extern unsigned char timer_flag;

extern volatile int timer1;
extern volatile int timer2;
extern volatile int timer3;
extern volatile int timer4;
extern volatile int timer5;




extern volatile int timer1_duration;
extern volatile int timer2_duration;
extern volatile int timer3_duration;
extern volatile int timer4_duration;
extern volatile int timer5_duration;


extern volatile uint8_t timer1_flag;
extern volatile uint8_t timer2_flag;
extern volatile uint8_t timer3_flag;
extern volatile uint8_t timer4_flag;
extern volatile uint8_t timer5_flag;




void timer_run();

#define TIMER_PERIOD_MS 10

#endif
