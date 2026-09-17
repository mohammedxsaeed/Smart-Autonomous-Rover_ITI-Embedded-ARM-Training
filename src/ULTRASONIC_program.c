/*
 * ULTRASONIC_program.c
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 **/
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/TIM/TIM_private.h"
#include "HAL/ULTRASONIC/ULTRASONIC_config.h"
#include "HAL/ULTRASONIC/ULTRASONIC_interface.h"

/* HC-SR04 timing limits, in microseconds */
#define ULTRASONIC_TRIG_PULSE_US           10U
#define ULTRASONIC_ECHO_START_TIMEOUT_US   30000U  /* no obstacle detected */
#define ULTRASONIC_ECHO_WIDTH_TIMEOUT_US   38000U  /* out-of-range echo    */

/* Reads the free-running TIM4 counter (1 tick = 1 microsecond). */
static u32 TIM3_ReadMicros(void)
{
    return TIM3->CNT;
}


static void TIM3_WaitMicros(u32 Copy_u32Micros)
{
    u32 Local_u32Start = TIM3_ReadMicros();
    while ((u16)(TIM3_ReadMicros() - Local_u32Start) < Copy_u32Micros);
}

void ULTRASONIC_Init(void)
{
    GPIO_InitConfig_t TrigPin = {
        .Mode = OUTPUT, .OType = PUSH_PULL, .Speed = SPEED_HIGH, .Pupd = NONE
    };
    TrigPin.Pin = ULTRASONIC_TRIG_PIN;
    GPIO_Init(ULTRASONIC_TRIG_PORT, &TrigPin);

    GPIO_InitConfig_t EchoPin = {
        .Mode = INPUT, .OType = PUSH_PULL, .Speed = SPEED_HIGH, .Pupd = NONE
    };
    EchoPin.Pin = ULTRASONIC_ECHO_PIN;
    GPIO_Init(ULTRASONIC_ECHO_PORT, &EchoPin);

    TIM3->PSC = 15;         /* 16MHz / 16 = 1MHz -> 1 tick = 1 microsecond */
    TIM3->ARR = 0xFFFF;
    TIM3->CNT = 0;
    SET_BIT(TIM3->CR1, 0);  /* CEN = 1 */
}


u16 ULTRASONIC_GetDistance(void)
{
    u32 Local_u32StartTime;
    u32 Local_u32ElapsedTime;
    u16 Local_u16Distance;

    /* Send 10us trigger pulse */
    GPIO_SetPinValue(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, HIGH);
    TIM3_WaitMicros(ULTRASONIC_TRIG_PULSE_US);
    GPIO_SetPinValue(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, LOW);

    /* Wait for Echo to go HIGH (start of pulse), bounded by a real timeout */
    Local_u32StartTime = TIM3_ReadMicros();
    while (GPIO_GetPinValue(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == LOW)
    {
        if ((u16)(TIM3_ReadMicros() - Local_u32StartTime) > ULTRASONIC_ECHO_START_TIMEOUT_US)
        {
            return 0;
        }
    }

    /* Measure how long Echo stays HIGH, bounded by a real timeout */
    Local_u32StartTime = TIM3_ReadMicros();
    while (GPIO_GetPinValue(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == HIGH)
    {
        Local_u32ElapsedTime = (u16)(TIM3_ReadMicros() - Local_u32StartTime);
        if (Local_u32ElapsedTime > ULTRASONIC_ECHO_WIDTH_TIMEOUT_US)
        {
            return 0;
        }
    }
    Local_u32ElapsedTime = (u16)(TIM3_ReadMicros() - Local_u32StartTime);

    Local_u16Distance = (u16)(Local_u32ElapsedTime / 58);
    return Local_u16Distance;
}
