#include "sched.h"
#include "main.h"
#include <stdio.h>

#define DEFAULT_MODE
//#define LINKEDLIST_MODE

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



// Con trỏ toàn cục



//-------------------------------------------------------------------------
// SCH_Add_Task() Causes a task (function) to be executed at regular intervals
// or after a user-defined delay
//-------------------------------------------------------------------------
#ifdef DEFAULT_MODE
void SCH_Update(void) {
    unsigned char Index;
    // NOTE: calculations are in *TICKS* (not milliseconds)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        // Check if there is a task at this location
        if (SCH_tasks_G[Index].pTask)
        {
            if (SCH_tasks_G[Index].Delay == 0)
            {
                // The task is due to run
                // Inc. the 'RunMe' flag
                SCH_tasks_G[Index].RunMe += 1;
                if (SCH_tasks_G[Index].Period)
                {
                    // Schedule periodic tasks to run again
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            }
            else
            {
                // Not yet ready to run: just decrement the delay
                SCH_tasks_G[Index].Delay -= 1;
                if (SCH_tasks_G[Index].Delay == 0)
				{
					// The task is due to run
					// Inc. the 'RunMe' flag
					SCH_tasks_G[Index].RunMe += 1;
					if (SCH_tasks_G[Index].Period)
					{
						// Schedule periodic tasks to run again
						SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
					}
				}
            }
        }
    }
}
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

    // If we’re here, there is a space in the task array
    SCH_tasks_G[Index].pTask  = pFunction;
    SCH_tasks_G[Index].Delay  = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe  = 0;

    // Return position of task (to allow later deletion)
    return Index;
}

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

void SCH_Dispatch_Tasks(void) {
    unsigned char Index;
    // Dispatches (runs) the next task (if one is ready)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].RunMe > 0) {
            (*SCH_tasks_G[Index].pTask)();   // Run the task
            SCH_tasks_G[Index].RunMe -= 1;   // Reset / reduce RunMe flag

            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (SCH_tasks_G[Index].Period == 0) {
                SCH_Delete_Task(Index);
            }
        }
    }

    // Report system status
    SCH_Report_Status();

    // The scheduler enters idle mode at this point
    // SCH_Go_To_Sleep();
}
#endif

#ifdef LINKEDLIST_MODE

void SCH_Update(void) {
    // Chỉ kiểm tra task đầu tiên!
    if (pHead != NULL) {
        // Giảm delay của task đầu tiên
        if (pHead->deltaDelay > 0) {
            pHead->deltaDelay--;
        }

        // Nếu task đầu tiên đến hạn (Delay == 0)
        if (pHead->deltaDelay == 0) {
            // Đặt cờ RunMe
            pHead->RunMe += 1;
        }
    }
}
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
    unsigned char Index = 0;

    // 1. Tìm một slot trống trong mảng
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }

    if (Index == SCH_MAX_TASKS) {
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        printf("too many task\r\n");
        return SCH_MAX_TASKS;
    }

    // 2. Khởi tạo task mới
    sTask* newTask = &SCH_tasks_G[Index];
    newTask->pTask = pFunction;
    newTask->Delay = DELAY; // Lưu delay tuyệt đối để tham khảo (không bắt buộc)
    newTask->Period = PERIOD;
    newTask->RunMe = 0;
    newTask->TaskID = Index;
    newTask->pNext = NULL;
    newTask->pPrev = NULL;

    // --- Logic chèn vào danh sách liên kết đôi ---

    // Trường hợp 1: Danh sách rỗng
    if (pHead == NULL) {
        newTask->deltaDelay = DELAY;
        pHead = newTask;
        pTail = newTask;
        return Index;
    }

    // Trường hợp 2: Chèn vào danh sách không rỗng
    sTask* pCurrent = pHead;
    uint32_t accumulativeDelay = 0; // Thời gian delay tích lũy

    while (pCurrent != NULL) {
        accumulativeDelay += pCurrent->deltaDelay;

        if (DELAY < accumulativeDelay) {
            // Đã tìm thấy vị trí: Chèn newTask vào *trước* pCurrent

            // Tính toán lại deltaDelay
            uint32_t prevAccumDelay = accumulativeDelay - pCurrent->deltaDelay;
            newTask->deltaDelay = DELAY - prevAccumDelay;
            pCurrent->deltaDelay = accumulativeDelay - DELAY; // (hoặc pCurrent->deltaDelay - newTask->deltaDelay)

            // Cập nhật con trỏ của newTask
            newTask->pNext = pCurrent;
            newTask->pPrev = pCurrent->pPrev;

            // Cập nhật con trỏ của các task lân cận
            if (pCurrent->pPrev != NULL) {
                // Chèn vào giữa
                pCurrent->pPrev->pNext = newTask;
            } else {
                // Chèn vào đầu danh sách (pHead)
                pHead = newTask;
            }
            pCurrent->pPrev = newTask;

            return Index;
        }

        // Nếu chưa tìm thấy, đi tiếp
        if (pCurrent->pNext == NULL) {
            break; // Đã đến cuối danh sách
        }
        pCurrent = pCurrent->pNext;
    }

    // Trường hợp 3: Chèn vào cuối danh sách
    // (pCurrent lúc này là pTail)
    newTask->deltaDelay = DELAY - accumulativeDelay;
    newTask->pNext = NULL;
    newTask->pPrev = pCurrent;
    pCurrent->pNext = newTask;
    pTail = newTask; // Cập nhật pTail

    return Index;
}

void SCH_Dispatch_Tasks(void) {
    // Chạy tất cả các task ở đầu danh sách mà có RunMe > 0
    // (Xử lý trường hợp nhiều task chạy cùng 1 tick)
    while (pHead != NULL && pHead->RunMe > 0) {

        sTask* pTaskToRun = pHead; // Lấy task đầu tiên

        // Lưu lại thông tin task TRƯỚC KHI xóa
        void (*pFunction)(void) = pTaskToRun->pTask;
        uint32_t Period = pTaskToRun->Period;
        uint8_t TaskID = pTaskToRun->TaskID;

        // 1. Chạy task
        (*pFunction)();

        // 2. Xóa task khỏi danh sách (hàm này sẽ tự cập nhật pHead)
        SCH_Delete_Task(TaskID);

        // 3. Nếu nó là periodic, thêm nó trở lại danh sách
        if (Period > 0) {
            // Dùng hàm SCH_Add_Task để nó tự chèn vào đúng vị trí
            SCH_Add_Task(pFunction, Period, Period);
        }
    }

    // ...
    // SCH_Report_Status();
    // SCH_Go_To_Sleep();
}

//====================================================================

unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX) {
    if (TASK_INDEX >= SCH_MAX_TASKS || SCH_tasks_G[TASK_INDEX].pTask == 0) {
        // Task không tồn tại
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        return RETURN_ERROR;
    }

    sTask* pTaskToDelete = &SCH_tasks_G[TASK_INDEX];

    // 1. Cập nhật deltaDelay của task *phía sau* (nếu có)
    // Task phía sau sẽ gánh thêm phần deltaDelay của task bị xóa
    if (pTaskToDelete->pNext != NULL) {
        pTaskToDelete->pNext->deltaDelay += pTaskToDelete->deltaDelay;
    }

    // 2. "Vá" lại danh sách liên kết
    if (pTaskToDelete->pPrev != NULL) {
        // Task này ở giữa hoặc cuối
        pTaskToDelete->pPrev->pNext = pTaskToDelete->pNext;
    } else {
        // Task này là pHead
        pHead = pTaskToDelete->pNext;
    }

    if (pTaskToDelete->pNext != NULL) {
        // Task này ở giữa hoặc đầu
        pTaskToDelete->pNext->pPrev = pTaskToDelete->pPrev;
    } else {
        // Task này là pTail
        pTail = pTaskToDelete->pPrev;
    }

    // 3. Xóa dữ liệu task trong mảng (làm cho slot này "trống")
    pTaskToDelete->pTask = 0x0000;
    pTaskToDelete->Delay = 0;
    pTaskToDelete->Period = 0;
    pTaskToDelete->RunMe = 0;
    pTaskToDelete->pNext = NULL;
    pTaskToDelete->pPrev = NULL;
    // pTaskToDelete->TaskID = 0; // Không cần thiết lắm nhưng cho sạch

    return RETURN_NORMAL; // return status
}
#endif

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




