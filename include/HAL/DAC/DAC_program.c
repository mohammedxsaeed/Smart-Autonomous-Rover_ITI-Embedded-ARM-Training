#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "HAL/DAC/DAC_config.h"
#include "HAL/DAC/DAC_private.h"
#include "HAL/DAC/DAC_interface.h"

void DAC_init(void)
{
    GPIO_InitConfig_t DAC_PinConfig = {
        .Mode = OUTPUT,
        .OType = PUSH_PULL,
        .Speed = SPEED_LOW,
        .Pupd = NONE
    };

    for (u8 i = 0; i < 8; i++)
    {
        DAC_PinConfig.Pin = i;
        GPIO_Init(DAC_PORT, &DAC_PinConfig);
    }
}

void DAC_SEND_DATA(const u8* Copy_pu8DataArray, u32 Copy_u32ArraySize, u32 Copy_u32DelayUs)
{
    u32 Counter;
    u8 BitIndex;

    for (Counter = 0; Counter < Copy_u32ArraySize; Counter++)
    {
        for (BitIndex = 0; BitIndex < 8; BitIndex++)
        {
            GPIO_SetPinValue(DAC_PORT, BitIndex, GET_BIT(Copy_pu8DataArray[Counter], BitIndex));
        }
        Delay_us(Copy_u32DelayUs);
    }
}
