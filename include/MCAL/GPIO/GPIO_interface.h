/*
 * GPIO_interface.h
 *
 *  Created on: Aug 17, 2026
 *      Author: Mohammed Saeed
 */
#ifndef INCLUDE_MCAL_GPIO_INTERFACE_H_
#define INCLUDE_MCAL_GPIO_INTERFACE_H_

typedef enum {
	PORTA = 0,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTH = 7
} GPIO_Port_t;

typedef enum {
	PIN0 = 0,
	PIN1,
	PIN2,
	PIN3,
	PIN4,
	PIN5,
	PIN6,
	PIN7,
	PIN8,
	PIN9,
	PIN10,
	PIN11,
	PIN12,
	PIN13,
	PIN14,
	PIN15
} GPIO_Pin_t;

typedef enum {
	INPUT = 0,
	OUTPUT,
	AF,
	ANALOG
} GPIO_Mode_t;

typedef enum {
	PUSH_PULL = 0,
	OPEN_DRAIN
} GPIO_OType_t;

typedef enum {
	SPEED_LOW = 0,
	SPEED_MEDIUM,
	SPEED_HIGH,
	SPEED_VERY_HIGH
} GPIO_Speed_t;

typedef enum {
	NONE = 0,
	PULL_UP,
	PULL_DOWN
} GPIO_Pupd_t; // input Mode

typedef enum {
	LOW = 0,  // SET LOW
	HIGH  		 // SET HIGH
} GPIO_PinState_t;

typedef enum {
	AF0 = 0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15
} GPIO_AltFunc_t;

typedef struct {
	GPIO_Pin_t Pin;  	// PIN0 - PIN15
	GPIO_Mode_t Mode; 	//INPUT, OUTPUT, AF, ANALOG
	GPIO_OType_t OType; // Output type (PUSH_PULL ,OPEN_DRAIN)
	GPIO_Speed_t Speed; // Speed Configuration:	SPEED_LOW , SPEED_MEDIUM, SPEED_HIGH, SPEED_VERY_HIGH
	GPIO_Pupd_t Pupd;   // NONE (Not PU nor PD), PULL_UP, PULL_DOWN
	GPIO_AltFunc_t AF;  // Alternate function index (AF0 - AF15)
} GPIO_InitConfig_t;

// Sets the pin operating mode (Input, Output, AF, Analog)
void GPIO_SetPinMode(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_Mode_t Copy_uddtMode);

/*************OUTPUT MODE****************/
// Configures Output type (Push-Pull / Open-Drain) and  speed (Low, Med, High, VHigh)
void GPIO_SetOutputConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_OType_t Copy_uddtType, GPIO_Speed_t Copy_uddtSpeed);

// Sets the pin output value (High / Low) using ODR register
void GPIO_SetPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_PinState_t Copy_uddtValue);

// Sets the pin output value atomically (High / Low) using BSRR register
void GPIO_SetAtomicPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_PinState_t Copy_uddtValue);

/* Function to write on the lower 8 bits of a specific Port */
void GPIO_SetLowerByteValue(u8 Copy_u8Port, u8 Copy_u8Value);
// Toggles the current output state of the selected pin(s)
void GPIO_TogglePin(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin);

/*************INPUT MODE*******************/
// Configures Internal Pull-up or Pull-down resistor for Input pins
void GPIO_SetInputConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_Pupd_t Copy_uddtPupd);

// Reads and returns the digital value of an input pin (0 or 1) from IDR register
GPIO_PinState_t GPIO_GetPinValue(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin);

/************* Alternative MODE**********************/
// Selects the Alternate Function number (AF0..AF15) for peripherals (USART, SPI, TIM,..etc)
void GPIO_SetAlternativeConfig(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_AltFunc_t Copy_uddtAltFunc);

/*************** Initiate and Control GPIO Pins*************/
// Initializes one or multiple pins at once using the configuration struct
void GPIO_Init(GPIO_Port_t Copy_uddtPort, const GPIO_InitConfig_t *Copy_pConfig);

// Resets the configuration and output registers of the selected pin(s) to default reset values
void GPIO_DeInit(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin);

// Freezes and locks the configuration of selected pin(s) until the next system reset using LCKR
void GPIO_LockPin(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin);
#endif /* INCLUDE_MCAL_GPIO_INTERFACE_H_ */
