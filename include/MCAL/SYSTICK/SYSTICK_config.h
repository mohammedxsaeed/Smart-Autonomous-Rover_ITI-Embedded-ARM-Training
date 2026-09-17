/*
 * SYSTICK_config.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SYSTICK_CONFIG_H_
#define SYSTICK_CONFIG_H_

/*
 * Options for Clock Source:
 * 1. AHB_DIV_8  (Processor clock / 8) -> Recommended for longer delays
 * 2. AHB        (Processor clock)
 */
#define STK_CLK_SRC     AHB

/*
 * Select the interval Mode:
 * 1. SINGLE_MODE
 * 2. PERIODIC_MODE
 */
#define STK_MODE 		PERIODIC_MODE

#endif /* SYSTICK_CONFIG_H_ */
