/*
 * MRCC_program.c
 *  Created on: Aug 16, 2026
 *  Author: Mohammed Saeed
 */
/********************************************/
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/RCC/MRCC_private.h"
#include "MCAL/RCC/MRCC_config.h"
#include "MCAL/RCC/MRCC_interface.h"
/********************************************/

//Initializes the system clock based on MRCC_config.h settings.
void MRCC_init(void) {
#if SYSTEM_CLK == HSI_CLK
	// 1. Turn ON HSI
	SET_BIT(RCC->CR, HSION);

	// 2. Wait until HSI is ready
	while (!GET_BIT(RCC->CR, HSIRDY));

	//3. Switch System Clock to HSI (SW = 00)
	CLR_BIT(RCC->CFGR, SW0);
	CLR_BIT(RCC->CFGR, SW1);

#elif SYSTEM_CLK == HSE_CLK
	// 1. Configure Oscillator Type
#if HSE_OSL == RC_CLK
	SET_BIT(RCC->CR, HSEBYP);
#elif HSE_OSL == MECHANICAL_CLK
	CLR_BIT(RCC->CR, HSEBYP);
#endif

	// 2. Turn ON HSE
	SET_BIT(RCC->CR, HSEON);

	// 3. Wait until HSE is ready
	while (GET_BIT(RCC->CR, HSERDY) == 0)
		;

	// 4. Switch System Clock to HSE (SW = 01)
	SET_BIT(RCC->CFGR, SW0);
	CLR_BIT(RCC->CFGR, SW1);

#elif SYSTEM_CLK == PLL_CLK
	// 1. Turn ON PLL
	SET_BIT(RCC->CR, PLLON);

	// 2. Wait until PLL is locked/ready
	while (GET_BIT(RCC->CR, PLLRDY) == 0);

	// 3. Switch System Clock to PLL (SW = 10)
	CLR_BIT(RCC->CFGR, SW0);
	SET_BIT(RCC->CFGR, SW1);

#else
#error "Invalid SYSTEM_CLK configuration in MRCC_config.h!"
#endif
}

/*****************************************************************/
/* Enables the clock for a specific peripheral.
 * COPY_uddtBus: Bus type (AHB1_BUS, AHB2_BUS, APB1_BUS, APB2_BUS)
 * COPY_peripheral: Peripheral bit number (GPIOA_EN,GPIOB_EN, GPIOC_EN,APB2_TIM1,APB2_USART1,...etc)
 */
void MRCC_EN_peripheral_CLK(SYSTEM_BUS_t COPY_uddtBus, u8 COPY_peripheral) {

	switch (COPY_uddtBus) {
	case AHB1_BUS:
		SET_BIT(RCC->AHB1ENR, COPY_peripheral);
		break;

	case AHB2_BUS:
		SET_BIT(RCC->AHB2ENR, COPY_peripheral);
		break;

	case APB1_BUS:
		SET_BIT(RCC->APB1ENR, COPY_peripheral);
		break;

	case APB2_BUS:
		SET_BIT(RCC->APB2ENR, COPY_peripheral);
		break;

	default:
		break;
	}
}

/* Disables the clock for a specific peripheral.
 * COPY_uddtBus: Bus type (AHB1_BUS, AHB2_BUS, APB1_BUS, APB2_BUS)
 * COPY_peripheral: Peripheral bit number (GPIOA_EN,GPIOB_EN, GPIOC_EN,APB2_TIM1,APB2_USART1,...etc)
 */
void MRCC_DEN_peripheral_CLK(SYSTEM_BUS_t COPY_uddtBus, u8 COPY_peripheral) {
	switch (COPY_uddtBus) {
	case AHB1_BUS:
		CLR_BIT(RCC->AHB1ENR, COPY_peripheral);
		break;

	case AHB2_BUS:
		CLR_BIT(RCC->AHB2ENR, COPY_peripheral);
		break;

	case APB1_BUS:
		CLR_BIT(RCC->APB1ENR, COPY_peripheral);
		break;

	case APB2_BUS:
		CLR_BIT(RCC->APB2ENR, COPY_peripheral);
		break;

	default:
		break;
	}

}
