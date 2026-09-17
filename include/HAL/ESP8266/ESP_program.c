/*
 * ESP_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */

#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/USART/USART_interface.h"
#include "HAL/ESP8266/ESP_config.h"
#include "HAL/ESP8266/ESP_interface.h"

// Internal Helper Function to validate if ESP replied with "OK"
static u8 ESP_ValidateCmd(void)
{
    u8 Local_u8Response = 0;
    u8 Local_u8Result = 0;
    u32 Local_u32Timeout = 0;

    while (Local_u32Timeout < ESP_TIMEOUT)
    {
        if (USART_IsDataAvailable(ESP_USART_INSTANCE) == 1)
        {
            Local_u8Response = USART_Rx(ESP_USART_INSTANCE);

            // Check for 'O' followed by 'K'
            if (Local_u8Response == 'O')
            {
                Local_u8Response = USART_Rx(ESP_USART_INSTANCE);
                if (Local_u8Response == 'K')
                {
                    Local_u8Result = 1; // Success
                    break;
                }
            }
        }
        Local_u32Timeout++;
    }
    return Local_u8Result;
}

void ESP_Init(void)
{
    // 1. Disable Echo (ATE0) so ESP doesn't repeat our commands back
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"ATE0\r\n");
    ESP_ValidateCmd();

    // 2. Set ESP to Station Mode (Client mode, not Access Point)
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"AT+CWMODE=1\r\n");
    ESP_ValidateCmd();
}

u8 ESP_ConnectToWiFi(u8 *Copy_u8SSID, u8 *Copy_u8Password)
{
    // Send AT+CWJAP="SSID","PASSWORD"
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"AT+CWJAP=\"");
    USART_SendString(ESP_USART_INSTANCE, Copy_u8SSID);
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"\",\"");
    USART_SendString(ESP_USART_INSTANCE, Copy_u8Password);
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"\"\r\n");

    // This might take a bit longer, so validation is crucial here
    return ESP_ValidateCmd();
}

u8 ESP_ConnectToTCP(u8 *Copy_u8IP, u8 *Copy_u8Port)
{
    // Send AT+CIPSTART="TCP","IP",PORT
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"AT+CIPSTART=\"TCP\",\"");
    USART_SendString(ESP_USART_INSTANCE, Copy_u8IP);
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"\",");
    USART_SendString(ESP_USART_INSTANCE, Copy_u8Port);
    USART_SendString(ESP_USART_INSTANCE, (u8 *)"\r\n");

    return ESP_ValidateCmd();
}
