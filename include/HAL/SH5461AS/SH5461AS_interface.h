/*
 * SH5461AS_interface.h
 *
 *  Created on: Aug 21, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SH5461AS_INTERFACE_H_
#define SH5461AS_INTERFACE_H_

// Initializes the GPIO pins for the 7-Segment display
void SH5461AS_Init(void);

// Stores the Hours and Minutes into the display buffer
void SH5461AS_SetTime(u8 Copy_u8Hours, u8 Copy_u8Minutes);

// Refreshes the display (MUST be called continuously in the super loop)
void SH5461AS_RefreshDisplay(void);

// Turns off all digits and segments
void SH5461AS_TurnOffAll(void);
// Test All Digits of The Seven Segment, Set delay Between each digit
void SH5461AS_TestWiring(u32 Copy_u32DelayMs);

#endif /* SH5461AS_INTERFACE_H_ */
