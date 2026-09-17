/*
 * BTM_interface.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */

#ifndef BTM_INTERFACE_H_
#define BTM_INTERFACE_H_

// Initialize Bluetooth Module (Initializes the underlying UART)
void BTM_Init(void);

// Send data to the connected device (Mobile App)
void BTM_SendData(u8 Copy_u8Data);

// Send a full string to the connected device
void BTM_SendString(u8 *Copy_u8String);

// Receive a single byte from the Bluetooth Module (Blocking)
u8 BTM_ReceiveData(void);

// Check if new data is available without blocking the CPU
u8 BTM_IsDataAvailable(void);

void BTM_SendNumber(u16 Copy_u16Number);
#endif /* BTM_INTERFACE_H_ */
