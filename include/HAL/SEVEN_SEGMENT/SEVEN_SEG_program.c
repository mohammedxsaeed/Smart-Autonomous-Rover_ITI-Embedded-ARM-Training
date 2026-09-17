#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "HAL/SEVEN_SEGMENT/SEVEN_SEG_config.h"
#include "HAL/SEVEN_SEGMENT/SEVEN_SEG_interface.h"

static const u8 seg[10] = {
    0xC0, /* 0 */
    0xF9, /* 1 */
    0xA4, /* 2 */
    0xB0, /* 3 */
    0x99, /* 4 */
    0x92, /* 5 */
    0x82, /* 6 */
    0xF8, /* 7 */
    0x80, /* 8 */
    0x90  /* 9 */
};

void Seg_Init(void)
{
    for(u8 i = 0; i < 7; i++)
    {
    	GPIO_SetPinMode(SEG_PORT, i, OUTPUT);
        GPIO_SetOutputConfig(SEG_PORT, i, PUSH_PULL, SPEED_MEDIUM);
    }
}
// from PIN0 to PIN7
void Seg_Display_num(u8 num)
{
    if (num < 10)
    {
        for(u8 i = 0; i < 7; i++)
        {
            u8 bit_val = GET_BIT(seg[num], i);
            GPIO_SetPinValue(SEG_PORT, i, bit_val);
        }
    }
}

void Seg_Display_All(void)
{
    for(u8 i = 0; i < 10; i++)
    {
        	Seg_Display_num(i);

        	STK_SetBusyWait(1000 * 2000);
    }
}
