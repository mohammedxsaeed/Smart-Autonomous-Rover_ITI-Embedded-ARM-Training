/*
 * ST7735_config.h
 *
 *  Created on: Sep 7, 2026
 *      Author: Mohammed Saeed
 */

#ifndef ST7735_CONFIG_H_
#define ST7735_CONFIG_H_
// See SPI_config.h to change there
#define ST7735_SPI_PORT     PORTA
#define ST7735_PIN_SCK      PIN5   // SPI1_SCK
#define ST7735_PIN_SDA      PIN7   // SPI1_MOSI

#define ST7735_CTRL_PORT    PORTB
#define ST7735_PIN_A0       PIN2   // Data/Command (A0)
#define ST7735_PIN_RST      PIN3   // Reset (RST)
#define ST7735_PIN_CS       PIN4   // Chip Select (CS)

#endif
