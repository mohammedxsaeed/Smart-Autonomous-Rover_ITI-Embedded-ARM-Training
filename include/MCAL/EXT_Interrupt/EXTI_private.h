/*
 * EXTI_private.h
 *
 *  Created on: Aug 20, 2026
 *      Author: Mohammed Saeed
 */

#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_

#define EXTI_BASE_ADDR      0x40013C00UL
#define SYSCFG_BASE_ADDR    0x40013800UL

typedef struct
{
    volatile u32 IMR;       // Interrupt mask register
    volatile u32 EMR;       // Event mask register
    volatile u32 RTSR;      // Rising trigger selection register
    volatile u32 FTSR;      // Falling trigger selection register
    volatile u32 SWIER;     // Software interrupt event register
    volatile u32 PR;        // Pending register
} EXTI_t;

typedef struct
{
    volatile u32 MEMRMP;
    volatile u32 PMC;
    volatile u32 EXTICR[4];
    volatile u32 Reserved[2];
    volatile u32 CMPCR;
} SYSCFG_t;

#define EXTI    ((volatile EXTI_t*)EXTI_BASE_ADDR)
#define SYSCFG      ((volatile SYSCFG_t*)SYSCFG_BASE_ADDR)

#endif /* EXTI_PRIVATE_H_ */
