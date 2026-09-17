/*
 * ADC_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/ADC/ADC_private.h"
#include "MCAL/ADC/ADC_config.h"
#include "MCAL/ADC/ADC_interface.h"

void ADC_Init(void)
{
    // 1. Set the Resolution
#if ADC_RESOLUTION == 0
    CLR_BIT(ADC1->CR1, ADC_CR1_RES0);
    CLR_BIT(ADC1->CR1, ADC_CR1_RES1);
#elif ADC_RESOLUTION == 1
    SET_BIT(ADC1->CR1, ADC_CR1_RES0);
    CLR_BIT(ADC1->CR1, ADC_CR1_RES1);
#elif ADC_RESOLUTION == 2
    CLR_BIT(ADC1->CR1, ADC_CR1_RES0);
    SET_BIT(ADC1->CR1, ADC_CR1_RES1);
#elif ADC_RESOLUTION == 3
    SET_BIT(ADC1->CR1, ADC_CR1_RES0);
    SET_BIT(ADC1->CR1, ADC_CR1_RES1);
#endif

    // 2. Set Data Alignment (Right or Left)
#if ADC_DATA_ALIGN == 0
    CLR_BIT(ADC1->CR2, ADC_CR2_ALIGN);
#elif ADC_DATA_ALIGN == 1
    SET_BIT(ADC1->CR2, ADC_CR2_ALIGN);
#endif

    // 3. Turn on the ADC (ADON bit)
    SET_BIT(ADC1->CR2, ADC_CR2_ADON);
}

u16 ADC_GetConversionValue(u8 Copy_u8Channel)
{
    // 1. Clear the first sequence register (SQ1) bits
    ADC1->SQR3 &= ~(0x1F); // Clear bits 0-4

    // 2. Set the requested channel as the first conversion in the sequence
    ADC1->SQR3 |= (Copy_u8Channel & 0x1F);

    // 3. Set the total sequence length to 1 conversion (L[3:0] in SQR1 = 0000)
    ADC1->SQR1 &= ~(0xF << 20);

    // 4. Start the conversion (SWSTART)[cite: 6]
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);

    // 5. Wait for the End Of Conversion (EOC) flag
    while (GET_BIT(ADC1->SR, ADC_SR_EOC) == 0);

    // 6. Return the converted data
    return (u16)(ADC1->DR & 0xFFFF);
}
