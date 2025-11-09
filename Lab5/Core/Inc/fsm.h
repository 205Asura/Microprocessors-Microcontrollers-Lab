#ifndef FSM_H
#define FSM_H

#define MAX_BUFFER_SIZE 30
#define TIMEOUT_DURATION 1000 // 3 seconds

#include <stdint.h>

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

// Command parser variables
typedef enum {
    CMD_IDLE,
    CMD_RTS,    // "!RST#"
    CMD_OK      // "!OK#"
} CommandType;

extern CommandType command_flag;
extern uint8_t command_data[MAX_BUFFER_SIZE];

// UART communication FSM variables
typedef enum {
    UART_IDLE,
    UART_WAIT_FOR_RTS,
    UART_WAIT_FOR_OK,
    UART_TIMEOUT,
	UART_COMM_COMPLETE
} UARTState;

typedef enum {
	CMD_BEFORE,
	CMD_START,
	CMD_WRONG,
	CMD_O,
	CMD_K,
	CMD_R,
	CMD_S,
	CMD_T,
//	CMD_END
} CMDState;

extern CMDState cmd_state;

extern UARTState uart_state;
extern uint32_t timeout_counter;
extern uint32_t adc_value;
extern uint32_t last_adc_value;


#endif
