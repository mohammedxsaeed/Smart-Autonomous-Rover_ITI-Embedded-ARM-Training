/*
 * SH5461AS_program.c
 *
 *  Created on: Aug 21, 2026
 *      Author: Mohammed Saeed
 */

#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"

#include "MCAL/GPIO/GPIO_interface.h"

#include "HAL/SH5461AS/SH5461AS_private.h"
#include "HAL/SH5461AS/SH5461AS_config.h"
#include "HAL/SH5461AS/SH5461AS_interface.h"

// Buffer to store the 4 digits to be displayed
static u8 G_u8DisplayBuffer[4] = {0, 0, 0, 0};

// Initializes the 7-segment display pins (Segments and Digits)
void SH5461AS_Init(void)
{
    // Common configuration for all display pins
    GPIO_InitConfig_t DisplayConfig = {
        .Mode = OUTPUT,
        .OType = PUSH_PULL,
        .Speed = SPEED_MEDIUM,
        .Pupd = NONE
    };

    // Configure Segments (A to G)
    DisplayConfig.Pin = SH5461AS_SEG_A_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_B_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_C_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_D_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_E_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_F_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_SEG_G_PIN;  GPIO_Init(SH5461AS_SEG_PORT, &DisplayConfig);

    // Configure Digits (D1 to D4)
    DisplayConfig.Pin = SH5461AS_DIG_D1_PIN; GPIO_Init(SH5461AS_DIG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_DIG_D2_PIN; GPIO_Init(SH5461AS_DIG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_DIG_D3_PIN; GPIO_Init(SH5461AS_DIG_PORT, &DisplayConfig);
    DisplayConfig.Pin = SH5461AS_DIG_D4_PIN; GPIO_Init(SH5461AS_DIG_PORT, &DisplayConfig);

    // Turn off all digits initially to prevent ghosting
    SH5461AS_TurnOffAll();
}

// Updates the display buffer with hours and minutes
// Inputs: Copy_u8Hours (0-23), Copy_u8Minutes (0-59)
void SH5461AS_SetTime(u8 Copy_u8Hours, u8 Copy_u8Minutes)
{
    // Extract digits and store them in the buffer
    G_u8DisplayBuffer[0] = Copy_u8Hours / 10;
    G_u8DisplayBuffer[1] = Copy_u8Hours % 10;
    G_u8DisplayBuffer[2] = Copy_u8Minutes / 10;
    G_u8DisplayBuffer[3] = Copy_u8Minutes % 10;
}

// Disables all 4 digits (Assumes Common Cathode, so HIGH turns them OFF)
void SH5461AS_TurnOffAll(void)
{
    GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D1_PIN, HIGH);
    GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D2_PIN, HIGH);
    GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D3_PIN, HIGH);
    GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D4_PIN, HIGH);
}

// Multiplexes the display. MUST be called continuously in the main loop.
void SH5461AS_RefreshDisplay(void)
{
    static u8 Local_u8CurrentDigit = 0;
    u8 Local_u8NumberPattern = SevSegArr[G_u8DisplayBuffer[Local_u8CurrentDigit]];

    // 1. Turn off all digits before updating segments to prevent ghosting
    SH5461AS_TurnOffAll();

    // 2. Write the 7-segment pattern for the current digit (A to G)
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_A_PIN, GET_BIT(Local_u8NumberPattern, 0));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_B_PIN, GET_BIT(Local_u8NumberPattern, 1));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_C_PIN, GET_BIT(Local_u8NumberPattern, 2));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_D_PIN, GET_BIT(Local_u8NumberPattern, 3));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_E_PIN, GET_BIT(Local_u8NumberPattern, 4));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_F_PIN, GET_BIT(Local_u8NumberPattern, 5));
    GPIO_SetPinValue(SH5461AS_SEG_PORT, SH5461AS_SEG_G_PIN, GET_BIT(Local_u8NumberPattern, 6));

    // 3. Enable the current digit (LOW enables Common Cathode)
    switch (Local_u8CurrentDigit)
    {
        case 0: GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D1_PIN, LOW); break;
        case 1: GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D2_PIN, LOW); break;
        case 2: GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D3_PIN, LOW); break;
        case 3: GPIO_SetPinValue(SH5461AS_DIG_PORT, SH5461AS_DIG_D4_PIN, LOW); break;
    }

    // 4. Move to the next digit for the next refresh cycle
    Local_u8CurrentDigit++;
    if (Local_u8CurrentDigit > 3)
    {
        Local_u8CurrentDigit = 0;
    }
}
