#include "main.h"
#include "fsm_traffic.h"
#include "timer.h"
#include "input_reading.h"
#include "led_display.h"
enum status status = START;
static uint8_t prev_mode = 1;
uint32_t red_timer = 5000, green_timer = 3000, yellow_timer = 2000;
uint32_t red_timer_temp = 5000, green_timer_temp = 3000, yellow_timer_temp = 2000;



void fsm_traffic()
{

	if (prev_mode != mode)
	{
		prev_mode = mode;
		if (mode == 1)
			status = START;
	}
	switch (mode)
	{
	case 1:

	switch (status)
	{
		case START:
		status = RED0_GREEN1_AUTO;
		setTimer2(red_timer);
		setTimer3(green_timer);
		break;

		case RED0_GREEN1_AUTO:

		GPIOB->BSRR = ((LED_RED0_Pin | LED_GREEN1_Pin) << 16)
		| LED_GREEN0_Pin | LED_YELLOW0_Pin | LED_RED1_Pin | LED_YELLOW1_Pin;
		if (timer2 <= yellow_timer || timer3_flag == 1)
		{
			status = RED0_YELLOW1_AUTO;
			setTimer3(yellow_timer);
		}

		break;

		case RED0_YELLOW1_AUTO:
		GPIOB->BSRR = ((LED_RED0_Pin | LED_YELLOW1_Pin) << 16)
		| LED_GREEN0_Pin | LED_YELLOW0_Pin | LED_RED1_Pin | LED_GREEN1_Pin;
		if (timer2_flag == 1 || timer3_flag == 1)
		{
			status = GREEN0_RED1_AUTO;
			setTimer2(green_timer);
			setTimer3(red_timer);
		}
		break;

		case GREEN0_RED1_AUTO:

		GPIOB->BSRR = ((LED_RED1_Pin | LED_GREEN0_Pin) << 16)
		| LED_RED0_Pin | LED_YELLOW0_Pin | LED_GREEN1_Pin | LED_YELLOW1_Pin;
		if (timer2_flag == 1 || timer3 <= yellow_timer)
		{
			status = YELLOW0_RED1_AUTO;
			setTimer2(yellow_timer); // 5s for red
		}
		break;

		case YELLOW0_RED1_AUTO:
		GPIOB->BSRR = ((LED_RED1_Pin | LED_YELLOW0_Pin) << 16)
		| LED_GREEN0_Pin | LED_RED0_Pin | LED_GREEN1_Pin | LED_YELLOW1_Pin;
		if (timer2_flag == 1 || timer3_flag == 1)
		{
			status = RED0_GREEN1_AUTO;
			setTimer2(red_timer); // 5s for red
			setTimer3(green_timer);
		}
	}
	break;

	case 2:

	if (timer1_flag == 1)
	{
		GPIOB->ODR ^= LED_RED0_Pin | LED_RED1_Pin;
		setTimer1(100);
	}
	GPIOB->BSRR = LED_GREEN0_Pin | LED_YELLOW0_Pin | LED_GREEN1_Pin | LED_YELLOW1_Pin;
	break;

	case 3:
	if (timer1_flag == 1)
	{
		GPIOB->ODR ^= LED_YELLOW0_Pin | LED_YELLOW1_Pin;
		setTimer1(100);
	}
	GPIOB->BSRR = LED_GREEN0_Pin | LED_RED0_Pin | LED_GREEN1_Pin | LED_RED1_Pin;
	break;

	case 4:
	if (timer1_flag == 1)
	{
		GPIOB->ODR ^= LED_GREEN0_Pin | LED_GREEN1_Pin;
		setTimer1(100);
	}
	GPIOB->BSRR = LED_YELLOW0_Pin | LED_RED0_Pin | LED_YELLOW1_Pin | LED_RED1_Pin;
	break;

	}

}
