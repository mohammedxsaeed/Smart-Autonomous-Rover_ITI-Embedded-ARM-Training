/*
 * ST7735_program.c
 *
 *  Created on: Sep 7, 2026
 *      Author: Mohammed Saeed
 */
#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "MCAL/SPI/SPI_interface.h"
#include "HAL/TFT/ST7735_config.h"
#include "HAL/TFT/ST7735_interface.h"
#include <math.h>

#define SWRESET   0x01
#define SLPOUT    0x11
#define COLMOD    0x3A
#define DISPON    0x29
#define CASET     0x2A
#define RASET     0x2B
#define RAMWR     0x2C

static void SendCommand(u8 Cmd)
{
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_A0, LOW);
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_CS, LOW);

    SPI_Transfer(Cmd);

    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_CS, HIGH);
}

static void SendData(u8 Data)
{
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_A0, HIGH);
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_CS, LOW);

    SPI_Transfer(Data);

    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_CS, HIGH);
}

void ST7735_Init(void)
{
    GPIO_InitConfig_t CtrlPin = {
        .Mode  = OUTPUT,
        .OType = PUSH_PULL,
        .Speed = SPEED_LOW,
        .Pupd  = NONE
    };

    CtrlPin.Pin = ST7735_PIN_A0;
    GPIO_Init(ST7735_CTRL_PORT, &CtrlPin);

    CtrlPin.Pin = ST7735_PIN_RST;
    GPIO_Init(ST7735_CTRL_PORT, &CtrlPin);

    CtrlPin.Pin = ST7735_PIN_CS;
    GPIO_Init(ST7735_CTRL_PORT, &CtrlPin);

    SPI_Init();

    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_RST, HIGH);
    Delay_ms(10);
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_RST, LOW);
    Delay_ms(10);
    GPIO_SetPinValue(ST7735_CTRL_PORT, ST7735_PIN_RST, HIGH);
    Delay_ms(150);

    SendCommand(SWRESET);
    Delay_ms(150);

    SendCommand(SLPOUT);
    Delay_ms(500);

    SendCommand(COLMOD);
    SendData(0x05); // 16-bit color

    SendCommand(DISPON);
    Delay_ms(100);
}

static void ST7735_SetAddressWindow(u8 x0, u8 y0, u8 x1, u8 y1)
{
    SendCommand(CASET);
    SendData(0x00);
    SendData(x0 + 2);
    SendData(0x00);
    SendData(x1 + 2);

    SendCommand(RASET);
    SendData(0x00);
    SendData(y0 + 1);
    SendData(0x00);
    SendData(y1 + 1);

    SendCommand(RAMWR);
}

void ST7735_DrawPixel(u8 x, u8 y, u16 color)
{
    ST7735_SetAddressWindow(x, y, x, y);
    SendData(color >> 8);
    SendData(color & 0xFF);
}

void ST7735_FillScreen(u16 color)
{
    ST7735_SetAddressWindow(0, 0, 127, 159);
    for (u16 i = 0; i < 128 * 160; i++)
    {
        SendData(color >> 8);
        SendData(color & 0xFF);
    }
}

void ST7735_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u16 color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;) {
        ST7735_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static void ST7735_SafePixel(s16 x, s16 y, u16 color)
{
    if (x < 0 || x > 127 || y < 0 || y > 159) return;
    ST7735_DrawPixel((u8)x, (u8)y, color);
}

void ST7735_DrawCircle(s16 x0, s16 y0, s16 r, u16 color)
{
    s16 x = r, y = 0, err = 0;
    while (x >= y)
    {
        ST7735_SafePixel(x0 + x, y0 + y, color);
        ST7735_SafePixel(x0 + y, y0 + x, color);
        ST7735_SafePixel(x0 - y, y0 + x, color);
        ST7735_SafePixel(x0 - x, y0 + y, color);
        ST7735_SafePixel(x0 - x, y0 - y, color);
        ST7735_SafePixel(x0 - y, y0 - x, color);
        ST7735_SafePixel(x0 + y, y0 - x, color);
        ST7735_SafePixel(x0 + x, y0 - y, color);
        y += 1;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) { x -= 1; err += 1 - 2 * x; }
    }
}

/* Small filled dot, used for the radar "blip" marking a detected object. */
void ST7735_DrawFilledCircle(s16 x0, s16 y0, s16 r, u16 color)
{
    for (s16 dy = -r; dy <= r; dy++)
        for (s16 dx = -r; dx <= r; dx++)
            if ((dx * dx + dy * dy) <= (r * r))
                ST7735_SafePixel(x0 + dx, y0 + dy, color);
}

/* Minimal 5x7 font: space, '0'-'9', 'A'-'Z'. Enough for radar labels
 * ("CM", distances, ring markers) without dragging in a full font table. */
static const u8 Font5x7[37][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // ' '
    {0x3E,0x51,0x49,0x45,0x3E}, // '0'
    {0x00,0x42,0x7F,0x40,0x00}, // '1'
    {0x42,0x61,0x51,0x49,0x46}, // '2'
    {0x21,0x41,0x45,0x4B,0x31}, // '3'
    {0x18,0x14,0x12,0x7F,0x10}, // '4'
    {0x27,0x45,0x45,0x45,0x39}, // '5'
    {0x3C,0x4A,0x49,0x49,0x30}, // '6'
    {0x01,0x71,0x09,0x05,0x03}, // '7'
    {0x36,0x49,0x49,0x49,0x36}, // '8'
    {0x06,0x49,0x49,0x29,0x1E}, // '9'
    {0x7E,0x11,0x11,0x11,0x7E}, // 'A'
    {0x7F,0x49,0x49,0x49,0x36}, // 'B'
    {0x3E,0x41,0x41,0x41,0x22}, // 'C'
    {0x7F,0x41,0x41,0x22,0x1C}, // 'D'
    {0x7F,0x49,0x49,0x49,0x41}, // 'E'
    {0x7F,0x09,0x09,0x09,0x01}, // 'F'
    {0x3E,0x41,0x49,0x49,0x7A}, // 'G'
    {0x7F,0x08,0x08,0x08,0x7F}, // 'H'
    {0x00,0x41,0x7F,0x41,0x00}, // 'I'
    {0x20,0x40,0x41,0x3F,0x01}, // 'J'
    {0x7F,0x08,0x14,0x22,0x41}, // 'K'
    {0x7F,0x40,0x40,0x40,0x40}, // 'L'
    {0x7F,0x02,0x0C,0x02,0x7F}, // 'M'
    {0x7F,0x04,0x08,0x10,0x7F}, // 'N'
    {0x3E,0x41,0x41,0x41,0x3E}, // 'O'
    {0x7F,0x09,0x09,0x09,0x06}, // 'P'
    {0x3E,0x41,0x51,0x21,0x5E}, // 'Q'
    {0x7F,0x09,0x19,0x29,0x46}, // 'R'
    {0x46,0x49,0x49,0x49,0x31}, // 'S'
    {0x01,0x01,0x7F,0x01,0x01}, // 'T'
    {0x3F,0x40,0x40,0x40,0x3F}, // 'U'
    {0x1F,0x20,0x40,0x20,0x1F}, // 'V'
    {0x3F,0x40,0x38,0x40,0x3F}, // 'W'
    {0x63,0x14,0x08,0x14,0x63}, // 'X'
    {0x07,0x08,0x70,0x08,0x07}, // 'Y'
    {0x61,0x51,0x49,0x45,0x43}  // 'Z'
};

static u8 CharToFontIndex(char c)
{
    if (c >= '0' && c <= '9') return 1 + (u8)(c - '0');
    if (c >= 'A' && c <= 'Z') return 11 + (u8)(c - 'A');
    return 0; /* space / anything unsupported */
}

void ST7735_DrawChar(u8 x, u8 y, char c, u16 color, u16 bgcolor, u8 size)
{
    u8 Local_u8Idx = CharToFontIndex(c);

    for (u8 col = 0; col < 5; col++)
    {
        u8 Local_u8Line = Font5x7[Local_u8Idx][col];
        for (u8 row = 0; row < 7; row++)
        {
            u16 Local_u16PxColor = (Local_u8Line & 0x01) ? color : bgcolor;
            for (u8 dx = 0; dx < size; dx++)
                for (u8 dy = 0; dy < size; dy++)
                    ST7735_SafePixel(x + col * size + dx, y + row * size + dy, Local_u16PxColor);
            Local_u8Line >>= 1;
        }
    }
}

void ST7735_DrawString(u8 x, u8 y, const char *str, u16 color, u16 bgcolor, u8 size)
{
    u8 Local_u8CursorX = x;
    while (*str)
    {
        ST7735_DrawChar(Local_u8CursorX, y, *str, color, bgcolor, size);
        Local_u8CursorX += (u8)(6 * size);
        str++;
    }
}
