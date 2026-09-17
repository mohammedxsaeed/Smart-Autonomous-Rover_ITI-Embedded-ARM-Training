/*
 * NVIC_interface.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Mohammed Saeed
 */

#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_

#define NVIC_MAX_Interrupts  	84

typedef enum {
	GROUP16_SUB0 = 3, /* 4 bits for Group, 0 bits for Sub */
	GROUP8_SUB2 = 4, /* 3 bits for Group, 1 bit  for Sub */
	GROUP4_SUB4 = 5, /* 2 bits for Group, 2 bits for Sub */
	GROUP2_SUB8 = 6, /* 1 bit  for Group, 3 bits for Sub */
	GROUP0_SUB16 = 7 /* 0 bits for Group, 4 bits for Sub */
} NVIC_GroupMode_t;

/* Enables an external interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 */
void NVIC_EnableInterrupt(u8 Copy_u8IRQn);

/* Disables an external interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 */
void NVIC_DisableInterrupt(u8 Copy_u8IRQn);

/* Sets the pending flag of an external interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 */
void NVIC_SetPendingFlag(u8 Copy_u8IRQn);

/* Clears the pending flag of an external interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 */
void NVIC_ClearPendingFlag(u8 Copy_u8IRQn);

/* Reads the active flag of an external interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 * Returns 1 if active, 0 if not active.
 */
u8 NVIC_GetActiveFlag(u8 Copy_u8IRQn);

/* Reads the pending status of an interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 * Returns 1 if pending, 0 if not pending.
 */
u8 NVIC_GetPendingFlag(u8 Copy_u8IRQn);

/* Sets the priority of an interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 * Copy_u8Priority: Priority level (0 to 15 for STM32F4)
 */
void NVIC_SetPriority(u8 Copy_u8IRQn, u8 Copy_u8Priority);

/*
 * Configures the priority grouping system for all interrupts.
 * Copy_GroupMode: One of the NVIC_GroupMode_t options.
 */
void NVIC_SetPriorityGrouping(NVIC_GroupMode_t Copy_GroupMode);

/* Reads the priority of an interrupt.
 * Copy_u8IRQn: Interrupt number (0 to 239)
 * Returns the current priority level.
 */
u8 NVIC_GetPriority(u8 Copy_u8IRQn);

#endif /*NVIC_INTERFACE_H_ */
