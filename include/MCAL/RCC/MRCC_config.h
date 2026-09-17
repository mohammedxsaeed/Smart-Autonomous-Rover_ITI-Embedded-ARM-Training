/*
 * MRCC_config.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MCAL_MRCC_CONFIG_H_
#define MCAL_MRCC_CONFIG_H_

/*
 * Choose Between:
 1. HSI_CLK
 2. HSE_CLK
 3. PLL_CLK


 */
#define SYSTEM_CLK HSI_CLK

/*
 Choose Between:
 1. MECHANICAL_CLK
 2. RC_CLK
 */
#define HSE_OSL MECHANICAL_CLK

#endif /* MCAL_MRCC_CONFIG_H_ */
