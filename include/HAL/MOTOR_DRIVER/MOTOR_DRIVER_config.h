/*
 * MOTOR_DRIVER_config.h
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MOTOR_DRIVER_CONFIG_H_
#define MOTOR_DRIVER_CONFIG_H_

#define SHIFT_REG_PORT     PORTB
#define SHIFT_DATA_PIN     PIN12   // DIR_SER - D8
#define SHIFT_CLK_PIN      PIN13   // DIR_CLK - D4
#define SHIFT_LATCH_PIN    PIN14   // DIR_LATCH - D12
#define SHIFT_EN_PIN       PIN15   // DIR_EN (Active Low) - D7
// PWM Pins
#define TIM2_PORT       PORTA
#define TIM2_CH1_PIN    PIN0  //D11
#define TIM2_CH2_PIN    PIN1  //D3
#define TIM2_CH3_PIN    PIN2  //D5
#define TIM2_CH4_PIN    PIN3  //D6

#endif /* MOTOR_DRIVER_CONFIG_H_ */
