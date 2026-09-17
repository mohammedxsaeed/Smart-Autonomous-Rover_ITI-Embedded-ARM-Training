/*
 * DMA_config.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef DMA_CONFIG_H_
#define DMA_CONFIG_H_

// Select DMA Controller: 1 for DMA1, 2 for DMA2
#define DMA_ACTIVE_CONTROLLER    2

// Select Stream: 0 to 7
#define DMA_ACTIVE_STREAM        0

/* Select Transfer Direction:
 * 0: Peripheral to Memory
 * 1: Memory to Peripheral
 * 2: Memory to Memory (Only valid if DMA_ACTIVE_CONTROLLER is 2)
 */
#define DMA_TRANSFER_DIRECTION   2

#endif /* DMA_CONFIG_H_ */
