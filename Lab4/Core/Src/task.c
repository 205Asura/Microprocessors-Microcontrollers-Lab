#include "task.h"

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
