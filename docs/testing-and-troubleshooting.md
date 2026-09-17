# Testing and Troubleshooting

## Bring-up checklist

| ID | Test | Procedure | Expected result |
|---|---|---|---|
| T01 | Build | Import project and build from a clean configuration | ELF is generated without errors |
| T02 | Clock | Toggle a GPIO or measure a timer output | Measured frequency matches configured clock assumptions |
| T03 | UART | Connect a USB-UART adapter at 9600 baud | Telemetry is readable and line-terminated |
| T04 | SPI/TFT | Initialize display and draw a fixed color/line | Correct orientation and stable image |
| T05 | Servo | Command known angles with motor supply disconnected | Pulse period and position are within servo limits |
| T06 | Ultrasonic | Measure known distances and timeout/no-echo cases | Distance is plausible; timeout is safe |
| T07 | Motor | Lift wheels, command each direction and speed | Correct direction and PWM response |
| T08 | Matrix | Run scrolling text for several minutes | No flicker, ghosting, or missed refresh |
| T09 | Safety | Place obstacle below 30 cm | Rover stops and remains stopped as designed |
| T10 | Link timeout | Stop sending valid control commands | Motor stop occurs within the configured timeout |
| T11 | Scheduler | Instrument callbacks with GPIO or trace pins | Periods and priority order are acceptable |

Record actual measurements, firmware revision, board revision, supply voltage, and environmental conditions for each physical test. The repository does not contain evidence that these tests have been completed, so their status should remain “Not executed” until bench validation.

## Common failure modes

**No UART data:** check PA9/PA10 cross-over, common ground, AF7, baud rate, and USART1 clock gating. **No TFT output:** confirm SPI1 pins, CS/DC/RST levels, display power, and controller variant. **Incorrect servo motion:** measure TIM4 clock, confirm APB1 timer clock multiplication, and validate pulse limits. **Incorrect distance:** verify echo voltage, trigger timing, timer units, and sensor timeout handling. **Unexpected motor motion:** disconnect motor power, inspect active-low enable and direction mapping, and test `StopRover()` in isolation. **Matrix artifacts:** verify latch timing, bit order, refresh interrupt frequency, and shared-buffer updates.

## Review priorities

Before claiming “zero blocking,” search for `while` loops around peripheral flags and calls to `Delay_ms`/`Delay_us`. Before claiming an 84 MHz clock, calculate the active RCC path rather than relying on project context. Before publishing, scan the tree for credentials and remove generated binaries from Git history.
