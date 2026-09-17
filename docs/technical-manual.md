# Smart Autonomous Rover — Technical Manual

## Contents

1. [Executive summary](#executive-summary)
2. [Architecture](#architecture)
3. [ITI curriculum alignment](#iti-curriculum-alignment)
4. [MCAL and HAL drivers](#mcal-and-hal-drivers)
5. [Scheduler and application](#scheduler-and-application)
6. [Radar mathematics](#radar-mathematics)
7. [Communication](#communication)
8. [Hardware integration](#hardware-integration)
9. [Build and validation](#build-and-validation)
10. [Verification status](#verification-status)

## Executive summary

Smart Autonomous Rover is a bare-metal STM32F401-family firmware project for a motorized rover with Bluetooth control, obstacle sensing, servo scanning, LED-matrix status output, TFT radar visualization, and buzzer feedback. It demonstrates the ITI Embedded Systems Track through register-level C, peripheral clock gating, GPIO alternate functions, timer PWM, USART, SPI, interrupt control, and a layered driver structure.

The strongest custom contribution is the integration of multiple time-sensitive components under a cooperative SysTick scheduler. The implementation is not a real-time operating system and is not completely free of blocking code. Instead, it provides a lightweight foreground scheduler while selected low-level transfers and legacy sound helpers still poll or delay.

## Architecture

The intended dependency direction is `LIB → MCAL → HAL → OS → APP`. `main.c` composes the system and starts the scheduler. Detailed diagrams and file responsibilities are in [Architecture](architecture.md).

The source contains both active rover features and reusable training modules. This is valuable for an ITI portfolio, but the active product boundary should be made clearer in future revisions by separating demos from production modules and removing unused drivers from the application build.

## ITI curriculum alignment

| ITI topic | Project evidence | Engineering significance |
|---|---|---|
| Embedded C | Separate `.c`/`.h` drivers, enums, structs, callbacks | Maintainable firmware organization |
| Bit manipulation | `SET_BIT`, `CLR_BIT`, `GET_BIT`, register masks | Direct hardware control |
| Memory-mapped I/O | Peripheral base addresses and register structs | MCU register abstraction |
| RCC and GPIO | Clock gating, modes, pull configuration, AF selection | Correct peripheral bring-up |
| Timers | TIM2 motor PWM, TIM4 servo PWM, TIM1 refresh interrupt | Actuator control and periodic work |
| SysTick | 1 ms periodic tick and callback flag | Deterministic time base |
| USART | USART1 polling transport and Bluetooth wrapper | Embedded communication |
| SPI | SPI1 master driver for ST7735 | Display integration |
| NVIC/interrupts | TIM1 refresh and interrupt control modules | Responsive peripheral service |
| HAL design | Motor, ultrasonic, servo, TFT, matrix, buzzer wrappers | Hardware abstraction |

Custom extensions include TFT radar graphics, a cooperative task scheduler, manually serialized telemetry, automatic matrix refresh, and a safety policy. The active implementation and limitations are documented in [Implementation Verification](implementation-verification.md).

## MCAL and HAL drivers

The MCAL configures internal STM32 resources while the HAL composes them into component behavior. See [Drivers](drivers.md) for register-level responsibilities, active settings, APIs, timing, and limitations.

The most important active path is:

```text
RCC → GPIO/AF → TIM2 PWM → motor driver
RCC → GPIO/AF → USART1 → BTM → application commands/telemetry
RCC → GPIO/AF → SPI1 → ST7735 → TFT radar
RCC → TIM4 PWM → servo scanner
GPIO/timing → ultrasonic → safety policy → StopRover
```

## Scheduler and application

The scheduler owns ten statically allocated task slots. SysTick sets a flag and the main loop calls `OS_Update()`. The scheduler iterates in index order, runs ready callbacks, and reloads each callback's delay. This provides cooperative priority ordering, but a callback that blocks or runs too long delays all subsequent work.

The application updates rover state, reads the ultrasonic sensor, stops on dangerous distance, controls the servo sweep, generates matrix frames, draws TFT radar graphics, and sends telemetry. See [Application and Scheduler](application-and-scheduler.md) for the execution model and concurrency review.

## Radar mathematics

The TFT task converts a polar scan point into display coordinates:

\[
X = X_{center} - (R \times 1.4 \times \cos(\theta))
\]

\[
Y = Y_{center} - (R \times 1.4 \times \sin(\theta))
\]

The implementation clamps the distance to the radar maximum and clamps the result to the 128×160 display bounds. It erases the previous line, draws the current green sweep line, and draws a red blip for valid distances. The servo direction change triggers a background redraw.

## Communication

USART1 carries Bluetooth traffic at 9600 baud. Telemetry is emitted as a CRLF-terminated JSON object containing distance, angle, speed, direction, and mode. The transport wrapper is polling-based. Message framing, parser robustness, and heartbeat behavior require the validation steps in [Communication Protocol](communication-protocol.md).

## Hardware integration

The important pin mappings, power guidance, bring-up order, and safety cautions are in [Hardware Integration](hardware-integration.md). Motor and servo power must be treated separately from MCU logic power, with a shared reference ground and appropriate protection.

## Build and validation

This is an Eclipse/STM32CubeIDE project with CMSIS/startup sources and linker scripts. Open the project in STM32CubeIDE, select an ARM GNU toolchain, clean-build, flash through the supported debugger, and validate one peripheral at a time. The generated `Debug/` directory is excluded from Git because it contains machine-specific build artifacts.

The full test matrix is in [Testing and Troubleshooting](testing-and-troubleshooting.md). No physical test result should be presented as complete unless it is recorded against a board and firmware revision.

## Verification status

The source is the authority. The project brief describes an 84 MHz, zero-blocking rover, while the active source selects HSI and still contains blocking polling/delay helpers. These differences do not reduce the educational value of the project; documenting them accurately demonstrates engineering maturity. See [Implementation Verification](implementation-verification.md) for the complete discrepancy register.
