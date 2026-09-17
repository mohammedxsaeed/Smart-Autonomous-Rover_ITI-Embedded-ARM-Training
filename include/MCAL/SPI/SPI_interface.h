/*
 * SPI_interface.h
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

void SPI_Init(void);
void SPI_SetRole(u8 Copy_u8Role);
u8 SPI_Transfer(u8 Copy_u8Data);


#endif /* SPI_INTERFACE_H_ */
