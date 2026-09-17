/*
 * DMA_private.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef DMA_PRIVATE_H_
#define DMA_PRIVATE_H_

#define DMA1_BASE_ADDRESS    0x40026000
#define DMA2_BASE_ADDRESS    0x40026400

typedef struct {
    volatile u32 CR;
    volatile u32 NDTR;
    volatile u32 PAR;
    volatile u32 M0AR;
    volatile u32 M1AR;
    volatile u32 FCR;
} DMA_Stream_t;

typedef struct {
    volatile u32 LISR;
    volatile u32 HISR;
    volatile u32 LIFCR;
    volatile u32 HIFCR;
    DMA_Stream_t STREAM[8];
} DMA_Reg_t;


#define DMA1                 ((DMA_Reg_t*) DMA1_BASE_ADDRESS)
#define DMA2                 ((DMA_Reg_t*) DMA2_BASE_ADDRESS)

#define DMA_CR_EN            0
#define DMA_CR_MINC          10
#define DMA_CR_PINC          9
#define DMA_CR_DIR0          6
#define DMA_CR_DIR1          7
#define DMA_CR_TCIE          4

#endif /* DMA_PRIVATE_H_ */
