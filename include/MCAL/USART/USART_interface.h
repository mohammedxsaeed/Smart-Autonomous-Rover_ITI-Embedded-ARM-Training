/*
 * USART_interface.h
 *
 *  Created on: Aug 30, 2026
 *      Author: Mohammed Saeed
 */
#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

typedef enum {
    USART_1 = 0,
    USART_2,
    USART_6
} USART_Instance_t;

// Initiate UART Tx and Rx
void USART_Init(USART_Instance_t Copy_Instance);

// Send any data from Master MC To any Rx
void USART_SendData(USART_Instance_t Instance, u8 Copy_u8Data);

// Send String Function
void USART_SendString(USART_Instance_t Instance, u8 *Copy_u8String);

// Receive data from MC or Rx Pin
u8 USART_Rx(USART_Instance_t Copy_Instance);

// Check if new data is received (Non-Blocking)
u8 USART_IsDataAvailable(USART_Instance_t Copy_Instance);
#endif /* USART_INTERFACE_H_ */
