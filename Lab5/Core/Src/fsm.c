#include "fsm.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

CommandType command_flag = CMD_IDLE;
uint8_t command_data[MAX_BUFFER_SIZE];

// UART communication FSM variables


UARTState uart_state = UART_IDLE;
uint32_t timeout_counter = 0;
uint32_t adc_value = 0;
uint32_t last_adc_value = 0;

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
// this version when i type !OK$ and delete $, and type # its unknown
void command_parser_fsm(void) {
    static uint8_t cmd_buffer[10];  // Giới hạn độ dài command ngắn (đủ cho !RST# hoặc !OK#)
    static uint8_t cmd_index = 0;
    static uint8_t in_command = 0;

    if (index_buffer == 0 || buffer_flag == 0) return;

    for (int i = 0; i < index_buffer; i++) {
        uint8_t c = buffer[i];

        // Bỏ qua \r, \n, khoảng trắng (noise từ terminal)
        if (c == '\r' || c == '\n' || c == ' ') continue;

        if (!in_command) {
            if (c == '!') {
                in_command = 1;
                cmd_index = 0;
                cmd_buffer[cmd_index++] = c;
            }
        } else {
            // Xử lý backspace (0x08 hoặc 0x7F)
            if (c == 0x08 || c == 0x7F) {
                if (cmd_index > 1) {  // Giữ lại '!' đầu tiên
                    cmd_index--;
                }
                continue;  // Không thêm backspace vào buffer
            }

            // Chỉ chấp nhận ký tự hợp lệ: A-Z, a-z, 0-9, '#'
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') || c == '#') {
                if (cmd_index >= sizeof(cmd_buffer) - 1) {
                    // Overflow: reset
                    in_command = 0;
                    cmd_index = 0;
                    continue;
                }
                cmd_buffer[cmd_index++] = c;

                if (c == '#') {
                    cmd_buffer[cmd_index] = '\0';

                    // So sánh case-sensitive (chỉ chấp nhận chữ hoa)
                    if (cmd_index == 5 &&
                        (cmd_buffer[1] == 'R') &&
                        (cmd_buffer[2] == 'S') &&
                        (cmd_buffer[3] == 'T')) {
                        command_flag = CMD_RTS;
                    } else if (cmd_index == 4 &&
                               (cmd_buffer[1] == 'O') &&
                               (cmd_buffer[2] == 'K')) {
                        command_flag = CMD_OK;
                    } else {
                        // Không hiển thị unknown, chỉ reset flag nếu cần
                    }

                    in_command = 0;
                    cmd_index = 0;
                    break;  // Thoát sau khi xử lý 1 command
                }
            } else {
                // Ký tự không hợp lệ: bỏ qua
            }
        }
    }

    index_buffer = 0;
    buffer_flag = 0;  // Reset flag
    memset(buffer, 0, MAX_BUFFER_SIZE);  // Xóa buffer để tránh rác
}

// This version is error when use backspace
//void command_parser_fsm(void) {
//    static uint8_t cmd_buffer[MAX_BUFFER_SIZE];
//    static uint8_t cmd_index = 0;
//    static uint8_t in_command = 0;
//
//    if (index_buffer == 0 || buffer_flag == 0) return;
//
//    for (int i = 0; i < index_buffer; i++) {
//        uint8_t c = buffer[i];
//
//        if (c == '\r' || c == '\n') continue; // Bỏ ký tự xuống dòng
//
//        if (!in_command) {
//            if (c == '!') {
//                in_command = 1;
//                cmd_index = 0;
//                cmd_buffer[cmd_index++] = c;
//            }
//        }
//        else {
//            cmd_buffer[cmd_index++] = c;
//
//            if (c == '#') {
//                cmd_buffer[cmd_index] = '\0';
//
//                char received_msg[60];
//                sprintf(received_msg, "\r\nCommand received: %s\r\n", cmd_buffer);
//                HAL_UART_Transmit(&huart2, (uint8_t*)received_msg, strlen(received_msg), 100);
//
//                if (strcmp((char*)cmd_buffer, "!RST#") == 0) {
//                    command_flag = CMD_RTS;
//                } else if (strcmp((char*)cmd_buffer, "!OK#") == 0) {
//                    command_flag = CMD_OK;
//                } else {
//                    char debug_msg[52];
//                    sprintf(debug_msg, "\r\nUnknown command: %s\r\n", cmd_buffer);
//                    HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), 100);
//                }
//
//                in_command = 0;
//                cmd_index = 0;
//            }
//
//            // Tràn buffer hoặc nhận ký tự không hợp lệ
//            else if (cmd_index >= MAX_BUFFER_SIZE - 1) {
//                in_command = 0;
//                cmd_index = 0;
//            }
//        }
//    }
//
//    index_buffer = 0;
////    buffer_flag = 0;
//}

// DEBUG VERSION
//void command_parser_fsm(void) {
//    static uint8_t cmd_buffer[MAX_BUFFER_SIZE];
//    static uint8_t cmd_index = 0;
//    static uint8_t in_command = 0;
//
//    if (index_buffer == 0 || buffer_flag == 0) return;
//
//    // DEBUG: Hiển thị buffer nhận được
//    char debug_msg[100];
//    sprintf(debug_msg, "\r\nDEBUG Buffer (%d bytes): [", index_buffer);
//    HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), 100);
//
//    for(int j = 0; j < index_buffer; j++) {
//        if(buffer[j] >= 32 && buffer[j] <= 126) {
//            sprintf(debug_msg, "%c", buffer[j]);
//        } else {
//            sprintf(debug_msg, "\\x%02X", buffer[j]);
//        }
//        HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), 100);
//    }
//    HAL_UART_Transmit(&huart2, (uint8_t*)"]\r\n", 3, 100);
//
//    for (int i = 0; i < index_buffer; i++) {
//        uint8_t c = buffer[i];
//
//        if (c == '\r' || c == '\n') continue;
//
//        if (!in_command) {
//            if (c == '!') {
//                in_command = 1;
//                cmd_index = 0;
//                cmd_buffer[cmd_index++] = c;
//            }
//        }
//        else {
//            cmd_buffer[cmd_index++] = c;
//
//            if (c == '#') {
//                cmd_buffer[cmd_index] = '\0';
//
//                char received_msg[70];
//                sprintf(received_msg, "\r\nCommand received: [%s] (length=%d)\r\n", cmd_buffer, cmd_index);
//                HAL_UART_Transmit(&huart2, (uint8_t*)received_msg, strlen(received_msg), 100);
//
//                // So sánh CHÍNH XÁC với độ dài
//                if (cmd_index == 5 && strcmp((char*)cmd_buffer, "!RST#") == 0) {
//                    command_flag = CMD_RTS;
//                    HAL_UART_Transmit(&huart2, (uint8_t*)"-> Valid !RST#\r\n", 16, 100);
//                } else if (cmd_index == 4 && strcmp((char*)cmd_buffer, "!OK#") == 0) {
//                    command_flag = CMD_OK;
//                    HAL_UART_Transmit(&huart2, (uint8_t*)"-> Valid !OK#\r\n", 15, 100);
//                } else {
//                    char debug_msg[90];
//                    sprintf(debug_msg, "-> Unknown command: [%s] (expected length: !RST#=5, !OK#=4)\r\n", cmd_buffer);
//                    HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), 100);
//                }
//
//                in_command = 0;
//                cmd_index = 0;
//                break; // Quan trọng: thoát sau khi xử lý command
//            }
//
//            else if (cmd_index >= MAX_BUFFER_SIZE - 1) {
//                in_command = 0;
//                cmd_index = 0;
//            }
//        }
//    }
//
//    index_buffer = 0;
//    buffer_flag = 0;  // Bỏ comment dòng này
//}


// This version is good but !\n\nOK# is still valid
//void command_parser_fsm(void) {
//    static uint8_t cmd[10];
//    static uint8_t idx = 0;
//    static uint8_t collecting = 0;
//
//    if (!buffer_flag) return;
//
//    for (int i = 0; i < index_buffer; i++) {
//        uint8_t c = buffer[i];
//
//        if (!collecting && c == '!') {
//            collecting = 1;
//            idx = 0;
//            cmd[idx++] = c;
//        }
//        else if (collecting) {
//            // XỬ LÝ BACKSPACE (0x08 hoặc 0x7F)
//            if (c == 0x08 || c == 0x7F) {
//                if (idx > 1) { // Giữ lại ký tự '!' đầu tiên
//                    idx--;
//                }
//            }
//            // CHỈ chấp nhận ký tự hợp lệ cho command
//            else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
//                     (c >= '0' && c <= '9') || c == '#') {
//                cmd[idx++] = c;
//
//                if (c == '#') {
//                    cmd[idx] = '\0';
//
//                    // HIỆN COMMAND
//                    HAL_UART_Transmit(&huart2, (uint8_t*)"Command: ", 9, 100);
//                    HAL_UART_Transmit(&huart2, cmd, idx, 100);
//                    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
//
//                    // SO SÁNH (case-insensitive)
//                    if (idx == 5 &&
//                        (cmd[1]=='R' || cmd[1]=='r') &&
//                        (cmd[2]=='S' || cmd[2]=='s') &&
//                        (cmd[3]=='T' || cmd[3]=='t')) {
//                        command_flag = CMD_RTS;
//                    }
//                    else if (idx == 4 &&
//                             (cmd[1]=='O' || cmd[1]=='o') &&
//                             (cmd[2]=='K' || cmd[2]=='k')) {
//                        command_flag = CMD_OK;
//                    }
//                    else {
//                        HAL_UART_Transmit(&huart2, (uint8_t*)"Unknown\r\n", 9, 100);
//                    }
//
//                    collecting = idx = 0;
//                    index_buffer = buffer_flag = 0;
//                    memset(buffer, 0, MAX_BUFFER_SIZE);
//                    return;
//                }
//                else if (idx >= 9) {
//                    collecting = idx = 0;
//                }
//            }
//            // Bỏ qua các ký tự khác (khoảng trắng, ký tự đặc biệt, v.v.)
//        }
//    }
//
//    index_buffer = buffer_flag = 0;
//    memset(buffer, 0, MAX_BUFFER_SIZE);
//}



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
