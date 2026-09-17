/*
 * ADC_config.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

/*
 * Resolution Options:
 * 0: 12-bit (15 ADCCLK cycles)
 * 1: 10-bit (13 ADCCLK cycles)
 * 2: 8-bit  (11 ADCCLK cycles)
 * 3: 6-bit  (9 ADCCLK cycles)
 */
#define ADC_RESOLUTION     0

/* Data Alignment: 0 for Right Alignment, 1 for Left Alignment */
#define ADC_DATA_ALIGN     0

#endif /* ADC_CONFIG_H_ */
