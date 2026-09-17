# Drivers

This document describes the drivers found in the repository. Register names and settings below are based on the current source; physical behavior still requires bench validation.

## MCAL drivers

### RCC / MRCC

`MRCC_init()` selects HSI through `SYSTEM_CLK` in `MRCC_config.h`. `MRCC_EN_peripheral_CLK()` gates GPIOA/GPIOB, TIM2, TIM3, TIM4, TIM1, USART1, and SPI1 in `main.c`. The configuration comments support HSE/PLL options, but the active selection is HSI. The source does not establish the expected 84 MHz PLL configuration; the active clock path is therefore not verified as 84 MHz.

### GPIO

`GPIO_Init()` configures mode, output type, speed, pull configuration, and alternate functions through STM32 GPIO register structures. `GPIO_SetPinValue`, `GPIO_TogglePin`, and input helpers abstract ODR/IDR access. Component configuration identifies the following important pins: PB5 buzzer, PB6 TIM4 servo, PB0/PB1 ultrasonic trigger/echo, PB2/PB3/PB4 TFT D/C/reset/CS, PA5/PA6/PA7 SPI1, PA9/PA10 USART1, PB8/PB9/PB10 shift-register signals, and PB12–PB15 motor-shield control signals.

### SysTick

`SYSTICK_config.h` selects the AHB clock and periodic mode. `Start_OS()` requests a periodic interval calculated from the configured 1 ms tick and the assumed 16 MHz HSI path. The ISR callback sets a scheduler flag rather than executing application tasks inside interrupt context.

### Timers

TIM2 is configured for motor PWM on PA0–PA3. TIM4 channel 1 is configured for the servo on PB6 with `PSC=15` and `ARR=19999`, producing a nominal 20 ms period if the timer clock is 16 MHz. TIM1 is used by the shift-register auto-refresh interrupt with `PSC=15999` and `ARR=1`; its exact refresh frequency depends on the active timer clock and should be measured.

### USART

USART1 is active at 9600 baud on PA9/PA10 with AF7. The driver exposes initialization, byte/string transmission, receive, and RXNE availability. Transmission and reception use status-flag polling; there is no evidence of a ring buffer in the active implementation.

### SPI

SPI1 is selected as master on PA5/PA6/PA7. The driver configures software management and a baud-rate prescaler through CR1, then uses polling on TXE/RXNE in `SPI_Transfer`. This is simple and deterministic for a small TFT but is blocking during each byte transfer.

## HAL drivers

### Motor driver

The motor HAL combines PB12–PB15 shift-register direction/control signals with TIM2 PWM channels on PA0–PA3. Direction and duty-cycle behavior should be verified against the connected motor shield because the source contains board-specific comments and active-low enable semantics.

### Bluetooth

`BTM` wraps USART1 and provides byte, string, receive, availability, and decimal-number functions. The application constructs telemetry JSON incrementally. Command framing and parsing are handled in the application; the HAL itself is a transport wrapper.

### Ultrasonic

The HC-SR04 HAL drives PB0 and samples PB1. It uses the project timing facilities to measure echo duration and convert it to distance. A zero reading is converted to 400 cm in `App_UltrasonicTask`, then the safety check stops the rover at or below 30 cm.

### Servo

The servo uses TIM4 CH1 on PB6. The implementation clamps angles above 180 degrees and maps angle to a pulse-width expression beginning at 1000 timer units. The configured radar sweep range must be read from the application rather than assumed to be 30–150 degrees.

### Shift register and LED matrix

`STP` bit-bangs data, shift clock, and latch clock on PB8–PB10. `STP_StartAutoRefresh` uses a TIM1 update interrupt to refresh columns. The matrix HAL prepares scrolling frames and the interrupt refreshes the output independently of the foreground task.

### ST7735 TFT

The TFT driver uses SPI1 plus PB2/PB3/PB4 for data/command, reset, and chip select. It provides initialization and drawing primitives used by the radar screen. The radar task erases the previous line, draws the current line, and tracks one distance blip.

### Buzzer

The buzzer is on PB5. The scheduled `BUZZER_Task` implements startup, reversing, alert, and mode-switch patterns using state and step counters. Separate helper functions (`Tone`, `Delay_ms`, `Delay_us`) are explicitly blocking and should not be called from time-critical callbacks.

### Additional modules

The repository also contains EXTI/NVIC, IR remote, ESP8266, DAC, seven-segment, SH5461AS, and safety-policy modules. Their presence does not prove that each is part of the active rover runtime; see [implementation verification](implementation-verification.md).
