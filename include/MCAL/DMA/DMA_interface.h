/*
 * DMA_interface.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef DMA_INTERFACE_H_
#define DMA_INTERFACE_H_

// Initialize DMA based on DMA_config.h
void DMA_Init(void);

// Set source, destination, and block size dynamically
void DMA_SetAddress(u32 *Copy_pu32SrcAddress, u32 *Copy_pu32DestAddress, u16 Copy_u16Size);

// Trigger the DMA transfer
void DMA_Enable(void);

#endif /* DMA_INTERFACE_H_ */
