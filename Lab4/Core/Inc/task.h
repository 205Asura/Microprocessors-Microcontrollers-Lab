#ifndef TASK_H
#define TASK_H

#include "main.h"

enum traffic_states{INIT, RED, GREEN, YELLOW};
extern enum traffic_states state;

extern uint32_t red_timer, green_timer, yellow_timer;
extern uint8_t seg_buffer[2];

void LED_A5_BLINK();
void LED_A4_BLINK();
void LED_A3_BLINK();
void LED_A2_BLINK();

void LED_RED();
void LED_GREEN();
void LED_YELLOW();

void traffic_auto();
void display7Seg();
void updateSeg0Buffer();

#endif
