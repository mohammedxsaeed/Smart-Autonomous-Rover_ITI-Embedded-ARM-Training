/*
 * OS_program.c
 * Created on: Aug 23, 2026
 * Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/SYSTICK/SYSTICK_private.h"
#include "MCAL/SYSTICK/SYSTICK_config.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "OS_Scheduler/OS_private.h"
#include "OS_Scheduler/OS_config.h"
#include "OS_Scheduler/OS_interface.h"

/* Array of Tasks (Priority is the Index) */
static Task_t OS_Tasks[OS_NUMBER_OF_TASKS] = { 0 };
static volatile u8 OS_u8TickFlag = 0;

static void Scheduler(void);

static void OS_TickISR(void)
{
    OS_u8TickFlag = 1;
}

void OS_CreateTask(u8 Copy_u8Priority, u16 Copy_u16Periodicity,
		void (*Copy_ptrTaskFunc)(void), u16 Copy_u16FirstDelay) {
	if ((Copy_u8Priority < OS_NUMBER_OF_TASKS) && (Copy_ptrTaskFunc != NULL)) {
		OS_Tasks[Copy_u8Priority].TaskFunc = Copy_ptrTaskFunc;
		OS_Tasks[Copy_u8Priority].Periodicity = Copy_u16Periodicity;
		OS_Tasks[Copy_u8Priority].FirstDelay = Copy_u16FirstDelay;
		OS_Tasks[Copy_u8Priority].State = OS_TASK_READY;
	}
}

void Start_OS(void) {
	u32 Local_u32Ticks = 0;

	STK_Init();

	if (STK_CLK_SRC == AHB) {
		/* 16 MHz Clock -> 1 ms = 16,000 Ticks */
		Local_u32Ticks = OS_TICK_TIME_MS * 16000;
	} else {
		/* AHB/8 -> 2 MHz Clock -> 1 ms = 2,000 Ticks */
		Local_u32Ticks = OS_TICK_TIME_MS * 2000;
	}

	STK_SetIntervalPeriodic(Local_u32Ticks, OS_TickISR);
}

void OS_Update(void) {
	if (OS_u8TickFlag) {
		OS_u8TickFlag = 0;
		Scheduler();
	}
}

static void Scheduler(void) {
	u8 Local_u8TaskCounter;

	for (Local_u8TaskCounter = 0; Local_u8TaskCounter < OS_NUMBER_OF_TASKS;
			Local_u8TaskCounter++) {
		if (OS_Tasks[Local_u8TaskCounter].State == OS_TASK_READY) {
			if (OS_Tasks[Local_u8TaskCounter].FirstDelay == 0) {
				/* 1. Execute Task */
				if (OS_Tasks[Local_u8TaskCounter].TaskFunc != NULL) {
					OS_Tasks[Local_u8TaskCounter].TaskFunc();
				}

				/* 2. Reload FirstDelay */
				OS_Tasks[Local_u8TaskCounter].FirstDelay = OS_Tasks[Local_u8TaskCounter].Periodicity - 1;
			} else {
				/* Decrement delay */
				OS_Tasks[Local_u8TaskCounter].FirstDelay--;
			}
		}
	}
}

void OS_SuspendTask(u8 Copy_u8Priority) {
	if (Copy_u8Priority < OS_NUMBER_OF_TASKS) {
		OS_Tasks[Copy_u8Priority].State = OS_TASK_SUSPENDED;
	}
}

void OS_ResumeTask(u8 Copy_u8Priority) {
	if (Copy_u8Priority < OS_NUMBER_OF_TASKS) {
		OS_Tasks[Copy_u8Priority].State = OS_TASK_READY;
	}
}
