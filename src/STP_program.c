/*
 * STP_program.c
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/RCC/MRCC_interface.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/TIM/TIM_interface.h"
#include "MCAL/NVIC/NVIC_interface.h"
#include "HAL/STP/STP_config.h"
#include "HAL/STP/STP_interface.h"

#define TIM1_UP_TIM10_IRQn      25

void STP_Init(void)
{
	GPIO_InitConfig_t STP_PinConfig = {
	        .Mode  = OUTPUT,
	        .OType = PUSH_PULL,
	        .Speed = SPEED_LOW,
	        .Pupd  = NONE
	    };
		STP_PinConfig.Pin = STP_DATA_PIN;
	    GPIO_Init(STP_PORT, &STP_PinConfig);

	    STP_PinConfig.Pin = STP_SH_CLK_PIN;
	    GPIO_Init(STP_PORT, &STP_PinConfig);

	    STP_PinConfig.Pin = STP_ST_CLK_PIN;
	    GPIO_Init(STP_PORT, &STP_PinConfig);
}
void STP_Latch(void)
{
	GPIO_SetPinValue(STP_PORT, STP_ST_CLK_PIN, LOW);
	GPIO_SetPinValue(STP_PORT, STP_ST_CLK_PIN, HIGH);
}
void STP_SendData( u8 Copy_u8Data)
{
	for(s8 i=7; i>=0; i--)
	{
		GPIO_SetPinValue(STP_PORT, STP_DATA_PIN, GET_BIT(Copy_u8Data, i));

		GPIO_SetPinValue(STP_PORT, STP_SH_CLK_PIN, LOW);
		GPIO_SetPinValue(STP_PORT, STP_SH_CLK_PIN, HIGH);

	}

	}

void STP_SendFrame(u8 *Copy_u8DataArray, u8 Copy_u8Size)
{
	for (s8 i=(Copy_u8Size-1); i>=0; i--)
	{
		STP_SendData( Copy_u8DataArray[i]);
        STP_SendData(~(1 << i));
        STP_Latch();

	}
}

void STP_RefreshNextColumn(volatile u8 *Copy_u8DataArray, u8 Copy_u8Size)
{
	static u8 Local_u8CurrentCol = 0;

	STP_SendData(Copy_u8DataArray[Local_u8CurrentCol]);
	STP_SendData(~(1 << Local_u8CurrentCol));
	STP_Latch();

	Local_u8CurrentCol++;
	if (Local_u8CurrentCol >= Copy_u8Size) Local_u8CurrentCol = 0;
}

static volatile u8 *Local_ptrAutoRefreshBuffer = NULL;
static u8  Local_u8AutoRefreshSize    = 0;

void STP_StartAutoRefresh(volatile u8 *Copy_u8DataArray, u8 Copy_u8Size)
{
	Local_ptrAutoRefreshBuffer = Copy_u8DataArray;
	Local_u8AutoRefreshSize    = Copy_u8Size;

	/* TIM1 is on APB2. Make sure APB2 TIM1 clock is enabled in main()
	 *   MRCC_EN_peripheral_CLK(APB2_BUS, APB2_TIM1);
	/* 16MHz / 16000 = 1kHz timer clock -> 1 tick = 1ms.
	 * ARR = 1 -> counts 0,1 then overflows -> interrupt every 2ms. */
	TIM1->PSC = 15999;
	TIM1->ARR = 1;

	SET_BIT(TIM1->DIER, 0); // UIE - update interrupt enable
	SET_BIT(TIM1->CR1, 0);  // CEN - counter enable

	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1); /* high-ish priority so the refresh stays on time even next to other ISRs (e.g. EXTI for IR) */
	NVIC_EnableInterrupt(TIM1_UP_TIM10_IRQn);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if (GET_BIT(TIM1->SR, 0)) // UIF
	{
		CLR_BIT(TIM1->SR, 0);

		if (Local_ptrAutoRefreshBuffer != NULL)
		{
			STP_RefreshNextColumn(Local_ptrAutoRefreshBuffer, Local_u8AutoRefreshSize);
		}
	}
}
