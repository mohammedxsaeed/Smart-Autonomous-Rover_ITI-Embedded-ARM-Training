/*
 * ADC_interface.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

// Initialize the ADC module based on configurations
void ADC_Init(void);

// Start conversion and read the analog value from a specific channel (Blocking / Polling)
u16 ADC_GetConversionValue(u8 Copy_u8Channel);

#endif /* ADC_INTERFACE_H_ */
