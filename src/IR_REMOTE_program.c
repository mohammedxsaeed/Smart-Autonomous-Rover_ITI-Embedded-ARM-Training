/*
 * IR_REMOTE_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "HAL/IR_REMOTE/IR_REMOTE_config.h"
#include "HAL/IR_REMOTE/IR_REMOTE_interface.h"

volatile u32 IR_u32FrameTimes[70] = {0};
volatile u8  IR_u8EdgeCounter = 0;
volatile u8  IR_u8DataReady = 0;

void IR_Init(void)
{
    GPIO_InitConfig_t IR_Pin = {
        .Mode = INPUT, .OType = PUSH_PULL, .Speed = SPEED_LOW, .Pupd = PULL_UP
    };
    IR_Pin.Pin = IR_PIN;
    GPIO_Init(IR_PORT, &IR_Pin);
}

void IR_GetFrame(u32 Copy_u32ElapsedTime)
{
    if (Copy_u32ElapsedTime > 20000)
    {
        IR_u8EdgeCounter = 0;
    }

    if (IR_u8EdgeCounter < 70)
    {
        IR_u32FrameTimes[IR_u8EdgeCounter] = Copy_u32ElapsedTime;
        IR_u8EdgeCounter++;
    }

    if (IR_u8EdgeCounter >= 68)
    {
        IR_u8DataReady = 1;
        IR_u8EdgeCounter = 0;
    }
}

u8 IR_Decode(void)
{
    u8 Local_u8Command = 0;
    u8 Local_u8BitIndex = 0;

    if (IR_u8DataReady == 1)
    {
        if ((IR_u32FrameTimes[1] >= 11000) && (IR_u32FrameTimes[1] <= 15000))
        {
            for (u8 i = 34; i < 50; i++)
            {
                if ((IR_u32FrameTimes[i] >= 1800) && (IR_u32FrameTimes[i] <= 3000))
                {
                    SET_BIT(Local_u8Command, Local_u8BitIndex);
                }
                else if ((IR_u32FrameTimes[i] >= 500) && (IR_u32FrameTimes[i] <= 1500))
                {
                    CLR_BIT(Local_u8Command, Local_u8BitIndex);
                }
                Local_u8BitIndex++;
            }
        }

        IR_u8DataReady = 0;
    }

    return Local_u8Command;
}
