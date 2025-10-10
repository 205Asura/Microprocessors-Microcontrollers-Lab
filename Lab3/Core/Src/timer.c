#include "main.h"
#include "input_reading.h"
#include "timer.h"
#include "led_display.h"

volatile int timer1 = 1; // timer for blinking leds
volatile int timer2 = 1; // timer for auto traffic
volatile int timer3 = 1; // timer for auto traffic 2
volatile int timer4 = 1; // timer for invoking fsm, display


volatile int timer1_duration = 100; // ms
volatile int timer2_duration = 0; // ms
volatile int timer3_duration = 0; // ms
volatile int timer4_duration = 100;



volatile uint8_t timer1_flag = 0;
volatile uint8_t timer2_flag = 0;
volatile uint8_t timer3_flag = 0;
volatile uint8_t timer4_flag = 0;


static uint16_t timer2_1scount = 1000;
static uint16_t timer3_1scount = 1000;


void setTimer1(uint16_t duration)
{
	timer1_duration = duration;
	timer1 = duration;
}

void setTimer2(uint16_t duration)
{
	timer2_duration = duration;
	timer2 = duration;
	timer2_1scount = 1000;
}

void setTimer3(uint16_t duration)
{
	timer3_duration = duration;
	timer3 = duration;
	timer3_1scount = 1000;
}

void setTimer4(uint16_t duration)
{
	timer4_duration = duration;
	timer4 = duration;
}
void timer_run()
{
	timer1 -= TIMER_PERIOD_MS; // timer for blinking led 2hz
//	timer2 -= TIMER_PERIOD_MS; // timer for auto traffic
//	timer3 -= TIMER_PERIOD_MS; // timer for invoking fsm
	timer4 -= TIMER_PERIOD_MS;
	timer2_1scount -= TIMER_PERIOD_MS;
	timer3_1scount -= TIMER_PERIOD_MS;

	if (timer2_1scount <= 0)
	{
		timer2 -= 1000;
		timer2_1scount = 1000;
	}

	if (timer3_1scount <= 0)
	{
		timer3 -= 1000;
		timer3_1scount = 1000;
	}

	if (timer1 <= 0)
	{
		timer1_flag = 1;
		timer1 = timer1_duration;
	}
	if (timer2 <= 0)
	{
		timer2_flag = 1;
		timer2 = timer2_duration;
	}

	if (timer3 <= 0)
	{
		timer3_flag = 1;
		timer3 = timer3_duration;
	}

	if (timer4 <= 0)
	{
		timer4_flag = 1;
		timer4 = timer4_duration;
		fsm_traffic();
		if (mode == 1)
		{
			led_buffer0[0] = timer2 / 10000;
			led_buffer0[1] = (timer2 % 10000) / 1000;
			led_buffer1[0] = timer3 / 10000;
			led_buffer1[1] = (timer3 % 10000) / 1000;
		}
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
    	timer_run();
        button_reading();
//        displayTimer(led_index_buffer0++);
//	  if (led_index_buffer0 == 2)
//		  led_index_buffer0 = 1;
    }
}
