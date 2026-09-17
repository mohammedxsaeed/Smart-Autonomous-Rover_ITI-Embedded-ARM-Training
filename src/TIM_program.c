#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/TIM/TIM_private.h"
#include "MCAL/TIM/TIM_config.h"
#include "MCAL/TIM/TIM_interface.h"

void TIM_PWM_Init(TIM_Reg_t* TIMx)
{
    TIMx->PSC = 15;
    TIMx->ARR = 999;

    CLR_PORT(TIMx->CCMR1);
    TIMx->CCMR1 |= (0x60 << 0);  // CH1 PWM1 Mode
    TIMx->CCMR1 |= (0x60 << 8);  // CH2 PWM1 Mode
    SET_BIT(TIMx->CCMR1, 3);
    SET_BIT(TIMx->CCMR1, 11);

    CLR_PORT(TIMx->CCMR2);
    TIMx->CCMR2 |= (0x60 << 0);  // CH3 PWM1 Mode
    TIMx->CCMR2 |= (0x60 << 8);  // CH4 PWM1 Mode
    SET_BIT(TIMx->CCMR2, 3);
    SET_BIT(TIMx->CCMR2, 11);

    SET_BIT(TIMx->CCER, 0);   // CC1E
    SET_BIT(TIMx->CCER, 4);   // CC2E
    SET_BIT(TIMx->CCER, 8);   // CC3E
    SET_BIT(TIMx->CCER, 12);  // CC4E
    SET_BIT(TIMx->CR1, 0);    // CEN
}

void TIM_PWM_SetDutyCycle(TIM_Reg_t* TIMx, u8 Copy_u8Channel, u8 Copy_u8DutyCycle)
{
    if (Copy_u8DutyCycle > 100) Copy_u8DutyCycle = 100;

    u32 Local_u32CompareValue = (Copy_u8DutyCycle * TIMx->ARR) / 100;

    switch (Copy_u8Channel)
    {
        case TIM_CH1: TIMx->CCR1 = Local_u32CompareValue; break;
        case TIM_CH2: TIMx->CCR2 = Local_u32CompareValue; break;
        case TIM_CH3: TIMx->CCR3 = Local_u32CompareValue; break;
        case TIM_CH4: TIMx->CCR4 = Local_u32CompareValue; break;
    }
}
