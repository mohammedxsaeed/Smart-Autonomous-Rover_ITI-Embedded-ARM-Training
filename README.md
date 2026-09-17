# Smart Autonomous Rover

Bare-metal STM32F401CCU6 firmware for a remotely controlled and obstacle-aware rover. The project was developed as an ITI Embedded Systems graduation project and demonstrates register-level peripheral programming, layered driver architecture, and a lightweight cooperative scheduler.

## Capabilities

The firmware integrates DC motor control through TIM2 PWM, Bluetooth communication through USART1, an HC-SR04 ultrasonic sensor, an SG90 servo scanner on TIM4, an ST7735 TFT radar view over SPI1, an 8×8 LED-matrix path through a 74HC595 shift register, and non-blocking buzzer state updates. The source tree also contains reusable drivers for IR remote input, ESP8266, DAC, seven-segment, SH5461AS, EXTI, and NVIC; their integration status is documented in the verification report.

## Architecture

```text
LIB → MCAL → HAL → OS Scheduler → APP
```

- **LIB:** portable types and bit-manipulation macros.
- **MCAL:** RCC, GPIO, NVIC, EXTI, SysTick, timers, USART, and SPI register abstractions.
- **HAL:** motors, Bluetooth, ultrasonic, servo, TFT, shift register, LED matrix, buzzer, and safety policy.
- **OS Scheduler:** 1 ms SysTick flag with cooperative periodic tasks.
- **APP:** rover modes, command handling, safety decisions, telemetry, display, and radar rendering.

## Project Layout

```text
include/    Public interfaces, register definitions, and configuration
src/        Application and driver implementation files
system/     CMSIS/startup and STM32 support code
ldscripts/  Linker scripts
Debug/      Local build output; ignored by Git
```

## Toolchain

The project is an Eclipse/STM32CubeIDE project for the STM32F401 family. Open the repository as an existing project in STM32CubeIDE, select the appropriate ARM GNU toolchain, verify the target/debugger settings, and build the project. The archived `Debug/` output is intentionally excluded from version control because it is generated and machine-specific.

## Important Verified Configuration

- MCU family: STM32F401xC; project context identifies STM32F401CCU6.
- Active system-clock selection: HSI in `include/MCAL/RCC/MRCC_config.h`.
- SysTick configuration: periodic mode, 1 ms OS tick, AHB source.
- Bluetooth: USART1, PA9/PA10, 9600 baud.
- TFT SPI: SPI1, PA5/PA6/PA7; control pins PB2/PB3/PB4.
- Servo: PB6 / TIM4 channel 1.
- Ultrasonic: PB0 trigger and PB1 echo.
- Safety thresholds: stop at 30 cm, caution at 60 cm, link timeout at 1000 ms, sensor timeout at 250 ms.

These values are extracted from the current source and should be rechecked against the physical wiring before energizing motors.

## Documentation

- [Technical manual](docs/technical-manual.md)
- [Architecture](docs/architecture.md)
- [Drivers](docs/drivers.md)
- [Application and scheduler](docs/application-and-scheduler.md)
- [Communication protocol](docs/communication-protocol.md)
- [Hardware integration](docs/hardware-integration.md)
- [Testing and troubleshooting](docs/testing-and-troubleshooting.md)
- [Implementation verification and discrepancies](docs/implementation-verification.md)

## Safety Notice

Test with the wheels lifted or motor power disconnected first. The firmware contains software stop logic, but it is not a substitute for a physical emergency stop, current limiting, correct grounding, or a validated power design.

## License

No license file was supplied in the archive. Add a license before distributing the source publicly.
