/*
 * SYSTICK_interface.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SYSTICK_INTERFACE_H_
#define SYSTICK_INTERFACE_H_

// Initializes the SysTick timer based on configurations in config.h
void STK_Init(void);

// Synchronous Delay function
void STK_SetBusyWait(u32 Copy_u32Ticks);

// Asynchronous function: fires interrupt ONCE after ticks finish
void STK_SetIntervalSingle(u32 Copy_u32Ticks, void (*Copy_ptrToFunc)(void));

// Asynchronous function: fires interrupt PERIODICALLY every ticks
void STK_SetIntervalPeriodic(u32 Copy_u32Ticks, void (*Copy_ptrToFunc)(void));

// Stops the timer and disables the interrupt
void STK_StopTimer(void);
// Returns the number of ticks elapsed since the timer started
u32 STK_GetElapsedTime(void);

// Returns the number of ticks remaining until zero
u32 STK_GetRemainingTime(void);

void Delay_ms(u32 ms);

void Delay_us(u32 us);

#endif /* SYSTICK_INTERFACE_H_ 0 */
