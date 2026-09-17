/*
 * SPI_private.h
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

#define SPI1_BASE_ADDRESS    0x40013000UL
#define SPI2_BASE_ADDRESS    0x40003800UL
#define SPI3_BASE_ADDRESS    0x40003C00UL

typedef struct {
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 SR;
    volatile u32 DR;
    volatile u32 CRCPR;
    volatile u32 RXCRCR;
    volatile u32 TXCRCR;
    volatile u32 I2SCFGR;
    volatile u32 I2SPR;
} SPI_RegDef_t;

#define SPI1    ((volatile SPI_RegDef_t*) SPI1_BASE_ADDRESS)
#define SPI2    ((volatile SPI_RegDef_t*) SPI2_BASE_ADDRESS)
#define SPI3    ((volatile SPI_RegDef_t*) SPI3_BASE_ADDRESS)

// SPI_CR1 Bits
#define SPI_CR1_CPHA      0
#define SPI_CR1_CPOL      1
#define SPI_CR1_MSTR      2
#define SPI_CR1_BR0       3
#define SPI_CR1_BR1       4
#define SPI_CR1_BR2       5
#define SPI_CR1_SPE       6
#define SPI_CR1_LSBFIRST  7
#define SPI_CR1_SSI       8
#define SPI_CR1_SSM       9
#define SPI_CR1_RXONLY    10
#define SPI_CR1_DFF       11

// SPI_SR Bits
#define SPI_SR_RXNE       0
#define SPI_SR_TXE        1
#define SPI_SR_BSY        7

#endif /* SPI_PRIVATE_H_ */
