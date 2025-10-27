#include "task.h"
#include "sched.h"
#include "timer.h"
#include "main.h"



uint32_t red_timer = 500, green_timer = 300, yellow_timer = 200;
uint8_t seg_counter = 0;


enum traffic_states state = RED;

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

static uint16_t SEG_ERROR[7] =
{
		SEG0_ERROR_Pin, SEG1_ERROR_Pin, SEG2_ERROR_Pin, SEG3_ERROR_Pin, SEG4_ERROR_Pin, SEG5_ERROR_Pin, SEG6_ERROR_Pin
};

uint8_t seg_buffer[3] =
{
		0, 0, 0
};

void LED_A3_BLINK()
{
	GPIOA->ODR ^= LED_A3_Pin;

}

void LED_A2_BLINK()
{
	GPIOA->ODR ^= LED_A2_Pin;

}

void LED_A1_BLINK()
{
	GPIOA->ODR ^= LED_A1_Pin;

}

void LED_A0_BLINK()
{
	GPIOA->ODR ^= LED_A0_Pin;

}

void LED_RED_BLINK()
{
	GPIOA->ODR ^= LED_RED_Pin;

}

void updateSegBuffer()
{

	seg_buffer[0] = seg_counter;
	seg_buffer[1] = Error_code_G;
	seg_buffer[2] = timer1 / 100;
}
static uint8_t index_buffer = 2;

void display7Seg0() // BUTTON
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

void display7Seg1() // Error
{
	GPIOA->BSRR = EN1_Pin << 16 | EN0_Pin;
	int mask = bitmask[seg_buffer[1]];
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

void display7Seg2() // TRAFFIC + ERROR
{
	if (index_buffer == 1)
		GPIOA->BSRR = EN1_Pin << 16 | EN0_Pin;
	else
		GPIOA->BSRR = EN0_Pin << 16 | EN1_Pin;
	int mask = bitmask[seg_buffer[index_buffer++]];
	int reset_mask = 0, set_mask = 0;
//	printf("sef_buffer[2] = %d\r\n", Error_code_G);
	for (int i = 0; i < 7; i++)
	{
		if (mask & (1 << (6 - i)))
			reset_mask |= SEG_ERROR[i];
		else
			(set_mask |= SEG_ERROR[i]);
	}
	GPIOB->BSRR = (reset_mask << 16) | set_mask;
	if (index_buffer == 3)
	{
		index_buffer = 1;
	}

}
//void display7Seg1()
//{
//	int mask = bitmask[seg_buffer[1]];
//		int reset_mask = 0, set_mask = 0;
//		for (int i = 0; i < 7; i++)
//		{
//			if (mask & (1 << (6 - i)))
//				reset_mask |= SEG_TIMER1[i];
//			else
//				(set_mask |= SEG_TIMER1[i]);
//		}
//		GPIOB->BSRR = (reset_mask << 16) | set_mask;
//}

void traffic_auto()
{
	switch (state)
	{
//	case INIT:
//		setTimer1(red_timer);
//		state = RED;
//		break;
	case RED:
		GPIOA->BSRR = LED_RED_Pin << 16 | LED_GREEN_Pin | LED_YELLOW_Pin;
		if (timer1_flag == 1)
		{
			setTimer1(green_timer);
			state = GREEN;
		}
		break;
	case GREEN:
		GPIOA->BSRR = LED_RED_Pin | LED_GREEN_Pin << 16 | LED_YELLOW_Pin;
		if (timer1_flag == 1)
		{
			setTimer1(yellow_timer);
			state = YELLOW;
		}
		break;
	case YELLOW:
		GPIOA->BSRR = LED_RED_Pin | LED_GREEN_Pin | LED_YELLOW_Pin << 16;
		if (timer1_flag == 1)
		{
			setTimer1(red_timer);
			state = RED;
		}
		break;
	}

}








