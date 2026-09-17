/*
 * GPIO_private.h
 *
 *  Created on: Aug 17, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MCAL_GPIO_PRIVATE_H_
#define MCAL_GPIO_PRIVATE_H_


// Base addresses
#define GPIOA_BASE          (0x40020000UL)
#define GPIOB_BASE          (0x40020400UL)
#define GPIOC_BASE          (0x40020800UL)
#define GPIOD_BASE          (0x40020C00UL)
#define GPIOE_BASE          (0x40021000UL)
#define GPIOH_BASE          (0x40021C00UL)

//Register map
typedef struct
{
    volatile u32 MODER;
    volatile u32 OTYPER;
    volatile u32 OSPEEDR;
    volatile u32 PUPDR;
    volatile u32 IDR;
    volatile u32 ODR;
    volatile u32 BSRR;
    volatile u32 LCKR;
    volatile u32 AFR[2];

} GPIO_t;

// GPIO Ports Mapping
#define GPIOA   ((GPIO_t *) GPIOA_BASE)
#define GPIOB   ((GPIO_t *) GPIOB_BASE)
#define GPIOC   ((GPIO_t *) GPIOC_BASE)
#define GPIOD   ((GPIO_t *) GPIOD_BASE)
#define GPIOE   ((GPIO_t *) GPIOE_BASE)
#define GPIOH   ((GPIO_t *) GPIOH_BASE)



#endif /* MCAL_GPIO_PRIVATE_H_ */
