/*
 * USART_config.h
 *
 *  Created on: Aug 30, 2026
 *      Author: Mohammed Saeed
 */

#ifndef USART_CONFIG_H_
#define USART_CONFIG_H_

// Define system clocks for Baud Rate calculation
#define USART_APB1_CLOCK    16000000UL
#define USART_APB2_CLOCK    16000000UL

// ENABLE or DISABLE USART instances
#define USART1_STATUS       1
#define USART2_STATUS       0
#define USART6_STATUS       0

// USART1 Configuration
#if USART1_STATUS == 1
    #define USART1_BAUD_RATE    9600
    #define USART1_TX_PORT      PORTA
    #define USART1_TX_PIN       PIN9		//Connect to RX
    #define USART1_RX_PORT      PORTA
    #define USART1_RX_PIN       PIN10 		//Connect to TX
    #define USART1_AF           AF7
#endif

// USART2 Configuration
#if USART2_STATUS == 1
    #define USART2_BAUD_RATE    9600
    #define USART2_TX_PORT      PORTA
    #define USART2_TX_PIN       PIN2
    #define USART2_RX_PORT      PORTA
    #define USART2_RX_PIN       PIN3
    #define USART2_AF           AF7
#endif

// USART6 Configuration
#if USART6_STATUS == 1
    #define USART6_BAUD_RATE    9600
    #define USART6_TX_PORT      PORTA
    #define USART6_TX_PIN       PIN11
    #define USART6_RX_PORT      PORTA
    #define USART6_RX_PIN       PIN12
    #define USART6_AF           AF8
#endif

#endif /* USART_CONFIG_H_ */
