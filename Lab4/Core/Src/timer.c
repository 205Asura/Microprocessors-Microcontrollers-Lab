#include "timer.h"
#include "sched.h"
#include "main.h"


uint8_t timer1_flag = 0;
uint8_t timer1_run_id = 0;
int32_t timer1_counter = 0;
int32_t timer1_1s_counter = 0;

void timer1_run()
{

	if (timer1_1s_counter > 0)
		timer1_1s_counter--;

	if (timer1_1s_counter == 0)
	{
		timer1_counter -= 100;
		timer1_1s_counter = 100;
	}

	if (timer1_counter == 0)
	{
		timer1_flag = 1;
		SCH_Delete_Task(timer1_run_id);
	}

}

void setTimer1(uint32_t duration)
{
	timer1_flag = 0;
	timer1_counter = duration;
	timer1_1s_counter = 100;
	timer1_run_id = SCH_Add_Task(timer1_run, 0, 1);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
    	SCH_Update();
    }
}
