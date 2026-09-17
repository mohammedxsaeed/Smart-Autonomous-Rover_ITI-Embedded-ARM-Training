/*
 * SERVO_program.c
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/RCC/MRCC_interface.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "MCAL/TIM/TIM_private.h"
#include "HAL/SERVO/SERVO_config.h"
#include "HAL/SERVO/SERVO_interface.h"

void SERVO_Init(void)
{
    MRCC_EN_peripheral_CLK(AHB1_BUS, GPIOB_EN);
    MRCC_EN_peripheral_CLK(APB1_BUS, 2); // TIM4EN bit is 2 in APB1ENR

    GPIO_InitConfig_t ServoPin = {
        .Mode = AF, .OType = PUSH_PULL, .Speed = SPEED_HIGH, .Pupd = NONE
    };
    ServoPin.Pin = SERVO_PIN;
    GPIO_Init(SERVO_PORT, &ServoPin);
    GPIO_SetAlternativeConfig(SERVO_PORT, SERVO_PIN, GPIO_AF2_TIM4);

    // System Clock = 16MHz -> PSC = 15 -> Timer Freq = 1MHz (1 Microsecond per tick)
    TIM4->PSC = 15;

    // Period = 20ms = 20000 Microseconds
    TIM4->ARR = 19999;

    CLR_PORT(TIM4->CCMR1);
    TIM4->CCMR1 |= (0x60 << 0);  // OC1M = 110 (PWM Mode 1)
    SET_BIT(TIM4->CCMR1, 3);     // OC1PE = 1 (Preload Enable)

        SET_BIT(TIM4->CCER, 0);      // CC1E = 1
    TIM4->CCR1 = 1500;
    SET_BIT(TIM4->EGR, 0);        // Generate update event
    SET_BIT(TIM4->CR1, 0);       // CEN = 1

}

void SERVO_SetAngle(u8 Copy_u8Angle)
{
    if (Copy_u8Angle > 180) Copy_u8Angle = 180;

    u32 Local_u32PulseWidth = 1000 + ((u32)Copy_u8Angle * 1000) / 180;

        TIM4->CCR1 = Local_u32PulseWidth;
    SET_BIT(TIM4->EGR, 0);

}
