/*
 * ADC_private.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

#define ADC1_BASE_ADDRESS    0x40012000  //
#define ADC_COMMON_BASE      0x40012300  //

typedef struct {
    volatile u32 SR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 SMPR1;
    volatile u32 SMPR2;
    volatile u32 JOFR1;
    volatile u32 JOFR2;
    volatile u32 JOFR3;
    volatile u32 JOFR4;
    volatile u32 HTR;
    volatile u32 LTR;
    volatile u32 SQR1;
    volatile u32 SQR2;
    volatile u32 SQR3;
    volatile u32 JSQR;
    volatile u32 JDR1;
    volatile u32 JDR2;
    volatile u32 JDR3;
    volatile u32 JDR4;
    volatile u32 DR;
} ADC_Reg_t;

#define ADC1    ((ADC_Reg_t*) ADC1_BASE_ADDRESS)

// ADC Status Register (SR) bits
#define ADC_SR_EOC       1

// ADC Control Register 1 (CR1) bits
#define ADC_CR1_RES0     24
#define ADC_CR1_RES1     25

// ADC Control Register 2 (CR2) bits
#define ADC_CR2_ADON     0
#define ADC_CR2_CONT     1
#define ADC_CR2_ALIGN    11
#define ADC_CR2_SWSTART  30

#endif /* ADC_PRIVATE_H_ */
