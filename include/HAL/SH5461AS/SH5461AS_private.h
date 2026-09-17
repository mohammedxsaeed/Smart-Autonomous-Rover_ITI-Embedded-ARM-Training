/*
 * SH5461AS_private.h
 *
 *  Created on: Aug 21, 2026
 *      Author: Mohammed Saeed
 */

#ifndef SH5461AS_PRIVATE_H_
#define SH5461AS_PRIVATE_H_

// Common Cathode 7-Segment Hex Values for Digits 0-9
// A=Bit0, B=Bit1, C=Bit2, D=Bit3, E=Bit4, F=Bit5, G=Bit6
static const u8 SevSegArr[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

#endif /* SH5461AS_PRIVATE_H_ */
