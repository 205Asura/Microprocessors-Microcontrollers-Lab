#include "timer.h"
#include "sched.h"
#include "main.h"
#include "input_reading.h"
#include "task.h"
#include <stdio.h>
#include <string.h>


uint8_t timer1_flag = 0;
int timer1 = 0;
int timer1_1s_counter = 0;

int timer2 = 0;


// *** BẮT ĐẦU THAY ĐỔI ***

// 1. Khai báo buffer tĩnh để DMA có thể truy cập
// Biến để theo dõi trạng thái DMA
//static uint8_t dma_ready = 1;
//static char tx_buffer[50];
//
//extern UART_HandleTypeDef huart1;
//
//void get_time()
//{
//    // Chỉ gửi nếu DMA đã sẵn sàng
//    if (dma_ready)
//    {
//        dma_ready = 0; // Đánh dấu DMA đang bận
//
//        // Chuẩn bị chuỗi
//        int len = sprintf(tx_buffer, "Current time: %d0 ms\r\n", timer2);
//
//        // Gửi qua DMA
//        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_buffer, len);
//    }
//}
//
//// Callback khi DMA hoàn thành (thêm vào timer.c)
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == USART1) {
//        dma_ready = 1; // Đánh dấu DMA đã sẵn sàng cho lần tiếp theo
//    }
//}
// *** KẾT THÚC THAY ĐỔI ***


void timer_run()
{

	if (timer1 > 0)
	{
		timer1_1s_counter--;
		if (timer1_1s_counter <= 0)
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
