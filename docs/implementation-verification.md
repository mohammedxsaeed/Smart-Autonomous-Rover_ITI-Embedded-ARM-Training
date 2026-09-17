# Implementation Verification and Discrepancies

This report separates source-confirmed behavior from project brief assumptions.

## Confirmed

- STM32F4 register-structured drivers exist for RCC, GPIO, SysTick, timers, USART, SPI, NVIC, and EXTI.
- The active clock selection is HSI, not a verified 84 MHz PLL configuration.
- USART1 is configured for 9600 baud on PA9/PA10.
- SPI1 is configured as master on PA5/PA6/PA7.
- TIM4 CH1 drives the servo on PB6; TIM2 channels support motor PWM.
- A cooperative SysTick-flag scheduler and task control block exist.
- ST7735 radar drawing uses polar-to-Cartesian conversion with cosine/sine, a 1.4 scale factor, coordinate clamping, line erasure, and a distance blip.
- Safety policy constants are 30 cm stop, 60 cm caution, 250 ms sensor timeout, and 1000 ms heartbeat timeout.
- Additional modules exist for IR remote, ESP8266, DAC, seven-segment, and SH5461AS.

## Discrepancies or limitations

1. **84 MHz claim:** the active `SYSTEM_CLK` is HSI and the scheduler comments assume a 16 MHz path. The 84 MHz target is not established by the active configuration.
2. **Zero blocking:** SPI transfer polls TXE/RXNE, and buzzer helper functions use `Delay_ms`/`Delay_us` and busy loops. The application is scheduler-oriented but the whole firmware is not strictly non-blocking.
3. **Dynamic task management:** suspend/resume functions exist, but task creation is fixed-array registration and there is no heap-based dynamic allocation.
4. **JSON command protocol:** telemetry is manually serialized as JSON-like text. The repository must be checked before claiming a full JSON parser for incoming commands.
5. **Task periods:** documentation must use the exact `OS_CreateTask` calls in `main.c`; comments are not sufficient evidence.
6. **Driver integration:** not every HAL module in the tree is necessarily initialized by `main()`. Presence of a driver is not proof of active product integration.
7. **Hardware map:** several display and matrix mappings are spread across configuration files. A schematic or continuity test is still required.
8. **Safety independence:** software stop decisions are not a substitute for a physical emergency-stop circuit or power-stage protection.

## Classification policy

- **Fully implemented:** active initialization and reachable runtime calls are present.
- **Partially implemented:** APIs or driver logic exist, but integration or error handling is incomplete.
- **Declared but unused:** symbols exist without a reachable call from the active application.
- **Comment-only:** statements exist in comments without executable evidence.
- **Not verified:** the source does not provide enough evidence to make a claim.

## Security review

No credential-bearing files were found in the extracted project during the initial scan. ESP8266 exposes an API accepting SSID and password parameters, but no actual credentials were found. The repository `.gitignore` excludes common secret formats and local environment files; review the staged diff before publication.
