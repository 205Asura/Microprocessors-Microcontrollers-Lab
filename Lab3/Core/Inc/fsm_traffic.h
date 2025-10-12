#ifndef FSM_TRAFFI_H
#define FSM_TRAFFI_H

void fsm_traffic();
enum status {START, RED0_GREEN1_AUTO, RED0_YELLOW1_AUTO, GREEN0_RED1_AUTO, YELLOW0_RED1_AUTO, RED_MAN, GREEN_MAN, YELLOW_MAN};
extern enum status status;
static uint8_t prev_mode;

extern volatile int32_t red_timer0, green_timer0, yellow_timer0, red_timer_temp0, green_timer_temp0, yellow_timer_temp0,
				red_timer1, green_timer1, yellow_timer1, red_timer_temp1, green_timer_temp1, yellow_timer_temp1;

#endif
