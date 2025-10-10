#include "main.h"
#include "fsm_traffic.h"
#include "timer.h"
#include "input_reading.h"
#include "led_display.h"
enum status status = START;
uint8_t isButton0Pressed = 0;
uint8_t prev_mode = 1;
uint16_t red_timer = 5000, green_timer = 3000, yellow_timer = 2000;
uint16_t red_timer_temp = 5000, green_timer_temp = 3000, yellow_timer_temp = 2000;



void fsm_traffic()
{
	if (prev_mode != mode)
	{
		status = START;
		prev_mode = mode;
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

		HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, SET);
		if (timer2 <= yellow_timer || timer3_flag == 1)
		{
			timer3_flag = 0;
			status = RED0_YELLOW1_AUTO;
			setTimer3(yellow_timer);
		}

		break;

		case RED0_YELLOW1_AUTO:
		HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, RESET);
		if (timer2_flag == 1 || timer3_flag == 1)
		{
			timer2_flag = 0;
			timer3_flag = 0;
			status = GREEN0_RED1_AUTO;
			setTimer2(green_timer);
			setTimer3(red_timer);
		}
		break;

		case GREEN0_RED1_AUTO:

		HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, SET);
		if (timer2_flag == 1 || timer3 <= yellow_timer)
		{
			timer2_flag = 0;
			status = YELLOW0_RED1_AUTO;
			setTimer2(yellow_timer); // 5s for red
		}
		break;

		case YELLOW0_RED1_AUTO:
		HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, SET);
		HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, SET);
		if (timer2_flag == 1 || timer3_flag == 1)
		{
			timer2_flag = 0;
			timer3_flag = 0;
			status = RED0_GREEN1_AUTO;
			setTimer2(red_timer); // 5s for red
			setTimer3(green_timer);
		}
	}
	break;

	case 2:

	if (timer1_flag == 1)
	{
		HAL_GPIO_TogglePin(GPIOB, LED_RED0_Pin);
		HAL_GPIO_TogglePin(GPIOB, LED_RED1_Pin);
		timer1_flag = 0;
	}
	HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, SET);
	break;

	case 3:
	if (timer1_flag == 1)
	{
		HAL_GPIO_TogglePin(GPIOB, LED_YELLOW0_Pin);
		HAL_GPIO_TogglePin(GPIOB, LED_YELLOW1_Pin);
		timer1_flag = 0;
	}
	HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_GREEN0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_GREEN1_Pin, SET);
	break;

	case 4:
	if (timer1_flag == 1)
	{
		HAL_GPIO_TogglePin(GPIOB, LED_GREEN0_Pin);
		HAL_GPIO_TogglePin(GPIOB, LED_GREEN1_Pin);
		timer1_flag = 0;
	}
	HAL_GPIO_WritePin(GPIOB, LED_RED0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_YELLOW0_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, LED_YELLOW1_Pin, SET);
	break;

	}

}
