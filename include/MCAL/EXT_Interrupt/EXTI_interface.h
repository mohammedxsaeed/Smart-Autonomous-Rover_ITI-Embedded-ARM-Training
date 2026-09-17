/*
 * EXTI_interface.h
 *
 *  Created on: Aug 20, 2026
 *      Author: Mohammed Saeed
 */

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

#include "LIB/STD_TYPES.h"

typedef enum
{
    EXTI_LineZero = 0,
    EXTI_LineOne,
    EXTI_LineTwo,
    EXTI_LineThree,
    EXTI_LineFour,
    EXTI_LineFive,
    EXTI_LineSix,
    EXTI_LineSeven,
    EXTI_LineEight,
    EXTI_LineNine,
    EXTI_LineTen,
    EXTI_LineEleven,
    EXTI_LineTwelve,
    EXTI_LineThirteen,
    EXTI_LineFourteen,
    EXTI_LineFifteen
} EXTI_Line;

typedef enum
{
    EXTI_Rising = 0,
    EXTI_Falling,
    EXTI_OnChange
} EXTI_TriggerMode;

typedef enum
{
    EXTI_PORTA = 0,
    EXTI_PORTB,
    EXTI_PORTC
} EXTI_PORT;


// Maps a specific EXTI line to a GPIO Port
void EXTI_SetInterruptPort(EXTI_Line Copy_uddtLine, EXTI_PORT Copy_uddtPort);

// Enables the external interrupt for a specific line
void EXTI_Enable(EXTI_Line Copy_uddtLine);

// Disables the external interrupt for a specific line
void EXTI_Disable(EXTI_Line Copy_uddtLine);

// Configures the trigger mode (Rising, Falling, or OnChange) for a specific line
void EXTI_SetTrigger(EXTI_Line Copy_uddtLine, EXTI_TriggerMode Copy_uddtmode);

// Sets the pending flag manually by software (Software Interrupt Trigger)
void EXTI_SetPendingFlag(EXTI_Line Copy_uddtLine);

// Clears the pending flag
void EXTI_ClearPendingFlag(EXTI_Line Copy_uddtLine);

// Reads the pending flag status (Returns 1 if interrupt is pending, 0 otherwise)
u8 EXTI_GetPendingFlag(EXTI_Line Copy_uddtLine);

// Sets the callback function to be executed when the interrupt fires for a specific line
void EXTI_SetCallBack(EXTI_Line Copy_uddtLine, void (*Copy_ptrToFunc)(void));

#endif /* EXTI_INTERFACE_H_ */
