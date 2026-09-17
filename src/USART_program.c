/*
 * USART_program.c
 *
 *  Created on: Aug 30, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/USART/USART_private.h"
#include "MCAL/USART/USART_config.h"
#include "MCAL/USART/USART_interface.h"

// Helper function to get the base address of the selected USART instance
static USART_Reg_t* USART_GetBaseAddress(USART_Instance_t Copy_Instance)
{
    switch(Copy_Instance)
    {
        case USART_1: return USART1;
        case USART_2: return USART2;
        case USART_6: return USART6;
        default:      return 0;
    }
}

void USART_Init(USART_Instance_t Copy_Instance)
{
    USART_Reg_t* Local_pUSART = USART_GetBaseAddress(Copy_Instance);

    // Guard against invalid or null pointers
    if (Local_pUSART == 0) return;

    GPIO_InitConfig_t USART_Pins = {
        .Mode  = AF,
        .OType = PUSH_PULL,
        .Speed = SPEED_HIGH,
        .Pupd  = NONE
    };

    u32 Local_u32ClockFreq = 0;
    u32 Local_u32BaudRate = 0;

#if USART1_STATUS == 1
    if (Copy_Instance == USART_1)
    {
        Local_u32ClockFreq = USART_APB2_CLOCK;
        Local_u32BaudRate  = USART1_BAUD_RATE;
        USART_Pins.AF      = USART1_AF;

        USART_Pins.Pin = USART1_TX_PIN;
        GPIO_Init(USART1_TX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART1_TX_PORT, USART1_TX_PIN, USART1_AF);

        USART_Pins.Pin = USART1_RX_PIN;
        GPIO_Init(USART1_RX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART1_RX_PORT, USART1_RX_PIN, USART1_AF);
    }
#endif

#if USART2_STATUS == 1
    if (Copy_Instance == USART_2)
    {
        Local_u32ClockFreq = USART_APB1_CLOCK;
        Local_u32BaudRate  = USART2_BAUD_RATE;
        USART_Pins.AF      = USART2_AF;

        USART_Pins.Pin = USART2_TX_PIN;
        GPIO_Init(USART2_TX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART2_TX_PORT, USART2_TX_PIN, USART2_AF);

        USART_Pins.Pin = USART2_RX_PIN;
        GPIO_Init(USART2_RX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART2_RX_PORT, USART2_RX_PIN, USART2_AF);
    }
#endif

#if USART6_STATUS == 1
    if (Copy_Instance == USART_6)
    {
        Local_u32ClockFreq = USART_APB2_CLOCK;
        Local_u32BaudRate  = USART6_BAUD_RATE;
        USART_Pins.AF      = USART6_AF;

        USART_Pins.Pin = USART6_TX_PIN;
        GPIO_Init(USART6_TX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART6_TX_PORT, USART6_TX_PIN, USART6_AF);

        USART_Pins.Pin = USART6_RX_PIN;
        GPIO_Init(USART6_RX_PORT, &USART_Pins);
        GPIO_SetAlternativeConfig(USART6_RX_PORT, USART6_RX_PIN, USART6_AF);
    }
#endif

    // Guard: Return if the instance was disabled in config
    if (Local_u32BaudRate == 0) return;

    // Enable Normal Mode
    Local_pUSART->CR1 = 0;
    Local_pUSART->CR2 = 0;

    // SET Baud Rate and UBRR
    Local_pUSART->BRR = (Local_u32ClockFreq + (Local_u32BaudRate / 2)) / Local_u32BaudRate;

    // Enable USART Sender & Receiver
    SET_BIT(Local_pUSART->CR1, USART_CR1_TE);
    SET_BIT(Local_pUSART->CR1, USART_CR1_RE);
    SET_BIT(Local_pUSART->CR1, USART_CR1_UE);
}

void USART_SendData(USART_Instance_t Instance, u8 Copy_u8Data)
{
    USART_Reg_t* Local_pUSART = USART_GetBaseAddress(Instance);

    while(!GET_BIT(Local_pUSART->SR, USART_SR_TXE));
    Local_pUSART->DR = Copy_u8Data;
}

u8 USART_Rx(USART_Instance_t Copy_Instance)
{
    USART_Reg_t* Local_pUSART = USART_GetBaseAddress(Copy_Instance);

    while(!GET_BIT(Local_pUSART->SR, USART_SR_RXNE));
    return (u8)(Local_pUSART->DR & 0xFF);
}

void USART_SendString(USART_Instance_t Instance, u8 *Copy_u8String)
{
    while(*Copy_u8String != '\0')
    {
        USART_SendData(Instance, *Copy_u8String);
        Copy_u8String++;
    }
    USART_SendData(Instance, '\n');
}
u8 USART_IsDataAvailable(USART_Instance_t Copy_Instance)
{
    USART_Reg_t* Local_pUSART = USART_GetBaseAddress(Copy_Instance);

    // Return the state of the RXNE flag
    return GET_BIT(Local_pUSART->SR, USART_SR_RXNE);
}
