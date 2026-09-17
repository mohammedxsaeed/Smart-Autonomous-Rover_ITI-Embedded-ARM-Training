/*
 * LEDMATRIX_program.c
 *
 *  Created on: Aug 25, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "HAL/LED_MATRIX/LEDMATRIX_config.h"
#include "HAL/LED_MATRIX/LEDMATRIX_interface.h"

static u8 COL[8]  = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};
static u8 ROWS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};
static void DisableAllCols(void)
{
    for (u8 i = 0; i < 8; i++)
    {
        GPIO_SetPinValue(COLUMS_PORT, COL[i], HIGH);
    }
}

static void SetRowValues(u8 Copy_u8Value)
{
    for (u8 i = 0; i < 8; i++)
    {
        GPIO_SetPinValue(ROWS_PORT, ROWS[i], GET_BIT(Copy_u8Value, i));
    }
}
void LEDMATRIX_init()
{
	GPIO_InitConfig_t Matrix_PinConfig = {
	        .Mode = OUTPUT,
	        .OType = PUSH_PULL,
	        .Speed = SPEED_LOW,
	        .Pupd = NONE
	    };
	for (u8 i = 0; i < 8; i++)
	    {
	        Matrix_PinConfig.Pin = ROWS[i];
	        GPIO_Init(ROWS_PORT, &Matrix_PinConfig);

	        Matrix_PinConfig.Pin = COL[i];
	        GPIO_Init(COLUMS_PORT, &Matrix_PinConfig);
	    }

	DisableAllCols();

}

void LEDMATRIX_DisplayFrame(u8 *Copy_u8Frame, u32 Frame_Duration)
{
	   for (u32 Local_u32Counter = 0; Local_u32Counter < Frame_Duration; Local_u32Counter++)
	        {
		   for(u8 i=0; i<8; i++)
		   {
		   	DisableAllCols();
		   	SetRowValues(Copy_u8Frame[i]);
		   	GPIO_SetPinValue(COLUMS_PORT, COL[i], LOW);
		   	Delay_ms(2);
		   }

	        }

}
