#include "main.h"
#include "input_reading.h"
#include "timer.h"

volatile uint16_t timer1 = 0;
volatile uint16_t timer2 = 0;

volatile uint16_t timer1_duration = 100; // ms
volatile uint16_t timer2_duration = 0; // ms

volatile uint8_t timer1_flag = 0;
volatile uint8_t timer2_flag = 0;

void setTimer1(uint16_t duration)
{
	timer1_duration = duration;
}

void setTimer2(uint16_t duration)
{
	timer2_duration = duration;
}

void timer_run()
{
	timer1 += TIMER_PERIOD_MS; // timer for blinking led 2hz
	timer2 += TIMER_PERIOD_MS; // timer for auto traffic
	if (timer1 >= timer1_duration)
	{
		timer1_flag = 1;
		timer1 = 0;
	}
	if (timer2 >= timer2_duration)
	{
		timer2_flag = 1;
		timer2 = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
    	timer_run();
        button_reading();
    }
}
