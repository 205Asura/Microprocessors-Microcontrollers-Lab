#include "main.h"
#include "input_reading.h"
#include "timer.h"
#include "led_display.h"

volatile int timer1 = 250; // timer for blinking leds
volatile int timer2 = 0; // timer for auto traffic
volatile int timer3 = 0; // timer for auto traffic 2
volatile int timer4 = 100; // timer for invoking fsm, display
volatile int timer5 = 250;

int timer_line0 = 0;
int timer_line1 = 0;


volatile uint8_t timer1_flag = 0;
volatile uint8_t timer2_flag = 0;
volatile uint8_t timer3_flag = 0;
volatile uint8_t timer4_flag = 0;
volatile uint8_t timer5_flag = 0;



static uint16_t timer2_1scount = 1000;
static uint16_t timer3_1scount = 1000;


void setTimer1(int duration)
{
	timer1_flag = 0;
	timer1 = duration;
}

void setTimer2(int duration)
{
	timer2_flag = 0;
	timer2 = duration;
	timer2_1scount = 1000;
}

void setTimer3(int duration)
{
	timer3_flag = 0;
	timer3 = duration;
	timer3_1scount = 1000;
}

void setTimer4(int duration)
{
	timer4_flag = 0;
	timer4 = duration;
}

void setTimer5(int duration)
{
	timer5_flag = 0;
	timer5 = duration;
}
void timer_run()
{
	if (timer1 > 0)
		timer1 -= TIMER_PERIOD_MS; // timer for blinking led 2hz


	if (timer2 > 0)
	{
		timer2_1scount -= TIMER_PERIOD_MS;
		if (timer2_1scount <= 0)
		{
			timer2 -= 1000;
			timer2_1scount = 1000;
		}
	}

	if (timer3 > 0)
	{
		timer3_1scount -= TIMER_PERIOD_MS;
		if (timer3_1scount <= 0)
		{
			timer3 -= 1000;
			timer3_1scount = 1000;
		}
	}

	if (timer4 > 0)
		timer4 -= TIMER_PERIOD_MS;

	if (timer5 > 0)
		timer5 -= TIMER_PERIOD_MS;

	if (timer1 <= 0)
	{
		timer1_flag = 1;

	}
	if (timer2 <= 0)
	{
		timer2_flag = 1;
	}

	if (timer3 <= 0)
	{
		timer3_flag = 1;
	}

	if (timer4 <= 0)
	{
		timer4_flag = 1;
	}

	if (timer5 <= 0)
	{
		timer5_flag = 1;
	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
    	timer_run();
    	button_reading();
    }
}
