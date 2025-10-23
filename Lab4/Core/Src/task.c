#include "task.h"
#include "sched.h"
#include "timer.h"

uint32_t red_timer = 500, green_timer = 300, yellow_timer = 200;

enum traffic_states state = INIT;

void LED_A5_BLINK()
{
	GPIOA->ODR ^= LED_A5_Pin;
}

void LED_A4_BLINK()
{
	GPIOA->ODR ^= LED_A4_Pin;
}

void LED_A3_BLINK()
{
	GPIOA->ODR ^= LED_A3_Pin;
}

void LED_A2_BLINK()
{
	GPIOA->ODR ^= LED_A2_Pin;
}

void LED_RED()
{
	GPIOB->BSRR = LED_RED_Pin << 16 | LED_GREEN_Pin | LED_YELLOW_Pin;
}

void LED_GREEN()
{
	GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin << 16 | LED_YELLOW_Pin;
}

void LED_YELLOW()
{
	GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin | LED_YELLOW_Pin << 16;
}

void traffic_auto()
{
	SCH_Add_Task(LED_RED, 0, red_timer * 2);
	SCH_Add_Task(LED_GREEN, red_timer, red_timer * 2);
	SCH_Add_Task(LED_YELLOW, red_timer + green_timer, red_timer * 2);
//	switch (state)
//	{
//	case INIT:
//		setTimer1(red_timer);
//		state = RED;
//		break;
//	case RED:
//		GPIOB->BSRR = LED_RED_Pin << 16 | LED_GREEN_Pin | LED_YELLOW_Pin;
//		if (timer1_flag == 1)
//		{
//			setTimer1(green_timer);
//			state = GREEN;
//		}
//		break;
//	case GREEN:
//		GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin << 16 | LED_YELLOW_Pin;
//		if (timer1_flag == 1)
//		{
//			setTimer1(yellow_timer);
//			state = YELLOW;
//		}
//		break;
//	case YELLOW:
//		GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin | LED_YELLOW_Pin << 16;
//		if (timer1_flag == 1)
//		{
//			setTimer1(red_timer);
//			state = RED;
//		}
//		break;
//	}

}








