#ifndef TIM_PRIVATE_H_
#define TIM_PRIVATE_H_

#define TIM1_BASE_ADDRESS    0x40010000
#define TIM2_BASE_ADDRESS    0x40000000
#define TIM3_BASE_ADDRESS    0x40000400
#define TIM4_BASE_ADDRESS    0x40000800
#define TIM5_BASE_ADDRESS    0x40000C00
#define TIM9_BASE_ADDRESS    0x40014000
#define TIM10_BASE_ADDRESS   0x40014400
#define TIM11_BASE_ADDRESS   0x40014800

typedef struct {
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 SMCR;
    volatile u32 DIER;
    volatile u32 SR;
    volatile u32 EGR;
    volatile u32 CCMR1;
    volatile u32 CCMR2;
    volatile u32 CCER;
    volatile u32 CNT;
    volatile u32 PSC;
    volatile u32 ARR;
    volatile u32 Reserved1;
    volatile u32 CCR1;
    volatile u32 CCR2;
    volatile u32 CCR3;
    volatile u32 CCR4;
} TIM_Reg_t;

#define TIM1    ((TIM_Reg_t*) TIM1_BASE_ADDRESS)
#define TIM2    ((TIM_Reg_t*) TIM2_BASE_ADDRESS)
#define TIM3    ((TIM_Reg_t*) TIM3_BASE_ADDRESS)
#define TIM4	((TIM_Reg_t*) TIM4_BASE_ADDRESS)
#define TIM5    ((TIM_Reg_t*) TIM5_BASE_ADDRESS)
#define TIM9    ((TIM_Reg_t*) TIM9_BASE_ADDRESS)
#define TIM10   ((TIM_Reg_t*) TIM10_BASE_ADDRESS)
#define TIM11   ((TIM_Reg_t*) TIM11_BASE_ADDRESS)

#endif /* TIM_PRIVATE_H_ */
