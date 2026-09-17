/*
 * DMA_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/DMA/DMA_private.h"
#include "MCAL/DMA/DMA_config.h"
#include "MCAL/DMA/DMA_interface.h"

// Map the configured controller to the hardware registers
#if DMA_ACTIVE_CONTROLLER == 1
    #define SELECTED_DMA    DMA1
#elif DMA_ACTIVE_CONTROLLER == 2
    #define SELECTED_DMA    DMA2
#else
    #error "Invalid DMA Controller Selected in DMA_config.h"
#endif

void DMA_Init(void)
{
    // 1. Disable the stream to configure it safely
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_EN);

    // Wait until the stream is explicitly disabled
    while (GET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_EN) == 1);

    // 2. Configure Transfer Direction
#if DMA_TRANSFER_DIRECTION == 0       // Peripheral to Memory
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR0);
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR1);
#elif DMA_TRANSFER_DIRECTION == 1     // Memory to Peripheral
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR0);
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR1);
#elif DMA_TRANSFER_DIRECTION == 2     // Memory to Memory
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR0);
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_DIR1);
#endif

    // Data Size: Word (32-bit) for both Source and Destination
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, 12); // PSIZE = 10
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, 11);
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, 14); // MSIZE = 10
    CLR_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, 13);

    // Increment Memory and Peripheral addresses automatically
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_MINC);
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_PINC);

    // Enable Transfer Complete Interrupt (TCIE)
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_TCIE);
}

void DMA_SetAddress(u32 *Copy_pu32SrcAddress, u32 *Copy_pu32DestAddress, u16 Copy_u16Size)
{
    // In Memory-to-Memory: PAR acts as Source, M0AR acts as Destination
    // In other modes: PAR is Peripheral, M0AR is Memory
    SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].PAR = (u32)Copy_pu32SrcAddress;
    SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].M0AR = (u32)Copy_pu32DestAddress;

    // Set the number of data items to transfer
    SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].NDTR = Copy_u16Size;
}

void DMA_Enable(void)
{
    // Clear Transfer Complete interrupt flag based on Stream number
    // (Simplified check logic; real CMSIS defines specific flags per stream)
    if (DMA_ACTIVE_STREAM == 0) SET_BIT(SELECTED_DMA->LIFCR, 5); // CTCIF0

    // Enable the stream to start data transfer
    SET_BIT(SELECTED_DMA->STREAM[DMA_ACTIVE_STREAM].CR, DMA_CR_EN);
}
