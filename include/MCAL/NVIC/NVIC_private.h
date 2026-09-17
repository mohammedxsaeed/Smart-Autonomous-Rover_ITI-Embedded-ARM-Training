/*
 * NVIC_private.h
 */

#ifndef MCAL_NVIC_PRIVATE_H_
#define MCAL_NVIC_PRIVATE_H_

#define NVIC_BASE_ADDRESS    0xE000E100UL
#define SCB_BASE_ADDRESS     0xE000ED00UL  //System Control Block

typedef struct {
	volatile u32 ISER[8]; 		//Interrupt Set-Enable Registers
	u32 RESERVED0[24];

	volatile u32 ICER[8];       // Interrupt Clear-Enable Registers
	u32 RESERVED1[24];

	volatile u32 ISPR[8];       // Interrupt Set-Pending Registers
	u32 RESERVED2[24];

	volatile u32 ICPR[8];       // Interrupt Clear-Pending Registers
	u32 RESERVED3[24];

	volatile u32 IABR[8];       // Interrupt Active Bit Registers
	u32 RESERVED4[56];

	volatile u8 IPR[240]; 		// IPR[60]->IP[4] :  Interrupt Priority Registers
	u32 RESERVED5[644];

	volatile u32 STIR; 			// Software Trigger Interrupt Register
} NVIC_t;

/* Pointer to the NVIC registers */
#define NVIC    ((NVIC_t*)NVIC_BASE_ADDRESS)

#define SCB_AIRCR   *((volatile u32*)(SCB_BASE_ADDRESS + 0x0C))

#define VECTKEY     0x05FA0000

#endif /* MCAL_NVIC_PRIVATE_H_ */
