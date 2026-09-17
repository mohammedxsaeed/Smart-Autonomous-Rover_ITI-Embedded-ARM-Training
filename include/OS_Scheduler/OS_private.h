/*
 * OS_private.h
 * Created on: Aug 23, 2026
 * Author: Mohammed Saeed
 */
#ifndef OS_PRIVATE_H_
#define OS_PRIVATE_H_

// Task States
#define OS_TASK_DORMANT       0
#define OS_TASK_READY         1
#define OS_TASK_SUSPENDED     2

//Task Control Block
typedef struct {
    void (*TaskFunc)(void);   // Pointer to the task function
    u16 Periodicity;          // How often the task runs (in Ticks)
    u16 FirstDelay;           // When it should fire for the first time
    u8  State;                // State: DORMANT, READY
} Task_t;

#endif /* OS_PRIVATE_H_ */
