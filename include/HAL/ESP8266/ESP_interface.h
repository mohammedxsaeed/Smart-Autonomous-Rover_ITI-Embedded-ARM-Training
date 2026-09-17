/*
 * ESP_interface.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Mohammed Saeed
 */

#ifndef ESP_INTERFACE_H_
#define ESP_INTERFACE_H_

// Initialize ESP8266 (Disable Echo & Set Station Mode)
void ESP_Init(void);

// Connect to a WiFi Access Point
u8 ESP_ConnectToWiFi(u8 *Copy_u8SSID, u8 *Copy_u8Password);

// Connect to a TCP Server (IP or DNS)
u8 ESP_ConnectToTCP(u8 *Copy_u8IP, u8 *Copy_u8Port);

#endif /* ESP_INTERFACE_H_ */
