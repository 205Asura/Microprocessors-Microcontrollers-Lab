#include "main.h"
#include "led_display.h"
#include "input_reading.h"
#include "fsm_traffic.h"
#include "timer.h"

static uint8_t segments[10] =
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

static uint16_t SEG_Pins[7] =
{
		SEG0_MODE_Pin, SEG1_MODE_Pin, SEG2_MODE_Pin, SEG3_MODE_Pin, SEG4_MODE_Pin, SEG5_MODE_Pin, SEG6_MODE_Pin
};

static uint16_t SEG_TIMER0[7] =
{
		SEG0_TIMER0_Pin, SEG1_TIMER0_Pin, SEG2_TIMER0_Pin, SEG3_TIMER0_Pin, SEG4_TIMER0_Pin, SEG5_TIMER0_Pin, SEG6_TIMER0_Pin
};

static uint16_t SEG_TIMER1[7] =
{
		SEG0_TIMER1_Pin, SEG1_TIMER1_Pin, SEG2_TIMER1_Pin, SEG3_TIMER1_Pin, SEG4_TIMER1_Pin, SEG5_TIMER1_Pin, SEG6_TIMER1_Pin
};

static uint16_t EN_Pins[2] =
{
		EN0_Pin, EN1_Pin
};

static uint8_t led_buffer0[2] = {0, 0};
static uint8_t led_buffer1[2] = {0, 0};

static uint8_t last_mode = 0;
static uint8_t current_digit = 0;
static uint8_t bitmask0, bitmask1;





void displayMode()
{

    // Chỉ cập nhật khi mode thay đổi
    if (mode != last_mode)
    {
        uint8_t bitmask = segments[mode];
        uint32_t reset_mask, set_mask = 0;
        for (uint8_t i = 0; i <= 6; i++)
        {
        	if (bitmask & (1 << (6 - i)))
        		reset_mask |= SEG_Pins[i];
        	else
        		set_mask |= SEG_Pins[i];
        }
        GPIOA->BSRR = (reset_mask << 16) | set_mask;
        last_mode = mode;
    }
}

// Trong led_display.c
void displayTimer()
{

    switch (current_digit)
    {
    case 0:
    	GPIOC->BSRR = (EN_Pins[0] << 16 | EN_Pins[1]);
        break;
    case 1:
    	GPIOC->BSRR = (EN_Pins[0] | EN_Pins[1] << 16);
        break;
    }

    uint32_t set_maskA = 0, reset_maskA = 0, set_maskB = 0, reset_maskB = 0;
    switch (mode)
    {
    case 1:
    	led_buffer0[0] = timer_line0 / 10000;
		led_buffer0[1] = (timer_line0 % 10000) / 1000;
		led_buffer1[0] = timer_line1 / 10000;
		led_buffer1[1] = (timer_line1 % 10000) / 1000;
		bitmask0 = segments[led_buffer0[current_digit]];
		bitmask1 = segments[led_buffer1[current_digit]];
		for (uint8_t i = 0; i <= 6; i++)
		{
			if (bitmask0 & (1 << (6 - i)))
				reset_maskA |= SEG_TIMER0[i];
			else
				set_maskA |= SEG_TIMER0[i];
			if (bitmask1 & (1 << (6 - i)))
				reset_maskB |= SEG_TIMER1[i];
			else
				set_maskB |= SEG_TIMER1[i];
		}

	break;

    case 2:
    	led_buffer0[0] = red_timer_temp0 / 10000;
    	led_buffer0[1] = (red_timer_temp0 % 10000) / 1000;
    	led_buffer1[1] = mode;
    	bitmask0 = segments[led_buffer0[current_digit]];
    	bitmask1 = segments[led_buffer1[current_digit]];
    	for (uint8_t i = 0; i <= 6; i++)
		{
    		if (bitmask0 & (1 << (6 - i)))
				reset_maskA |= SEG_TIMER0[i];
			else
				set_maskA |= SEG_TIMER0[i];
			if (bitmask1 & (1 << (6 - i)))
				reset_maskB |= SEG_TIMER1[i];
			else
				set_maskB |= SEG_TIMER1[i];
		}
    break;
    case 3:
    	led_buffer0[0] = yellow_timer_temp0 / 10000;
		led_buffer0[1] = (yellow_timer_temp0 % 10000) / 1000;
		led_buffer1[1] = mode;
		bitmask0 = segments[led_buffer0[current_digit]];
		bitmask1 = segments[led_buffer1[current_digit]];
		for (uint8_t i = 0; i <= 6; i++)
		{
			if (bitmask0 & (1 << (6 - i)))
				reset_maskA |= SEG_TIMER0[i];
			else
				set_maskA |= SEG_TIMER0[i];
			if (bitmask1 & (1 << (6 - i)))
				reset_maskB |= SEG_TIMER1[i];
			else
				set_maskB |= SEG_TIMER1[i];
		}
	break;

	case 4:
		led_buffer0[0] = green_timer_temp0 / 10000;
		led_buffer0[1] = (green_timer_temp0 % 10000) / 1000;
		led_buffer1[1] = mode;
		bitmask0 = segments[led_buffer0[current_digit]];
		bitmask1 = segments[led_buffer1[current_digit]];
		for (uint8_t i = 0; i <= 6; i++)
		{
			if (bitmask0 & (1 << (6 - i)))
				reset_maskA |= SEG_TIMER0[i];
			else
				set_maskA |= SEG_TIMER0[i];
			if (bitmask1 & (1 << (6 - i)))
				reset_maskB |= SEG_TIMER1[i];
			else
				set_maskB |= SEG_TIMER1[i];
		}
		break;
    }

    // Chuyển digit
    GPIOA->BSRR = (reset_maskA << 16) | set_maskA;
    GPIOB->BSRR = (reset_maskB << 16) | set_maskB;
    current_digit++;
    if (current_digit >= 2) {
        current_digit = 0;
    }
}
