#include "sched.h"
#include "main.h"
#include <stdio.h>

typedef struct sTask {
    // Pointer to the task (must be a 'void (void)' function)
    void (*pTask)(void);
    // Delay (ticks) until the function will (next) be run
    uint32_t Delay;
    // Interval (ticks) between subsequent runs.
    uint32_t deltaDelay;

    uint32_t Period;
    // Incremented (by scheduler) when task is due to execute
    uint8_t RunMe;
    // This is a hint to solve the question below.
    uint32_t TaskID;

    struct sTask* pNext;
    struct sTask* pPrev;

} sTask;

sTask* pHead = NULL;
sTask* pTail = NULL;

sTask SCH_tasks_G[SCH_MAX_TASKS];
unsigned char Error_code_G = 0;
unsigned char Last_error_code_G = 0;
uint32_t Error_tick_count_G = 0;


void SCH_Init(void) {
    unsigned char i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }

    // Reset the global error variable
    // - SCH_Delete_Task() will generate an error code,
    //   (because the task array is empty)
    Error_code_G = 0;

//    Timer_init();
//    Watchdog_init();
}

//void SCH_Update(void) {
//    unsigned char Index;
//    // NOTE: calculations are in *TICKS* (not milliseconds)
//    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
//    {
//        // Check if there is a task at this location
//        if (SCH_tasks_G[Index].pTask)
//        {
//            if (SCH_tasks_G[Index].Delay == 0)
//            {
//                // The task is due to run
//                // Inc. the 'RunMe' flag
//                SCH_tasks_G[Index].RunMe += 1;
//                if (SCH_tasks_G[Index].Period)
//                {
//                    // Schedule periodic tasks to run again
//                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
//                }
//            }
//            else
//            {
//                // Not yet ready to run: just decrement the delay
//                SCH_tasks_G[Index].Delay -= 1;
//                if (SCH_tasks_G[Index].Delay == 0)
//				{
//					// The task is due to run
//					// Inc. the 'RunMe' flag
//					SCH_tasks_G[Index].RunMe += 1;
//					if (SCH_tasks_G[Index].Period)
//					{
//						// Schedule periodic tasks to run again
//						SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
//					}
//				}
//            }
//        }
//    }
//}

// Con trỏ toàn cục

void SCH_Update(void) {
    // Chỉ kiểm tra task đầu tiên!
    if (pHead != NULL) {
        // Giảm delay của task đầu tiên


        // Nếu task đầu tiên đến hạn (Delay == 0)
        if (pHead->deltaDelay == 0)
        {
            // Đặt cờ RunMe
            pHead->RunMe += 1;
        }
        else
        {
        	pHead->deltaDelay--;
        	if (pHead->deltaDelay == 0)
        	{
				// Đặt cờ RunMe
				pHead->RunMe += 1;
			}
        }
    }
}

//-------------------------------------------------------------------------
// SCH_Add_Task() Causes a task (function) to be executed at regular intervals
// or after a user-defined delay
//-------------------------------------------------------------------------
//unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
//    unsigned char Index = 0;
//    // First find a gap in the array (if there is one)
//    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
//        Index++;
//    }
//
//    // Have we reached the end of the list?
//    if (Index == SCH_MAX_TASKS) {
//        // Task list is full
//        // Set the global error variable
//        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
//        // Also return an error code
//        return SCH_MAX_TASKS;
//    }
//
//    // If we’re here, there is a space in the task array
//    SCH_tasks_G[Index].pTask  = pFunction;
//    SCH_tasks_G[Index].Delay  = DELAY;
//    SCH_tasks_G[Index].Period = PERIOD;
//    SCH_tasks_G[Index].RunMe  = 0;
//
//    // Return position of task (to allow later deletion)
//    return Index;
//}

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
    unsigned char Index = 0;
    // First find a gap in the array (if there is one)
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }

    // Have we reached the end of the list?
    if (Index == SCH_MAX_TASKS) {
        // Task list is full
        // Set the global error variable
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        // Also return an error code
        return SCH_MAX_TASKS;
    }

    sTask* newTask = &SCH_tasks_G[Index];
    newTask->Delay = DELAY;
    newTask->pTask = pFunction;
    newTask->RunMe = 0;
    newTask->Period = PERIOD;
    newTask->TaskID= Index;
    newTask->pNext = NULL;
    // empty list
    if (pHead == NULL)
    {
    	printf("list empty\r\n");
    	pHead = newTask;
    	return Index;
    }

    // insert at first
    if (DELAY < pHead->deltaDelay)
    {
    	printf("insert at first\r\n");
    	pHead->deltaDelay -= DELAY;
    	newTask->pNext = pHead;
    	pHead = newTask;
    	return Index;
    }


    sTask* pCurrent = pHead;
    sTask* pPrev = NULL;

    int accumulativeDeltaDelay = pHead->deltaDelay;
    while (DELAY >= accumulativeDeltaDelay)
    {
    	pPrev = pCurrent;
    	pCurrent = pCurrent->pNext;
    	if (pCurrent != NULL)
    		accumulativeDeltaDelay += pCurrent->deltaDelay;
    	else
    	{
    		pPrev->pNext = newTask;
    		newTask->deltaDelay = DELAY - accumulativeDeltaDelay;
    		return Index;
    	}
    }
    // insert at middle
    if (pCurrent != NULL)
    {
    	pCurrent->deltaDelay -= DELAY;
    	newTask->pNext = pCurrent;
    	pPrev->pNext = newTask;
    	newTask->deltaDelay = DELAY - (accumulativeDeltaDelay - pCurrent->deltaDelay);
    	return Index;
    }


    return Index;
}

//void SCH_Dispatch_Tasks(void) {
//    unsigned char Index;
//    // Dispatches (runs) the next task (if one is ready)
//    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
//        if (SCH_tasks_G[Index].RunMe > 0) {
//            (*SCH_tasks_G[Index].pTask)();   // Run the task
//            SCH_tasks_G[Index].RunMe -= 1;   // Reset / reduce RunMe flag
//
//            // Periodic tasks will automatically run again
//            // - if this is a 'one shot' task, remove it from the array
//            if (SCH_tasks_G[Index].Period == 0) {
//                SCH_Delete_Task(Index);
//            }
//        }
//    }
//
//    // Report system status
//    SCH_Report_Status();
//
//    // The scheduler enters idle mode at this point
//    // SCH_Go_To_Sleep();
//}

void SCH_Dispatch_Tasks(void) {
    sTask* pTaskToRun;

    // Chạy tất cả các task ở đầu danh sách mà có RunMe = 1 (hoặc Delay=0)
    while (pHead != NULL && pHead->RunMe > 0) {

    	pHead->RunMe--;
        pTaskToRun = pHead; // Lấy task đầu tiên

        // 1. Chạy task
        (*pTaskToRun->pTask)();

        // 2. Xóa nó khỏi danh sách
        pHead = pTaskToRun->pNext;

        // 3. Nếu nó là periodic, thêm nó trở lại danh sách
        if (pTaskToRun->Period > 0) {
            // Dùng hàm SCH_Add_Task để nó tự chèn vào đúng vị trí
            SCH_Add_Task(pTaskToRun->pTask, pTaskToRun->Period, pTaskToRun->Period);
        }

        // (Nếu là one-shot thì chỉ cần xóa đi là xong)
        // (Bạn cũng cần logic để giải phóng pTaskToRun nếu dùng cấp phát động,
        // hoặc đưa nó vào "free list" nếu dùng mảng)
    }

    // ...
    // SCH_Report_Status();
    // SCH_Go_To_Sleep();
}

//====================================================================
unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX) {
    unsigned char Return_code;

    if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
        // No task at this location...
        // Set the global error variable
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

        // ...also return an error code
        Return_code = RETURN_ERROR;
    } else {
        Return_code = RETURN_NORMAL;
    }

    SCH_tasks_G[TASK_INDEX].pTask  = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay  = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe  = 0;

    return Return_code; // return status
}

void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS
    // ONLY APPLIES IF WE ARE REPORTING ERRORS
    // Check for a new error code
    if (Error_code_G != Last_error_code_G) {
        // Negative logic on LEDs assumed
        Error_port = 255 - Error_code_G;
        Last_error_code_G = Error_code_G;
        if (Error_code_G != 0) {
            Error_tick_count_G = 60000;
        } else {
            Error_tick_count_G = 0;
        }
    } else {
        if (Error_tick_count_G != 0) {
            if (--Error_tick_count_G == 0) {
                Error_code_G = 0; // Reset error code
            }
        }
    }
#endif
}




