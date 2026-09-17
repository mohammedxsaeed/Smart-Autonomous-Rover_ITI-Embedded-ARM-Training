/*
 * SYSTICK_program.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/SYSTICK/SYSTICK_private.h"
#include "MCAL/SYSTICK/SYSTICK_config.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"


// Global pointer to function to hold the callback address
static void (*STK_CallBack)(void) = NULL;

// Flag to indicate single or periodic interval (0 for Single, 1 for Periodic)
static u8 STK_u8ModeOfInterval = STK_MODE;

void STK_Init(void)
{
    // Disable SysTick, Disable Interrupt, Choose AHB/8 as clock source
    STK->CTRL = 0x00000000;
}

void STK_SetBusyWait(u32 Copy_u32Ticks)
{
    // 1. Load the ticks into the LOAD register
    STK->LOAD = Copy_u32Ticks;

    // 2. Clear the VAL register
    STK->VAL = 0;

    // 3. Enable the timer (ENABLE=1, TICKINT=0, CLKSOURCE=0 for AHB/8)
    STK->CTRL = 0x01;

    // 4. Wait until the COUNTFLAG (bit 16) is set
    while (!GET_BIT(STK->CTRL, 16));

    // 5. Stop the timer and clear registers
    STK->CTRL = 0;
    STK->LOAD = 0;
    STK->VAL = 0;
}

void STK_SetIntervalSingle(u32 Copy_u32Ticks, void (*Copy_ptrToFunc)(void))
{
    // Save the callback function and set mode to Single (0)
    if (Copy_ptrToFunc != NULL)
    {
        STK_CallBack = Copy_ptrToFunc;
        STK_u8ModeOfInterval = SINGLE_MODE;

        // 1. Load the ticks
        STK->LOAD = Copy_u32Ticks;

        // 2. Clear the VAL register
        STK->VAL = 0;

        // 3. Enable timer and interrupt (ENABLE=1, TICKINT=1, CLKSOURCE=0 for AHB/8)
        STK->CTRL = 0x03;
    }
}

void STK_SetIntervalPeriodic(u32 Copy_u32Ticks, void (*Copy_ptrToFunc)(void))
{
    // Save the callback function and set mode to Periodic
    if (Copy_ptrToFunc != NULL)
    {
        STK_CallBack = Copy_ptrToFunc;
        STK_u8ModeOfInterval = PERIODIC_MODE;

        // 1. Load the ticks
        STK->LOAD = Copy_u32Ticks;

        // 2. Clear the VAL register
        STK->VAL = 0;

        // 3. Enable timer and interrupt (ENABLE=1, TICKINT=1, CLKSOURCE=0 for AHB/8)
        STK->CTRL = 0x03;
    }
}

void STK_StopTimer(void)
{
    // Disable the timer and interrupt
    STK->CTRL = 0;
    STK->LOAD = 0;
    STK->VAL = 0;
}

u32 STK_GetElapsedTime(void)
{
    // Elapsed time = LOAD - VAL
    return (STK->LOAD - STK->VAL);
}

u32 STK_GetRemainingTime(void)
{
    // Remaining time is the current value in VAL register
    return (STK->VAL);
}

void Delay_us(u32 us)
{
    // Assuming Clock is 16MHz and AHB/8 is used -> Timer Clock = 2MHz
    // 1 us = 2 Ticks
    STK_SetBusyWait(us * 2);
}

void Delay_ms(u32 ms)
{
    for(u32 i = 0; i < ms; i++)
    {
        Delay_us(1000);
    }
}

//****************Interrupt Service Routine (ISR)**********************
void SysTick_Handler(void)
{
    u8 CLR_FLAG = 0;

    // If mode is Single, stop the timer so it doesn't fire again
    if (STK_u8ModeOfInterval == SINGLE_MODE)
    {
        STK->CTRL = 0;
        STK->LOAD = 0;
        STK->VAL = 0;
    }

    // Execute the callback function
    if (STK_CallBack != NULL)
    {
        STK_CallBack();
    }

    // Clear the COUNTFLAG by reading the CTRL register
    CLR_FLAG = GET_BIT(STK->CTRL, 16);
}
