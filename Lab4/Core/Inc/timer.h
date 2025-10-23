#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>



extern uint8_t timer1_flag;
extern int32_t timer1_counter;
extern int32_t timer1_1s_counter;
extern uint8_t timer1_run_id;
void setTimer1(uint32_t timer1_duration);

#endif
