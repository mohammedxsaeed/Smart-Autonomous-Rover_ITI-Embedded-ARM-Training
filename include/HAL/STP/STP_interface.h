/*
 * STP_interface.h
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */

#ifndef STP_INTERFACE_H_
#define STP_INTERFACE_H_

void STP_Init(void);
void STP_Latch(void);
void STP_SendData( u8 Copy_u8Data);
void STP_SendFrame(u8 *Copy_u8DataArray, u8 Copy_u8Size);
void STP_RefreshNextColumn(volatile u8 *Copy_u8DataArray, u8 Copy_u8Size);
void STP_StartAutoRefresh(volatile u8 *Copy_u8DataArray, u8 Copy_u8Size);


#endif /* STP_INTERFACE_H_ */
