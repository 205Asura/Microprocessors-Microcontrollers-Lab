#include "main.h"
#include "led_display.h"
#include "input_reading.h"
#include "fsm_traffic.h"
#include "timer.h"

uint8_t led_index_buffer0 = 0;
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

uint16_t SEG_TIMER0[7] =
{
		SEG_TIMER0_Pin, SEG_TIMER1_Pin, SEG_TIMER2_Pin, SEG_TIMER3_Pin, SEG_TIMER4_Pin, SEG_TIMER5_Pin, SEG_TIMER6_Pin
};

uint16_t SEG_TIMER1[7] =
{
		SEG0_TIMER1_Pin, SEG1_TIMER1_Pin, SEG2_TIMER1_Pin, SEG3_TIMER1_Pin, SEG4_TIMER1_Pin, SEG5_TIMER1_Pin, SEG6_TIMER1_Pin
};

uint16_t EN_Pins[2] =
{
		EN0_Pin, EN1_Pin
};

uint8_t led_buffer0[2] = {0, 0};
uint8_t led_buffer1[2] = {0, 0};
uint8_t led_buffer2[2] = {0, 0};

static uint8_t last_mode = 0;
static uint8_t current_digit = 0;
static uint8_t bitmask0, bitmask1;





void displayMode()
{

    // Chỉ cập nhật khi mode thay đổi
    if (mode != last_mode) {
        uint8_t bitmask = segments[mode];

        for (uint8_t i = 0; i <= 6; i++) {
            HAL_GPIO_WritePin(GPIOA, SEG_Pins[i], bitmask & (1 << (6 - i)) ? RESET : SET);
        }

        last_mode = mode;
    }
}

// Trong led_display.c
void displayTimer()
{

    // Hiển thị digit hiện tại
    switch (current_digit)
    {
    case 0:
        HAL_GPIO_WritePin(GPIOC, EN_Pins[0], RESET);
        HAL_GPIO_WritePin(GPIOC, EN_Pins[1], SET);
        break;
    case 1:
    	HAL_GPIO_WritePin(GPIOC, EN_Pins[0], SET);
        HAL_GPIO_WritePin(GPIOC, EN_Pins[1], RESET);
        break;
    }
    switch (mode)
    {
    case 1:
    	led_buffer0[0] = timer2 / 10000;
		led_buffer0[1] = (timer2 % 10000) / 1000;
		led_buffer1[0] = timer3 / 10000;
		led_buffer1[1] = (timer3 % 10000) / 1000;
		bitmask0 = segments[led_buffer0[current_digit]];
		bitmask1 = segments[led_buffer1[current_digit]];
		for (uint8_t i = 0; i <= 6; i++)
		{
			HAL_GPIO_WritePin(GPIOA, SEG_TIMER0[i], bitmask0 & (1 << (6 - i)) ? RESET : SET);
			HAL_GPIO_WritePin(GPIOB, SEG_TIMER1[i], bitmask1 & (1 << (6 - i)) ? RESET : SET);
		}
	break;

    case 2:
    	led_buffer2[0] = red_timer_temp / 10000;
    	led_buffer2[1] = (red_timer_temp % 10000) / 1000;
    	bitmask0 = segments[led_buffer2[current_digit]];
    	for (uint8_t i = 0; i <= 6; i++)
		{
			HAL_GPIO_WritePin(GPIOA, SEG_TIMER0[i], bitmask0 & (1 << (6 - i)) ? RESET : SET);
			HAL_GPIO_WritePin(GPIOB, SEG_TIMER1[i], SET);  // Off
		}
    break;
    case 3:
		led_buffer2[0] = yellow_timer_temp / 10000;
		led_buffer2[1] = (yellow_timer_temp % 10000) / 1000;
		bitmask0 = segments[led_buffer2[current_digit]];
		for (uint8_t i = 0; i <= 6; i++) {
			HAL_GPIO_WritePin(GPIOA, SEG_TIMER0[i], bitmask0 & (1 << (6 - i)) ? RESET : SET);
			HAL_GPIO_WritePin(GPIOB, SEG_TIMER1[i], SET);  // Off
		}
		break;

	case 4:
		led_buffer2[0] = green_timer_temp / 10000;
		led_buffer2[1] = (green_timer_temp % 10000) / 1000;
		bitmask0 = segments[led_buffer2[current_digit]];
		for (uint8_t i = 0; i <= 6; i++) {
			HAL_GPIO_WritePin(GPIOA, SEG_TIMER0[i], bitmask0 & (1 << (6 - i)) ? RESET : SET);
			HAL_GPIO_WritePin(GPIOB, SEG_TIMER1[i], SET);  // Off
		}
		break;
    }

    // Chuyển digit
    current_digit++;
    if (current_digit >= 2) {
        current_digit = 0;
    }
}
