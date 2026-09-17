/*
 * OS_interface.h
 * Created on: Aug 23, 2026
 * Author: Mohammed Saeed
 */
#ifndef OS_INTERFACE_H_
#define OS_INTERFACE_H_

/*
 * Creates a new task and changes its state from DORMANT to READY.
 * Copy_u8Priority: Priority (0 is highest priority)
 * Copy_u16Periodicity: Periodicity (in ticks/ms)
 * Copy_ptrTaskFunc: Function/Task (Address of the task )
 * Copy_u16FirstDelay: Offset before the first run
 */
void OS_CreateTask(u8 Copy_u8Priority, u16 Copy_u16Periodicity, void (*Copy_ptrTaskFunc)(void), u16 Copy_u16FirstDelay);

/*
 * Starts the OS by initializing SysTick and firing the Scheduler
 */
void Start_OS(void);

// Call this in main()'s while(1) loop, every iteration.
void OS_Update(void);
/*
 * Optional: Functions to control task states dynamically
 */
void OS_SuspendTask(u8 Copy_u8Priority);
void OS_ResumeTask(u8 Copy_u8Priority);

#endif /* OS_INTERFACE_H_ */
