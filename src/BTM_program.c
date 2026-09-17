/*
 * BTM_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/USART/USART_interface.h"
#include "HAL/BTM/BTM_config.h"
#include "HAL/BTM/BTM_interface.h"

void BTM_Init(void)
{
    // Initialize the configured USART instance
    USART_Init(BTM_USART_INSTANCE);
}

void BTM_SendData(u8 Copy_u8Data)
{
    USART_SendData(BTM_USART_INSTANCE, Copy_u8Data);
}

void BTM_SendString(u8 *Copy_u8String)
{
    USART_SendString(BTM_USART_INSTANCE, Copy_u8String);
}

u8 BTM_ReceiveData(void)
{
    return USART_Rx(BTM_USART_INSTANCE);
}

u8 BTM_IsDataAvailable(void)
{
    return USART_IsDataAvailable(BTM_USART_INSTANCE);
}
void BTM_SendNumber(u16 Copy_u16Number)
{
    u8 Digits[5];
    u8 Index = 0;

    if (Copy_u16Number == 0)
    {
        BTM_SendData('0');
        return;
    }

    while (Copy_u16Number > 0)
    {
        Digits[Index++] = (Copy_u16Number % 10) + '0';
        Copy_u16Number /= 10;
    }

    while (Index > 0)
    {
        BTM_SendData(Digits[--Index]);
    }
}
