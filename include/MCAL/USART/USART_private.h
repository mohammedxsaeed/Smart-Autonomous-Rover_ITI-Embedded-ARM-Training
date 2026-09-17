/*
 * USART_private.h
 *
 *  Created on: Aug 30, 2026
 *      Author: Mohammed Saeed
 */

#ifndef USART_PRIVATE_H_
#define USART_PRIVATE_H_

#define USART1_BASE_ADDRESS    0x40011000
#define USART2_BASE_ADDRESS    0x40004400
#define USART6_BASE_ADDRESS    0x40011400

typedef struct {
    volatile u32 SR;
    volatile u32 DR;
    volatile u32 BRR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 CR3;
    volatile u32 GTPR;
} USART_Reg_t;

#define USART1    ((USART_Reg_t*) USART1_BASE_ADDRESS)
#define USART2    ((USART_Reg_t*) USART2_BASE_ADDRESS)
#define USART6    ((USART_Reg_t*) USART6_BASE_ADDRESS)

#define USART_SR_RXNE    5
#define USART_SR_TC      6
#define USART_SR_TXE     7

#define USART_CR1_RE     2
#define USART_CR1_TE     3
#define USART_CR1_UE     13

#endif /* USART_PRIVATE_H_ */
