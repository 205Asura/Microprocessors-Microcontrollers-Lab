#include "fsm.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0; // the index in buffer to be write in
uint8_t buffer_flag = 0; // flag to indicate if a char is transmit through uart

CommandType command_flag = CMD_IDLE;
uint8_t command_data[MAX_BUFFER_SIZE];
CMDState cmd_state = CMD_BEFORE;


// UART communication FSM variables


UARTState uart_state = UART_IDLE;
uint32_t timeout_counter = 0;
uint32_t adc_value = 0;
uint32_t last_adc_value = 0;

const char* get_state_name(CMDState s) {
    switch (s) {
        case CMD_BEFORE: return "BEFORE";
        case CMD_START:  return "START";
        case CMD_O:      return "O";
        case CMD_K:      return "K";
        case CMD_R:      return "R";
        case CMD_S:      return "S";
        case CMD_T:      return "T";
//        case CMD_END:    return "END";
        case CMD_WRONG:  return "WRONG";
        default:         return "UNKNOWN";
    }
}

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART2){
    	HAL_UART_Transmit(&huart2, &temp, 1, 100);
        buffer[index_buffer++] = temp;
        if(index_buffer >= MAX_BUFFER_SIZE) {
            index_buffer = 0; // Prevent buffer overflow
        }

        buffer_flag = 1;
        HAL_UART_Receive_IT(&huart2, &temp, 1);
    }
}


void command_parser_fsm(void) {
    static char cmd_buffer[10];
    static uint8_t cmd_index = 0;
    static CMDState cmd_state = CMD_BEFORE;
    static CMDState prev_correct_state = CMD_BEFORE;
    static uint8_t wrong_count = 0;

    if (index_buffer == 0 || buffer_flag == 0) return;

    for (int i = 0; i < index_buffer; i++) {
        uint8_t c = buffer[i];

        switch (cmd_state) {

        case CMD_BEFORE:
            if (c == '!') {
                cmd_index = 0;
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_START;
                prev_correct_state = CMD_START;
                wrong_count = 0;
            }
            break;

        case CMD_START:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                if (cmd_index == 0) {
                    cmd_state = CMD_BEFORE;
                    prev_correct_state = CMD_BEFORE;
                }
                break;
            }

            if (c == '#') {
				// ✅ Kết thúc command sai
				cmd_buffer[cmd_index++] = c;
				cmd_buffer[cmd_index] = '\0';

				char msg[50];
				sprintf(msg, "\r\nUnknown command: %s\r\n", cmd_buffer);
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

				// ✅ Reset toàn bộ để user gõ lại
				cmd_state = CMD_BEFORE;
				cmd_index = 0;
				wrong_count = 0;
				memset(cmd_buffer, 0, sizeof(cmd_buffer));

			}
            else
            if (c == 'O') {
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_O;
                prev_correct_state = CMD_O;
            } else if (c == 'R') {
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_R;
                prev_correct_state = CMD_R;
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_O:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                cmd_state = CMD_START;
                prev_correct_state = CMD_START;
                break;
            }
            if (c == 'K') {
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_K;
                prev_correct_state = CMD_K;
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_K:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                cmd_state = CMD_O;
                prev_correct_state = CMD_O;
                break;
            }
            if (c == '#') {
                cmd_buffer[cmd_index++] = c;
                cmd_buffer[cmd_index] = '\0';

                // ✅ In ra lệnh hợp lệ
                char msg[40];
                sprintf(msg, "\r\nReceived Command: %s\r\n", cmd_buffer);
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

                // ✅ Đặt flag tương ứng
                command_flag = CMD_OK;

                // Reset hoàn toàn
                cmd_state = CMD_BEFORE;
                cmd_index = 0;
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_R:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                cmd_state = CMD_START;
                prev_correct_state = CMD_START;
                break;
            }
            if (c == 'S') {
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_S;
                prev_correct_state = CMD_S;
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_S:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                cmd_state = CMD_R;
                prev_correct_state = CMD_R;
                break;
            }
            if (c == 'T') {
                cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_T;
                prev_correct_state = CMD_T;
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_T:
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                }
                cmd_state = CMD_S;
                prev_correct_state = CMD_S;
                break;
            }
            if (c == '#') {
                cmd_buffer[cmd_index++] = c;
                cmd_buffer[cmd_index] = '\0';

                // ✅ In ra lệnh hợp lệ
                char msg[40];
                sprintf(msg, "\r\nReceived Command: %s\r\n", cmd_buffer);
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

                // ✅ Gán flag
                command_flag = CMD_RTS;

                // Reset hoàn toàn
                cmd_state = CMD_BEFORE;
                cmd_index = 0;
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            } else {
            	cmd_buffer[cmd_index++] = c;
                cmd_state = CMD_WRONG;
                wrong_count = 1;
            }
            break;

        case CMD_WRONG:
            if (c == 0x08 || c == 0x7F) {
                if (wrong_count > 0)
                	{
                		cmd_index--;
                	    cmd_buffer[cmd_index] = '\0';
                		wrong_count--;
                	}
                if (wrong_count == 0) {
                    cmd_state = prev_correct_state;
                }
            } else if (c == '#') {
                // ✅ Kết thúc command sai
                cmd_buffer[cmd_index++] = c;
                cmd_buffer[cmd_index] = '\0';

                char msg[50];
                sprintf(msg, "\r\nUnknown command: %s\r\n", cmd_buffer);
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

                // ✅ Reset toàn bộ để user gõ lại
                cmd_state = CMD_BEFORE;
                cmd_index = 0;
                wrong_count = 0;
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            } else {
            	cmd_buffer[cmd_index++] = c;
                wrong_count++;
            }
            break;
        }
    }

    index_buffer = 0;
    buffer_flag = 0;
    memset(buffer, 0, MAX_BUFFER_SIZE);
}




void uart_communication_fsm(void) {
    char str[50];
    static uint32_t last_tick = 0;
    uint32_t current_tick = HAL_GetTick();

    switch(uart_state) {
        case UART_IDLE:
            HAL_UART_Transmit(&huart2, (uint8_t*)"=== System READY ===\r\n", 23, 100);
            HAL_UART_Transmit(&huart2, (uint8_t*)"Type !RST# to request ADC value\r\n", 32, 100);
            uart_state = UART_WAIT_FOR_RTS;
            break;

        case UART_WAIT_FOR_RTS:
        	if(command_flag == CMD_OK) {
				HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n*** Communication COMPLETED! ***\r\n", 37, 100);
				command_flag = CMD_IDLE;
				uart_state = UART_WAIT_FOR_RTS;
//				HAL_UART_Transmit(&huart2, (uint8_t*)"Ready for new !RST# command\r\n\r\n", 30, 100);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			}
        	else
            if(command_flag == CMD_RTS)
            {
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nProcessing !RST# command...\r\n", 31, 100);

                // Reset command flag immediately
                command_flag = CMD_IDLE;

                if(HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK) {
                    adc_value = HAL_ADC_GetValue(&hadc1);
                    last_adc_value = adc_value;


                } else {
                    HAL_UART_Transmit(&huart2, (uint8_t*)"ADC read failed!\r\n", 18, 100);
                    adc_value = 0;
                }

                // Send ADC value
                sprintf(str, "!ADC=%lu#", adc_value);
                HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 1000);
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
                HAL_UART_Transmit(&huart2, (uint8_t*)"Type !OK# to stop or !RST# to update new ADC value\r\n", 52, 100);

                uart_state = UART_WAIT_FOR_OK;
                timeout_counter = current_tick;

                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
            }
            break;

        case UART_WAIT_FOR_OK:
            // CHO PHÉP !RST# MỚI HỦY COMMUNICATION CŨ
            if(command_flag == CMD_RTS) {
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nNew !RST# - Updating new ADC value\r\n", 38, 100);
                command_flag = CMD_RTS;
                uart_state = UART_WAIT_FOR_RTS;
            }
            else if(command_flag == CMD_OK) {
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n*** Communication COMPLETED! ***\r\n", 37, 100);
                command_flag = CMD_IDLE;
                uart_state = UART_WAIT_FOR_RTS;
//                HAL_UART_Transmit(&huart2, (uint8_t*)"Ready for new !RST# command\r\n\r\n", 30, 100);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            }
            else if((current_tick - timeout_counter) >= TIMEOUT_DURATION) {
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n*** TIMEOUT - Retransmitting... ***\r\n", 38, 100);
                sprintf(str, "!ADC=%lu#", last_adc_value);
                HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 1000);
                HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
                timeout_counter = current_tick;
            }
            break;
    }
}
