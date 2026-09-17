# Hardware Integration

The table below is extracted from configuration headers. A signal marked as inferred still requires wiring validation with a schematic or continuity test.

| Component | Signal | MCU pin | Peripheral/function | Notes |
|---|---|---|---|---|
| Bluetooth | TX/RX | PA9 / PA10 | USART1 AF7 | Cross TX and RX; 9600 baud |
| TFT ST7735 | SCK/MISO/MOSI | PA5 / PA6 / PA7 | SPI1 | Configuration names SDA for MOSI |
| TFT ST7735 | D/C, RST, CS | PB2 / PB3 / PB4 | GPIO | Active levels must follow module datasheet |
| Servo | PWM | PB6 | TIM4 CH1 AF2 | Nominal 20 ms period |
| Ultrasonic | Trigger | PB0 | GPIO output | HC-SR04 trigger |
| Ultrasonic | Echo | PB1 | GPIO/timing input | HC-SR04 echo |
| Buzzer | Output | PB5 | GPIO output | Push-pull |
| 74HC595 | DS, SHCP, STCP | PB8 / PB9 / PB10 | GPIO bit-bang | Shift data, shift clock, latch |
| Motor shield | Direction/control | PB12–PB15 | GPIO/shift interface | Enable is documented active-low |
| Motor shield | PWM channels | PA0–PA3 | TIM2 CH1–CH4 | Verify motor channel mapping |
| LED matrix | Rows/columns | PORTA / PORTB | GPIO | Exact per-line mapping is not centralized |
| IR receiver | Output | PA4 | GPIO/EXTI candidate | Module exists; runtime use requires verification |

## Power and safety

Motors and servo loads should use an appropriate external supply with a common ground to the MCU. Do not power motors from the MCU regulator. Add decoupling near the MCU, display, Bluetooth module, and shift register. Motor switching can inject noise into sensor readings and UART; use physical separation, filtering, and a star or carefully planned ground return.

Confirm that every external signal is within STM32 input voltage limits. In particular, verify Bluetooth, ESP8266, and HC-SR04 echo levels before connection. Use a level shifter or divider where required by the module and board variant.

## Bring-up order

First validate reset and SWD programming. Then test clocks and a single GPIO, followed by UART, SPI/TFT, servo PWM, ultrasonic echo timing, motor PWM with wheels lifted, and finally integrated safety behavior. Do not connect the motor supply during initial firmware bring-up.
