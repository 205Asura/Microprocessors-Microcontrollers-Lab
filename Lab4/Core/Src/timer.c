#include "timer.h"
#include "sched.h"
#include "main.h"
#include <stdio.h>


uint8_t timer1_flag = 0;
int timer1 = 0;
int timer1_1s_counter = 0;

int timer2 = 0;

void timer_run()
{

	if (timer1 > 0)
	{
		timer1_1s_counter--;
		if (timer1_1s_counter == 0)
		{
			timer1 -= 100;
			timer1_1s_counter = 100;

		}
	}

	timer2++;

	if (timer1 == 0)
	{
		timer1_flag = 1;

	}

}

void setTimer1(uint32_t duration)
{
	timer1_flag = 0;
	timer1 = duration;
	timer1_1s_counter = 100;

}

void get_time()
{
	printf("Current time: %d0 ms\r\n", timer2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
    	SCH_Update();
    }
}
