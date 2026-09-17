/*
 * SPI_program.c
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SPI/SPI_private.h"
#include "MCAL/SPI/SPI_config.h"
#include "MCAL/SPI/SPI_interface.h"

static volatile SPI_RegDef_t* SPI_GetSelectedInstance(void)
{
#if SPI_SELECTED_CHANNEL == SPI_CHANNEL_1
    return SPI1;
#elif SPI_SELECTED_CHANNEL == SPI_CHANNEL_2
    return SPI2;
#elif SPI_SELECTED_CHANNEL == SPI_CHANNEL_3
    return SPI3;
#else
    return SPI1;
#endif
}

void SPI_Init(void)
{
    volatile SPI_RegDef_t* Local_SPIx = SPI_GetSelectedInstance();

    GPIO_InitConfig_t SPI_Pins = {
        .Mode  = AF,
        .OType = PUSH_PULL,
        .Speed = SPEED_HIGH,
        .Pupd  = NONE,
        .AF    = AF5
    };

    SPI_Pins.Pin = SPI_PIN_SCK;
    GPIO_Init(SPI_PORT, &SPI_Pins);
    GPIO_SetAlternativeConfig(SPI_PORT, SPI_PIN_SCK, AF5);

    SPI_Pins.Pin = SPI_PIN_MISO;
    GPIO_Init(SPI_PORT, &SPI_Pins);
    GPIO_SetAlternativeConfig(SPI_PORT, SPI_PIN_MISO, AF5);

    SPI_Pins.Pin = SPI_PIN_MOSI;
    GPIO_Init(SPI_PORT, &SPI_Pins);
    GPIO_SetAlternativeConfig(SPI_PORT, SPI_PIN_MOSI, AF5);

    Local_SPIx->CR1 = 0;

    SET_BIT(Local_SPIx->CR1, SPI_CR1_SSM);

    SET_BIT(Local_SPIx->CR1, SPI_CR1_BR0);
    SET_BIT(Local_SPIx->CR1, SPI_CR1_BR1);
    CLR_BIT(Local_SPIx->CR1, SPI_CR1_BR2);

    SPI_SetRole(SPI_INITIAL_ROLE);
}

void SPI_SetRole(u8 Copy_u8Role)
{
	volatile SPI_RegDef_t* Copy_SPIx = SPI_GetSelectedInstance();
    CLR_BIT(Copy_SPIx->CR1, SPI_CR1_SPE);

    if (Copy_u8Role == MASTER)
    {
        SET_BIT(Copy_SPIx->CR1, SPI_CR1_MSTR);
        SET_BIT(Copy_SPIx->CR1, SPI_CR1_SSI);
    }
    else
    {
        CLR_BIT(Copy_SPIx->CR1, SPI_CR1_MSTR);
        CLR_BIT(Copy_SPIx->CR1, SPI_CR1_SSI);
        CLR_BIT(Copy_SPIx->CR1, SPI_CR1_SSM);
    }

    SET_BIT(Copy_SPIx->CR1, SPI_CR1_SPE);
}

u8 SPI_Transfer(u8 Copy_u8Data)
{
	volatile SPI_RegDef_t* Local_SPIx = SPI_GetSelectedInstance();

    while (GET_BIT(Local_SPIx->SR, SPI_SR_TXE) == 0);

    Local_SPIx->DR = Copy_u8Data;

    while (GET_BIT(Local_SPIx->SR, SPI_SR_RXNE) == 0);

    return (u8)(Local_SPIx->DR);
}
