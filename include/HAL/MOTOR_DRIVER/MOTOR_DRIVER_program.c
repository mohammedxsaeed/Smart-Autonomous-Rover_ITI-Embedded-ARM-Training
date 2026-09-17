/*
 * MOTOR_DRIVER_program.c
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/TIM/TIM_interface.h"
#include "HAL/MOTOR_DRIVER/MOTOR_DRIVER_config.h"
#include "HAL/MOTOR_DRIVER/MOTOR_DRIVER_interface.h"

#define M4A_BIT   0
#define M2A_BIT   1
#define M1A_BIT   2
#define M1B_BIT   3
#define M2B_BIT   4
#define M3A_BIT   5
#define M4B_BIT   6
#define M3B_BIT   7

static const u8 MotorTrimPercent[5] = { 100, 100, 100, 100, 100 };

static u8 ApplyTrim(u8 Copy_u8MotorID, u8 Copy_u8Speed)
{
    u32 Local_u32Trimmed;
    if (Copy_u8MotorID < 1 || Copy_u8MotorID > 4) return Copy_u8Speed;

    Local_u32Trimmed = ((u32)Copy_u8Speed * MotorTrimPercent[Copy_u8MotorID]) / 100;
    if (Local_u32Trimmed > 100) Local_u32Trimmed = 100;
    return (u8)Local_u32Trimmed;
}

static u8 Global_u8ShiftData = 0;

static void ShiftOutData(void)
{
    // Latch Low
    GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_LATCH_PIN, LOW);

    for (s8 i = 7; i >= 0; i--)
    {
        // Clock Low
        GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_CLK_PIN, LOW);

        if (GET_BIT(Global_u8ShiftData, i)) {
            GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_DATA_PIN, HIGH);
        } else {
            GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_DATA_PIN, LOW);
        }

        // Clock High
        GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_CLK_PIN, HIGH);
    }

    GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_LATCH_PIN, HIGH);
}

void MOTOR_SHIELD_Init(void)
{
    GPIO_InitConfig_t ShiftPins = {
        .Mode  = OUTPUT,
        .OType = PUSH_PULL,
        .Speed = SPEED_HIGH,
        .Pupd  = NONE
    };

    ShiftPins.Pin = SHIFT_DATA_PIN;  GPIO_Init(SHIFT_REG_PORT, &ShiftPins);
    ShiftPins.Pin = SHIFT_CLK_PIN;   GPIO_Init(SHIFT_REG_PORT, &ShiftPins);
    ShiftPins.Pin = SHIFT_LATCH_PIN; GPIO_Init(SHIFT_REG_PORT, &ShiftPins);
    ShiftPins.Pin = SHIFT_EN_PIN;    GPIO_Init(SHIFT_REG_PORT, &ShiftPins);

    GPIO_SetPinValue(SHIFT_REG_PORT, SHIFT_EN_PIN, LOW);

    Global_u8ShiftData = 0;
    ShiftOutData();
}

void MOTOR_SHIELD_SetMotor(u8 Copy_u8MotorID, u8 Copy_u8Direction, u8 Copy_u8Speed)
{
    u8 Local_u8PinA = 0;
    u8 Local_u8PinB = 0;

    switch (Copy_u8MotorID)
    {
        case MOTOR_1: Local_u8PinA = M1A_BIT; Local_u8PinB = M1B_BIT; break;
        case MOTOR_2: Local_u8PinA = M2A_BIT; Local_u8PinB = M2B_BIT; break;
        case MOTOR_3: Local_u8PinA = M3A_BIT; Local_u8PinB = M3B_BIT; break;
        case MOTOR_4: Local_u8PinA = M4A_BIT; Local_u8PinB = M4B_BIT; break;
        default: return;
    }

    if (Copy_u8Direction == DIR_FORWARD)
    {
        SET_BIT(Global_u8ShiftData, Local_u8PinA);
        CLR_BIT(Global_u8ShiftData, Local_u8PinB);
    }
    else if (Copy_u8Direction == DIR_BACKWARD)
    {
        CLR_BIT(Global_u8ShiftData, Local_u8PinA);
        SET_BIT(Global_u8ShiftData, Local_u8PinB);
    }
    else if (Copy_u8Direction == DIR_STOP)
    {
        CLR_BIT(Global_u8ShiftData, Local_u8PinA);
        CLR_BIT(Global_u8ShiftData, Local_u8PinB);
        Copy_u8Speed = 0;
    }

    ShiftOutData();

    if (Copy_u8Speed != 0)
    {
        Copy_u8Speed = ApplyTrim(Copy_u8MotorID, Copy_u8Speed);
    }

    switch (Copy_u8MotorID)
        {
            case MOTOR_1: TIM_PWM_SetDutyCycle(TIM2, TIM_CH1, Copy_u8Speed); break;
            case MOTOR_2: TIM_PWM_SetDutyCycle(TIM2, TIM_CH2, Copy_u8Speed); break;
            case MOTOR_3: TIM_PWM_SetDutyCycle(TIM2, TIM_CH3, Copy_u8Speed); break;
            case MOTOR_4: TIM_PWM_SetDutyCycle(TIM2, TIM_CH4, Copy_u8Speed); break;
        }
}

void MOTOR_SHIELD_MoveForward(u8 Copy_u8Speed)
{
    MOTOR_SHIELD_SetMotor(MOTOR_1, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_2, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_3, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_4, DIR_FORWARD, Copy_u8Speed);
}

void MOTOR_SHIELD_MoveBackward(u8 Copy_u8Speed)
{
    MOTOR_SHIELD_SetMotor(MOTOR_1, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_2, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_3, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_4, DIR_BACKWARD, Copy_u8Speed);
}

void MOTOR_SHIELD_TurnRight(u8 Copy_u8Speed)
{
    MOTOR_SHIELD_SetMotor(MOTOR_1, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_2, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_3, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_4, DIR_BACKWARD, Copy_u8Speed);
}

void MOTOR_SHIELD_TurnLeft(u8 Copy_u8Speed)
{
    MOTOR_SHIELD_SetMotor(MOTOR_1, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_2, DIR_BACKWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_3, DIR_FORWARD, Copy_u8Speed);
    MOTOR_SHIELD_SetMotor(MOTOR_4, DIR_FORWARD, Copy_u8Speed);
}

void MOTOR_SHIELD_Stop(void)
{
    MOTOR_SHIELD_SetMotor(MOTOR_1, DIR_STOP, 0);
    MOTOR_SHIELD_SetMotor(MOTOR_2, DIR_STOP, 0);
    MOTOR_SHIELD_SetMotor(MOTOR_3, DIR_STOP, 0);
    MOTOR_SHIELD_SetMotor(MOTOR_4, DIR_STOP, 0);
}
