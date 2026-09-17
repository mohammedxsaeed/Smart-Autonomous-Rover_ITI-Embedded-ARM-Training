/*
 * DAC_interface.h
 * Created on: Aug 24, 2026
 * Author: Mohammed Saeed
 */
#ifndef DAC_INTERFACE_H_
#define DAC_INTERFACE_H_

void DAC_init(void);

/*
 * Sends an array of digital values to the DAC to play an audio wave
 * Copy_pu8DataArray: Pointer to the Audio Array (like song_raw)
 * Copy_u32ArraySize: Total number of elements in the array
 * Copy_u32DelayUs: Delay between each sample in Microseconds
 */
void DAC_SEND_DATA(const u8* Copy_pu8DataArray, u32 Copy_u32ArraySize, u32 Copy_u32DelayUs);

#endif /* DAC_INTERFACE_H_ */
