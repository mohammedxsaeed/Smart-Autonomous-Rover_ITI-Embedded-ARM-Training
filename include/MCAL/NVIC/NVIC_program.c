/*
 * NVIC_program.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"

#include "MCAL/NVIC/NVIC_private.h"
#include "MCAL/NVIC/NVIC_interface.h"

void NVIC_EnableInterrupt(u8 Copy_u8IRQn)
{
    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        // Writing 1 enables the interrupt, writing 0 has no effect
        NVIC->ISER[Local_u8RegIdx] = (1 << Local_u8BitIdx);
    }
}

void NVIC_DisableInterrupt(u8 Copy_u8IRQn)
{
    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        // Writing 1 disables the interrupt, writing 0 has no effect
        NVIC->ICER[Local_u8RegIdx] = (1 << Local_u8BitIdx);
    }
}

void NVIC_SetPendingFlag(u8 Copy_u8IRQn)
{
    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        NVIC->ISPR[Local_u8RegIdx] = (1 << Local_u8BitIdx);
    }
}

void NVIC_ClearPendingFlag(u8 Copy_u8IRQn)
{
    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        NVIC->ICPR[Local_u8RegIdx] = (1 << Local_u8BitIdx);
    }
}

u8 NVIC_GetActiveFlag(u8 Copy_u8IRQn)
{
    u8 Local_u8Active = 0;

    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        // Use GET_BIT macro from UTILS.h to read the specific bit
        Local_u8Active = GET_BIT(NVIC->IABR[Local_u8RegIdx], Local_u8BitIdx);
    }

    return Local_u8Active;
}

u8 NVIC_GetPendingFlag(u8 Copy_u8IRQn)
{
    u8 Local_u8Pending = 0;

    if (Copy_u8IRQn <= 239)
    {
        u8 Local_u8RegIdx = Copy_u8IRQn / 32;
        u8 Local_u8BitIdx = Copy_u8IRQn % 32;

        Local_u8Pending = GET_BIT(NVIC->ISPR[Local_u8RegIdx], Local_u8BitIdx);
    }

    return Local_u8Pending;
}

void NVIC_SetPriority(u8 Copy_u8IRQn, u8 Copy_u8Priority)
{
    if (Copy_u8IRQn <= 239)
    {
        // Shift left by 4 because priority bits are in the upper 4 bits of each byte
        NVIC->IPR[Copy_u8IRQn] = (Copy_u8Priority << 4);
    }
}

void NVIC_SetPriorityGrouping(NVIC_GroupMode_t Copy_GroupMode)
{
    // Write VECTKEY password and shift the group mode to bit 8
    SCB_AIRCR = VECTKEY | (Copy_GroupMode << 8);
}

u8 NVIC_GetPriority(u8 Copy_u8IRQn)
{
    u8 Local_u8Priority = 0;

    if (Copy_u8IRQn <= 239)
    {
        // Shift right by 4 to extract the actual priority value
        Local_u8Priority = (NVIC->IPR[Copy_u8IRQn] >> 4);
    }

    return Local_u8Priority;
}
