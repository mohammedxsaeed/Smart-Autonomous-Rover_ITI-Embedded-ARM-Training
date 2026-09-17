/*
 * SYSTICK_private.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SYSTICK_PRIVATE_H_
#define SYSTICK_PRIVATE_H_


#define STK_BASE_ADDR       0xE000E010UL

typedef struct {
    volatile u32 CTRL;   /* Control and Status Register */
    volatile u32 LOAD;   /* Reload Value Register */
    volatile u32 VAL;    /* Current Value Register */
    volatile u32 CALIB;  /* Calibration Value Register */
} STK_t;

/* Pointer to the SysTick registers */
#define STK                 ((STK_t*)STK_BASE_ADDR)

#define AHB_DIV_8       0
#define AHB             1

#define SINGLE_MODE     0
#define PERIODIC_MODE   1

#endif /* SYSTICK_PRIVATE_H_ */
