#ifndef FSM_TRAFFI_H
#define FSM_TRAFFI_H

void fsm_traffic();
enum status {START, RED0_GREEN1_AUTO, RED0_YELLOW1_AUTO, GREEN0_RED1_AUTO, YELLOW0_RED1_AUTO, RED_MAN, GREEN_MAN, YELLOW_MAN};
extern enum status status;
extern uint8_t prev_mode;
extern uint16_t red_timer, green_timer, yellow_timer, red_timer_temp, green_timer_temp, yellow_timer_temp;

#endif
