/*
 * MRCC_private.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MRCC_PRIVATE_H_
#define MRCC_PRIVATE_H_

// base address of RCC from reference Manual page 38 for STM32F401
#define RCC_BASE_ADDR (0x40023800UL)
/********************************************/
typedef struct {
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved1;
	volatile u32 Reserved2;
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved3;
	volatile u32 Reserved4;
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved5;
	volatile u32 Reserved6;
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
	volatile u32 Reserved7;
	volatile u32 Reserved8;
	volatile u32 AHB1LPENR;
	volatile u32 AHB2LPENR;
	volatile u32 Reserved9;
	volatile u32 Reserved10;
	volatile u32 APB1LPENR;
	volatile u32 APB2LPENR;
	volatile u32 Reserved11;
	volatile u32 Reserved12;
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 Reserved13;
	volatile u32 Reserved14;
	volatile u32 SSCGR;
	volatile u32 PLLI2SCFGR;
	volatile u32 Reserved15;
	volatile u32 DCKCFGR;

} RCC_t;
/********************************************/
// RCC pointer to base Address access struct elements by RCC->CR ..etc
#define RCC		 ((volatile RCC_t*) RCC_BASE_ADDR)
/********************************************/
// Clock control registers (CR)
#define HSION 0
#define HSIRDY 1
/********************************************/
#define HSEON 16
#define HSERDY 17
#define HSEBYP 18
#define CSSON 19
/********************************************/
#define PLLON 24
#define PLLRDY 25
#define PLLI2SON 26
#define PLLI2SRDY 27
/********************************************/
// Clock Source Selection
#define HSI_CLK 0
#define HSE_CLK 1
#define PLL_CLK 2
/********************************************/
#define MECHANICAL_CLK 0
#define RC_CLK 1
/********************************************/
// System Clock Switch (SW and SWS) - clock Configuration Register CFGR
#define SW0          0
#define SW1          1
#define SWS0         2
#define SWS1         3
/********************************************/
#define PLLSRC     22 // Configuration Register PLLCFGR


#endif /* MRCC_PRIVATE_H_ */
