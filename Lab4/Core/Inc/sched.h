#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>

// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS   10
#define NO_TASK_ID      0


void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX);
void SCH_Report_Status(void);

extern unsigned char Error_code_G;
extern unsigned char Last_error_code_G;
extern uint32_t Error_tick_count_G;

#define ERROR_SCH_TOO_MANY_TASKS                        1
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK              2
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER 3
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START      4
#define ERROR_SCH_LOST_SLAVE                            5
#define ERROR_SCH_CAN_BUS_ERROR                         6
#define ERROR_I2C_WRITE_BYTE_AT24C64                    7
#define ERROR_SCH_CANNOT_DELETE_TASK                    8
#define RETURN_ERROR									9
#define RETURN_NORMAL									10

#endif
