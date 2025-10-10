#include "main.h"
#include "led_display.h"
#include "input_reading.h"

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

uint16_t SEG_TIMERs[7] =
{
		SEG_TIMER0_Pin, SEG_TIMER1_Pin, SEG_TIMER2_Pin, SEG_TIMER3_Pin, SEG_TIMER4_Pin, SEG_TIMER5_Pin, SEG_TIMER6_Pin
};

uint16_t EN_Pins[2] =
{
		EN0_Pin, EN1_Pin
};

uint8_t led_buffer0[2] = {0, 0};
uint8_t led_buffer1[2] = {0, 0};
static uint8_t last_mode = 0;
static uint8_t current_digit = 0;




//void displayMode()
//{
//	uint8_t bitmask = segments[mode];
//
//	for (uint8_t i = 0; i <= 6; i++)
//	{
//		HAL_GPIO_WritePin(GPIOA, SEG_Pins[i], bitmask & (1 << (6 - i)) ? RESET : SET);
//	}
//}
//
//void displayTimer()
//{
//	switch (led_index_buffer0)
//	{
//	case 0:
//		HAL_GPIO_WritePin(GPIOC, EN_Pins[0], RESET);
//		HAL_GPIO_WritePin(GPIOC, EN_Pins[1], SET);
//		break;
//	case 1:
//		HAL_GPIO_WritePin(GPIOC, EN_Pins[1], RESET);
//		HAL_GPIO_WritePin(GPIOC, EN_Pins[0], SET);
//		break;
//	}
//	uint8_t bitmask = segments[led_buffer0[led_index_buffer0]];
//
//		for (uint8_t i = 0; i <= 6; i++)
//		{
//			HAL_GPIO_WritePin(GPIOA, SEG_TIMERs[i], bitmask & (1 << (6 - i)) ? RESET : SET);
//		}
//}

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
//    static uint32_t last_switch_time = 0;

    // Chỉ chuyển digit mỗi 5ms
//    if (HAL_GetTick() - last_switch_time < 5) {
//        return;
//    }
//    last_switch_time = HAL_GetTick();

    // TẮT toàn bộ digit trước



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

    uint8_t bitmask = segments[led_buffer0[current_digit]];
    for (uint8_t i = 0; i <= 6; i++) {
        HAL_GPIO_WritePin(GPIOA, SEG_TIMERs[i], bitmask & (1 << (6 - i)) ? RESET : SET);
    }

    // Chuyển digit
    current_digit++;
    if (current_digit >= 2) {
        current_digit = 0;
    }
}
