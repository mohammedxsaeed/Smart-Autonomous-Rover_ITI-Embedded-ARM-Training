/*
 * ST7735_interface.h
 *
 *  Created on: Sep 7, 2026
 *      Author: Mohammed Saeed
 */

#ifndef ST7735_INTERFACE_H_
#define ST7735_INTERFACE_H_


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void ST7735_Init(void);
void ST7735_DrawPixel(u8 x, u8 y, u16 color);
void ST7735_FillScreen(u16 color);
void ST7735_DrawRect(u8 x, u8 y, u8 w, u8 h, u16 color);
void ST7735_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u16 color);
void ST7735_DrawCircle(s16 x0, s16 y0, s16 r, u16 color);
void ST7735_DrawFilledCircle(s16 x0, s16 y0, s16 r, u16 color);
void ST7735_DrawChar(u8 x, u8 y, char c, u16 color, u16 bgcolor, u8 size);
void ST7735_DrawString(u8 x, u8 y, const char *str, u16 color, u16 bgcolor, u8 size);

#endif
