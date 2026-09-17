/*
 * MRCC_interface.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MRCC_INTERFACE_H_
#define MRCC_INTERFACE_H_

//HB1 Peripheral Clock Enable (RCC_AHB1ENR)
#define GPIOA_EN 	 0
#define GPIOB_EN  	 1
#define GPIOC_EN 	 2
#define GPIOD_EN 	 3
#define GPIOE_EN 	 4
#define GPIOH_EN 	 7
#define CRC_EN  	12
#define DMA1_EN     21
#define DMA2_EN    22

/********************************************/
// AHB2 peripheral clock enable (RCC_AHB2ENR)
/********************************************/
#define OTGFS_EN 	 7

/*******************************************/
// APB1 Peripheral Clock Enable  (RCC_APB1ENR)
/**********************************************/
#define APB1_TIM2       0
#define APB1_TIM3       1
#define APB1_TIM4       2
#define APB1_TIM5       3
#define APB1_WWDG       11
#define APB1_SPI2       14
#define APB1_SPI3       15
#define APB1_USART2     17
#define APB1_I2C1       21
#define APB1_I2C2       22
#define APB1_I2C3       23
#define APB1_PWR        28

/********************************************/
// APB2 Peripheral Clock Enable  (RCC_APB2ENR)
/********************************************/
#define APB2_TIM1       0
#define APB2_USART1     4
#define APB2_USART6     5
#define APB2_ADC1       8
#define APB2_SDIO       11
#define APB2_SPI1       12
#define APB2_SPI4       13
#define APB2_SYSCFG     14
#define APB2_TIM9       16
#define APB2_TIM10      17
#define APB2_TIM11      18
/********************************************/
typedef enum
{
	AHB1_BUS=0,
	AHB2_BUS,
	APB1_BUS,
	APB2_BUS
}SYSTEM_BUS_t;
/********************************************/

//Initializes the system clock based on MRCC_config.h settings.
void MRCC_init(void);

/* Enables the clock for a specific peripheral.
 * COPY_uddtBus: Bus type (AHB1_BUS, AHB2_BUS, APB1_BUS, APB2_BUS)
 * COPY_peripheral: Peripheral bit number (GPIOA_EN,GPIOB_EN, GPIOC_EN,APB2_TIM1,APB2_USART1,...etc)
 */
void MRCC_EN_peripheral_CLK(SYSTEM_BUS_t  COPY_uddtBus, u8 COPY_peripheral);

/* Disables the clock for a specific peripheral.
 * COPY_uddtBus: Bus type (AHB1_BUS, AHB2_BUS, APB1_BUS, APB2_BUS)
 * COPY_peripheral: Peripheral bit number (GPIOA_EN,GPIOB_EN, GPIOC_EN,APB2_TIM1,APB2_USART1,...etc)
 */
void MRCC_DEN_peripheral_CLK(SYSTEM_BUS_t COPY_uddtBus, u8 COPY_peripheral);

#endif /* MRCC_INTERFACE_H_ */
