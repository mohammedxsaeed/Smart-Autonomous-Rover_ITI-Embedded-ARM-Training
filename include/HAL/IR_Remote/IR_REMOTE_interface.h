/*
 * IR_REMOTE_interface.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */
#ifndef IR_REMOTE_INTERFACE_H_
#define IR_REMOTE_INTERFACE_H_

// Buttons Code for IR Remote Control
#define IR_BTN_0 	   0x68
#define IR_BTN_1 	   0xC6
#define IR_BTN_2 	   0x8C
#define IR_BTN_3 	   0xBD
#define IR_BTN_4 	   0x08
#define IR_BTN_5 	   0x9C
#define IR_BTN_6 	   0xEB
#define IR_BTN_7 	   0x42
#define IR_BTN_8 	   0x4A
#define IR_BTN_9 	   0x52
#define IR_BTN_OFF 	   0xA2
#define IR_BTN_MODE    0x62
#define IR_BTN_MUTE    0xE2
#define IR_BTN_PAUSE   0x22
#define IR_BTN_BACK    0x02
#define IR_BTN_NEXT    0xC2
#define IR_BTN_EQ      0xE0
#define IR_BTN_VOL_low   0xA8
#define IR_BTN_VOL_High  0x90
#define IR_BTN_RPT       0x98
#define IR_BTN_U_SD      0xB0


void IR_Init(void);

void IR_GetFrame(u32 Copy_u32ElapsedTime);

u8 IR_Decode(void);

#endif /* IR_REMOTE_INTERFACE_H_ */
