/*
 * TIM_interface.h
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 */
#ifndef TIM_INTERFACE_H_
#define TIM_INTERFACE_H_

#include "MCAL/TIM/TIM_private.h"

#define TIM_CH1    1
#define TIM_CH2    2
#define TIM_CH3    3
#define TIM_CH4    4

void TIM_PWM_Init(TIM_Reg_t* TIMx);
void TIM_PWM_SetDutyCycle(TIM_Reg_t* TIMx, u8 Copy_u8Channel, u8 Copy_u8DutyCycle);


#endif /* TIM_INTERFACE_H_ */
