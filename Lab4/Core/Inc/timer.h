#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>



extern uint8_t timer1_flag;
extern int timer1;
extern int timer1_1s_counter;

extern int timer2;

void timer_run();
void get_time();
void setTimer1(uint32_t duration);

#endif
