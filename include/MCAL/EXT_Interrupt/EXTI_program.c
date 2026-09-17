/*
 * EXTI_program.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/EXT_Interrupt/EXTI_private.h"
#include "MCAL/EXT_Interrupt/EXTI_config.h"
#include "MCAL/EXT_Interrupt/EXTI_interface.h"

// Array of pointers to functions to hold the callbacks for the 16 lines
static void (*EXTI_CallBack[16])(void) = {NULL};

void EXTI_SetInterruptPort(EXTI_Line Copy_uddtLine, EXTI_PORT Copy_uddtPort)
{
    // Each register handles 4 lines, 4 bits per line
    u8 RegId = Copy_uddtLine / 4;
    u8 SHF_BIT = (Copy_uddtLine % 4) * 4;

    // Clear the 4 bits for the specific line
    SYSCFG->EXTICR[RegId] &= ~(0b1111 << SHF_BIT);

    // Set the new port value (0 for PA, 1 for PB, 2 for PC)
    SYSCFG->EXTICR[RegId] |= (Copy_uddtPort << SHF_BIT);
}

void EXTI_Enable(EXTI_Line Copy_uddtLine)
{
    SET_BIT(EXTI->IMR, Copy_uddtLine);
}

void EXTI_Disable(EXTI_Line Copy_uddtLine)
{
    CLR_BIT(EXTI->IMR, Copy_uddtLine);
}

void EXTI_SetTrigger(EXTI_Line Copy_uddtLine, EXTI_TriggerMode Copy_uddtmode)
{
    // Clear both triggers first to ensure a clean state
    CLR_BIT(EXTI->RTSR, Copy_uddtLine);
    CLR_BIT(EXTI->FTSR, Copy_uddtLine);

    switch (Copy_uddtmode)
    {
        case EXTI_Rising:
            SET_BIT(EXTI->RTSR, Copy_uddtLine);
            break;
        case EXTI_Falling:
            SET_BIT(EXTI->FTSR, Copy_uddtLine);
            break;
        case EXTI_OnChange:
            SET_BIT(EXTI->RTSR, Copy_uddtLine);
            SET_BIT(EXTI->FTSR, Copy_uddtLine);
            break;
    }
}

void EXTI_SetPendingFlag(EXTI_Line Copy_uddtLine)
{
    SET_BIT(EXTI->SWIER, Copy_uddtLine);
}

void EXTI_ClearPendingFlag(EXTI_Line Copy_uddtLine)
{
    //  the pending flag is cleared by SET 1.
	EXTI->PR = (1 << Copy_uddtLine);
}

u8 EXTI_GetPendingFlag(EXTI_Line Copy_uddtLine)
{
    return GET_BIT(EXTI->PR, Copy_uddtLine);
}

void EXTI_SetCallBack(EXTI_Line Copy_uddtLine, void (*Copy_ptrToFunc)(void))
{
    if (Copy_ptrToFunc != NULL)
    {
        EXTI_CallBack[Copy_uddtLine] = Copy_ptrToFunc;
    }
}
/***********************************************************/
// Interrupt Service Routines (ISRs)
/**********************************************************/
void EXTI0_IRQHandler(void)
{
    EXTI_ClearPendingFlag(EXTI_LineZero);
    if (EXTI_CallBack[0] != NULL) EXTI_CallBack[0]();
}

void EXTI1_IRQHandler(void)
{
    EXTI_ClearPendingFlag(EXTI_LineOne);
    if (EXTI_CallBack[1] != NULL) EXTI_CallBack[1]();
}

void EXTI2_IRQHandler(void)
{
    EXTI_ClearPendingFlag(EXTI_LineTwo);
    if (EXTI_CallBack[2] != NULL) EXTI_CallBack[2]();
}

void EXTI3_IRQHandler(void)
{
    EXTI_ClearPendingFlag(EXTI_LineThree);
    if (EXTI_CallBack[3] != NULL) EXTI_CallBack[3]();
}

void EXTI4_IRQHandler(void)
{
    EXTI_ClearPendingFlag(EXTI_LineFour);
    if (EXTI_CallBack[4] != NULL) EXTI_CallBack[4]();
}

void EXTI9_5_IRQHandler(void)
{
    // Check which line triggered the interrupt
    if (EXTI_GetPendingFlag(EXTI_LineFive) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineFive);
        if (EXTI_CallBack[5] != NULL) EXTI_CallBack[5]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineSix) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineSix);
        if (EXTI_CallBack[6] != NULL) EXTI_CallBack[6]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineSeven) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineSeven);
        if (EXTI_CallBack[7] != NULL) EXTI_CallBack[7]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineEight) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineEight);
        if (EXTI_CallBack[8] != NULL) EXTI_CallBack[8]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineNine) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineNine);
        if (EXTI_CallBack[9] != NULL) EXTI_CallBack[9]();
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetPendingFlag(EXTI_LineTen) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineTen);
        if (EXTI_CallBack[10] != NULL) EXTI_CallBack[10]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineEleven) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineEleven);
        if (EXTI_CallBack[11] != NULL) EXTI_CallBack[11]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineTwelve) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineTwelve);
        if (EXTI_CallBack[12] != NULL) EXTI_CallBack[12]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineThirteen) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineThirteen);
        if (EXTI_CallBack[13] != NULL) EXTI_CallBack[13]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineFourteen) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineFourteen);
        if (EXTI_CallBack[14] != NULL) EXTI_CallBack[14]();
    }
    if (EXTI_GetPendingFlag(EXTI_LineFifteen) == 1)
    {
        EXTI_ClearPendingFlag(EXTI_LineFifteen);
        if (EXTI_CallBack[15] != NULL) EXTI_CallBack[15]();
    }
}
