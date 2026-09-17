/*
 * GPIO_program.c
 *
 *  Created on: Aug 18, 2026
 *      Author: Mohammed Saeed
 */

#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_private.h"
#include "MCAL/GPIO/GPIO_config.h"
#include "MCAL/GPIO/GPIO_interface.h"

// Helper Function Point to GPIO Port Address
static GPIO_t* Get_GPIO_Port(GPIO_Port_t Copy_uddtPort) {
	GPIO_t* GPIO_PORT = NULL;

	switch (Copy_uddtPort) {
	case PORTA:
		GPIO_PORT = GPIOA;
		break;
	case PORTB:
		GPIO_PORT = GPIOB;
		break;
	case PORTC:
		GPIO_PORT = GPIOC;
		break;
	case PORTD:
		GPIO_PORT = GPIOD;
		break;
	case PORTE:
		GPIO_PORT = GPIOE;
		break;
	case PORTH:
		GPIO_PORT = GPIOH;
		break;
	default:
		GPIO_PORT = NULL;
		break;
	}

	return GPIO_PORT;
}
/*
 *  Sets the pin operating mode:
 *  Copy_uddtPort: which GPIO port (PORTA..PORTH)
 *  Copy_uddtPin:  which pin (PIN0..PIN15)
 *  Copy_uddtMode: Select Mode (Input, Output, AF, Analog)
 */
void GPIO_SetPinMode(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin,GPIO_Mode_t Copy_uddtMode)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15) {
		//  Set New Mode
		switch (Copy_uddtMode) {
		case INPUT:  //00
		{
			CLR_BIT(PortReg->MODER, (Copy_uddtPin * 2)); //bit 2y
			CLR_BIT(PortReg->MODER, ((Copy_uddtPin * 2) + 1)); // bit 2y + 1
		}
			break;
		case OUTPUT: //01
		{
			SET_BIT(PortReg->MODER, (Copy_uddtPin * 2)); //bit 2y
			CLR_BIT(PortReg->MODER, ((Copy_uddtPin * 2) + 1)); // bit 2y + 1
		}
			break;
		case AF:	// 10
		{
			CLR_BIT(PortReg->MODER, (Copy_uddtPin * 2)); //bit 2y
			SET_BIT(PortReg->MODER, ((Copy_uddtPin * 2) + 1)); // bit 2y + 1
		}
			break;
		case ANALOG: //11
		{
			SET_BIT(PortReg->MODER, (Copy_uddtPin * 2)); //bit 2y
			SET_BIT(PortReg->MODER, ((Copy_uddtPin * 2) + 1)); // bit 2y + 1
		}
			break;
		default: {
			CLR_BIT(PortReg->MODER, (Copy_uddtPin * 2)); //bit 2y
			CLR_BIT(PortReg->MODER, ((Copy_uddtPin * 2) + 1)); // bit 2y + 1
		}
			break;
		}

	}
}
/* Configures output type and speed for a pin.
 * Copy_uddtPort: which GPIO port (PORTA..PORTH)
 * Copy_uddtPin: which pin (PIN0..PIN15)
 * Copy_uddtType: PUSH_PULL or OPEN_DRAIN
 * Copy_uddtSpeed: SPEED_LOW, SPEED_MEDIUM, SPEED_HIGH, SPEED_VERY_HIGH
 */
void GPIO_SetOutputConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin,GPIO_OType_t Copy_uddtType, GPIO_Speed_t Copy_uddtSpeed)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		// OUTPUT TYPE
		if (Copy_uddtType == OPEN_DRAIN)
		{
			SET_BIT(PortReg->OTYPER, Copy_uddtPin);
		}
		else // PUSH_PULL
		{
			CLR_BIT(PortReg->OTYPER, Copy_uddtPin);
		}
		// OUTPUT SPEED
		switch (Copy_uddtSpeed)
		{
			case SPEED_LOW:        /* 00 */
				CLR_BIT(PortReg->OSPEEDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->OSPEEDR, ((Copy_uddtPin * 2) + 1));
				break;

			case SPEED_MEDIUM:     /* 01 */
				SET_BIT(PortReg->OSPEEDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->OSPEEDR, ((Copy_uddtPin * 2) + 1));
				break;

			case SPEED_HIGH:       /* 10 */
				CLR_BIT(PortReg->OSPEEDR, (Copy_uddtPin * 2));
				SET_BIT(PortReg->OSPEEDR, ((Copy_uddtPin * 2) + 1));
				break;

			case SPEED_VERY_HIGH:  /* 11 */
				SET_BIT(PortReg->OSPEEDR, (Copy_uddtPin * 2));
				SET_BIT(PortReg->OSPEEDR, ((Copy_uddtPin * 2) + 1));
				break;

			default:
				CLR_BIT(PortReg->OSPEEDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->OSPEEDR, ((Copy_uddtPin * 2) + 1));
				break;
		}
	}
}

/* Configures the internal Pull-up / Pull-down resistor for a pin.
 * Copy_uddtPort: which GPIO port
 * Copy_uddtPin: which pin
 * Copy_uddtPupd: NONE, PULL_UP, or PULL_DOWN
 */
void GPIO_SetInputConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_Pupd_t Copy_uddtPupd)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		switch (Copy_uddtPupd)
		{
			case NONE:        /* 00 */
				CLR_BIT(PortReg->PUPDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->PUPDR, ((Copy_uddtPin * 2) + 1));
				break;

			case PULL_UP:     /* 01 */
				SET_BIT(PortReg->PUPDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->PUPDR, ((Copy_uddtPin * 2) + 1));
				break;

			case PULL_DOWN:   /* 10 */
				CLR_BIT(PortReg->PUPDR, (Copy_uddtPin * 2));
				SET_BIT(PortReg->PUPDR, ((Copy_uddtPin * 2) + 1));
				break;

			default:
				CLR_BIT(PortReg->PUPDR, (Copy_uddtPin * 2));
				CLR_BIT(PortReg->PUPDR, ((Copy_uddtPin * 2) + 1));
				break;
		}
	}
}

/* Selects the Alternate Function (AF0-AF15) for a pin.
 * Copy_uddtPort: which GPIO port
 * Copy_uddtPin: which pin
 * Copy_uddtAltFunc: AF0 - AF15
 */
void GPIO_SetAlternativeConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_AltFunc_t Copy_uddtAltFunc)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		/* Calculate Array Index: 0 for PIN0 - PIN7 (AFRL) and 1 for PIN8 - PIN15 (AFRH) */
		u8 AFR_ID = (Copy_uddtPin < PIN8) ? 0 : 1;

		/* Calculate the starting bit offset for the 4-bit field */
		u8 AFR_PIN_Base = (Copy_uddtPin < PIN8) ? (Copy_uddtPin * 4) : ((Copy_uddtPin - 8) * 4);

		switch (Copy_uddtAltFunc)
		{
			case AF0:  /* 0000 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF1:  /* 0001 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF2:  /* 0010 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF3:  /* 0011 */
				SET_BIT(PortReg->AFR[AFR_ID],  AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF4:  /* 0100 */
				CLR_BIT(PortReg->AFR[AFR_ID],  AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF5:  /* 0101 */
				SET_BIT(PortReg->AFR[AFR_ID],  AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF6:  /* 0110 */
				CLR_BIT(PortReg->AFR[AFR_ID],  AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF7:  /* 0111 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF8:  /* 1000 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF9:  /* 1001 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF10: /* 1010 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF11: /* 1011 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF12: /* 1100 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF13: /* 1101 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				CLR_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF14: /* 1110 */
				CLR_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			case AF15: /* 1111 */
				SET_BIT(PortReg->AFR[AFR_ID], AFR_PIN_Base);
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 1));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 2));
				SET_BIT(PortReg->AFR[AFR_ID], (AFR_PIN_Base + 3));
				break;

			default:
				break;
		}
	}
}
/* Sets a pin output value directly through the ODR register.
 * Copy_uddtPort: which GPIO port
 * Copy_uddtPin: which pin
 * Copy_uddtValue: HIGH or LOW
 */
void GPIO_SetPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_PinState_t Copy_uddtValue)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		if (Copy_uddtValue == HIGH)
		{
			SET_BIT(PortReg->ODR, Copy_uddtPin);
		}
		else
		{
			CLR_BIT(PortReg->ODR, Copy_uddtPin);
		}
	}
}

/* Sets a pin output value atomically through the BSRR register (safe from ISR).
 * Copy_uddtPort: which GPIO port (PORTA - PORTH)
 * Copy_uddtPin: which pin (PIN0 - PIN15)
 * Copy_uddtValue: HIGH or LOW
 */
void GPIO_SetAtomicPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_PinState_t Copy_uddtValue)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		if (Copy_uddtValue == HIGH)
		{
			PortReg->BSRR = (1UL << Copy_uddtPin);          /* BSy bit from 0..15 */
		}
		else
		{
			PortReg->BSRR = (1UL << (Copy_uddtPin + 16));    /* BRy bit from 16..31 */
		}
	}
}

/* Reads the digital value of an input pin from the IDR register.
 * Copy_uddtPort: which GPIO port
 * Copy_uddtPin: which pin
 * Returns  PIN_STATE (HIGH or LOW)
 */
GPIO_PinState_t GPIO_GetPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin)
{
	GPIO_PinState_t PIN_STATE = LOW;
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
		PIN_STATE = (GPIO_PinState_t)GET_BIT(PortReg->IDR, Copy_uddtPin);
	}

	return PIN_STATE;
}

/* Toggles the current output state of one or more pins.
 * Copy_uddtPort: which GPIO port (PORTA - PORTHD)
 * Copy_uddtPin: PIN0 - PIN15
 */
void GPIO_TogglePin(GPIO_Port_t Copy_uddtPort,GPIO_Pin_t Copy_uddtPin)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL)
	{
		TGL_BIT(PortReg->ODR ,Copy_uddtPin);
	}
}

/* Initializes one or multiple pins at once using the configuration struct.
 * Copy_uddtPort: which GPIO port
 * Copy_pConfig: pointer to a filled GPIO_InitConfig_t (Pin mask, Mode, OType, Speed, Pupd, AF)
 */
void GPIO_Init(GPIO_Port_t Copy_uddtPort, const GPIO_InitConfig_t *Copy_pConfig)
{
    GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

    if (PortReg != NULL && Copy_pConfig != NULL)
    {

        GPIO_Pin_t Local_Pin = Copy_pConfig->Pin;

        	//1. Set Mode
		 GPIO_SetPinMode(Copy_uddtPort, Local_Pin, Copy_pConfig->Mode);

		 // 2. Set Output Config if Mode is Output or AF
        if (Copy_pConfig->Mode == OUTPUT || Copy_pConfig->Mode == AF)
        {
            GPIO_SetOutputConfig(Copy_uddtPort, Local_Pin, Copy_pConfig->OType, Copy_pConfig->Speed);
        }

        // 3. Set Pull-Up / Pull-Down
        GPIO_SetInputConfig(Copy_uddtPort, Local_Pin, Copy_pConfig->Pupd);

        // 4. Set Alternate Function if Mode is AF
        if (Copy_pConfig->Mode == AF)
        {
            GPIO_SetAlternativeConfig(Copy_uddtPort, Local_Pin, Copy_pConfig->AF);
        }
    }
}

/* Resets the configuration of one or multiple pins to default reset values
 * (Input, No pull, Push-pull, Low speed).
 * Copy_uddtPort: which GPIO port
 * Copy_uddtPin: PIN to reset
 */
void GPIO_DeInit(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL)
	{
		for (u8 PinIndex = 0; PinIndex < 16; PinIndex++)
		{
			if ((Copy_uddtPin & (1U << PinIndex)) != 0)
			{
				GPIO_SetPinMode(Copy_uddtPort, (GPIO_Pin_t)PinIndex, INPUT);
				GPIO_SetInputConfig(Copy_uddtPort, (GPIO_Pin_t)PinIndex, NONE);
				GPIO_SetOutputConfig(Copy_uddtPort, (GPIO_Pin_t)PinIndex, PUSH_PULL, SPEED_LOW);
			}
		}
	}
}

/* Freezes the configuration of one or multiple pins until the next system reset.
 * Copy_uddtPort: which GPIO port
 * Copy_u16PinMask: bitmask of pins to lock
 */
void GPIO_LockPin(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin)
{
	GPIO_t* PortReg = Get_GPIO_Port(Copy_uddtPort);

	if (PortReg != NULL && Copy_uddtPin <= PIN15)
	{
	// pin mask to get pin value
		u32 PIN_MASK = (1UL << Copy_uddtPin);

       // ADD pin 16 to lock
		u32 LOCK_SEQ = (1UL << 16) | PIN_MASK;

		PortReg->LCKR = LOCK_SEQ; // write 1 in PIN15
		PortReg->LCKR = PIN_MASK; // write 0 in PIN15
		PortReg->LCKR = LOCK_SEQ; // write 1 in PIN15

		u32 Local_u32Temp = PortReg->LCKR;
		(void)Local_u32Temp;
	}
}

void GPIO_SetLowerByteValue(u8 Copy_u8Port, u8 Copy_u8Value)
{
    switch(Copy_u8Port)
    {
        case PORTA:
            GPIOA->ODR = (GPIOA->ODR & 0xFFFFFF00) | Copy_u8Value;
            break;
        case PORTB:
            GPIOB->ODR = (GPIOB->ODR & 0xFFFFFF00) | Copy_u8Value;
            break;
        case PORTC:
            GPIOC->ODR = (GPIOC->ODR & 0xFFFFFF00) | Copy_u8Value;
            break;
    }
}
