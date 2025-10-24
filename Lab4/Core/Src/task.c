#include "task.h"
#include "sched.h"
#include "timer.h"
#include "main.h"

uint32_t red_timer = 500, green_timer = 300, yellow_timer = 200;

enum traffic_states state = INIT;

static uint8_t bitmask[10] =
{
		0b1111110, // zero
		0b0110000, // one
		0b1101101, // two
		0b1111001, // three
		0b0110011, // four
		0b1011011, // five
		0b1011111, // six
		0b1110000, // seven
		0b1111111, // eight
		0b1111011  // nine
};

static uint16_t SEG_TIMER0[7] =
{
		SEG0_TIMER0_Pin, SEG1_TIMER0_Pin, SEG2_TIMER0_Pin, SEG3_TIMER0_Pin, SEG4_TIMER0_Pin, SEG5_TIMER0_Pin, SEG6_TIMER0_Pin
};

uint8_t seg_buffer[2] =
{
		0, 0
};

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

void updateSeg0Buffer()
{
	seg_buffer[0] = timer1 / 100;
}

void display7Seg()
{
	int mask = bitmask[seg_buffer[0]];
	int reset_mask = 0, set_mask = 0;
	for (int i = 0; i < 7; i++)
	{
		if (mask & (1 << (6 - i)))
			reset_mask |= SEG_TIMER0[i];
		else
			(set_mask |= SEG_TIMER0[i]);
	}
	GPIOB->BSRR = (reset_mask << 16) | set_mask;

}

void traffic_auto()
{
	switch (state)
	{
	case INIT:
		setTimer1(red_timer);
		state = RED;
		break;
	case RED:
		GPIOB->BSRR = LED_RED_Pin << 16 | LED_GREEN_Pin | LED_YELLOW_Pin;
		if (timer1_flag == 1)
		{
			setTimer1(green_timer);
			state = GREEN;
		}
		break;
	case GREEN:
		GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin << 16 | LED_YELLOW_Pin;
		if (timer1_flag == 1)
		{
			setTimer1(yellow_timer);
			state = YELLOW;
		}
		break;
	case YELLOW:
		GPIOB->BSRR = LED_RED_Pin | LED_GREEN_Pin | LED_YELLOW_Pin << 16;
		if (timer1_flag == 1)
		{
			setTimer1(red_timer);
			state = RED;
		}
		break;
	}

}








