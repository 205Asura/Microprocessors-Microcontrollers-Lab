#include "main.h"
#include "led_display.h"
#include "input_reading.h"
uint8_t segments[10] =
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

uint16_t SEG_Pins[7] =
{
		SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin
};

uint8_t led_buffer1[2] = {0, 0};
uint8_t led_buffer2[2] = {0, 0};


void displayMode()
{
	uint8_t bitmask = segments[mode];

	for (uint8_t i = 0; i <= 6; i++)
	{
		HAL_GPIO_WritePin(GPIOA, SEG_Pins[i], bitmask & (1 << (6 - i)) ? RESET : SET);
	}
}

void displayTimer();
