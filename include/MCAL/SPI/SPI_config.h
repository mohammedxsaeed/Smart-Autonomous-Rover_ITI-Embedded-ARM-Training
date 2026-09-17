/*
 * SPI_config.h
 *
 *  Created on: Aug 26, 2026
 *      Author: Mohammed Saeed
 */
#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#define SLAVE    0
#define MASTER   1

#define SPI_CHANNEL_1    1
#define SPI_CHANNEL_2    2
#define SPI_CHANNEL_3    3

#define SPI_SELECTED_CHANNEL   SPI_CHANNEL_1

/*
 * Options:
 * - MASTER
 * - SLAVE
 */
#define SPI_INITIAL_ROLE       MASTER

#define SPI_PORT               PORTA
#define SPI_PIN_SCK            PIN5
#define SPI_PIN_MISO           PIN6
#define SPI_PIN_MOSI           PIN7

#endif /* SPI_CONFIG_H_ */
