# Communication Protocol

## Transport

The active Bluetooth abstraction uses USART1 on PA9/PA10 at 9600 baud. USART configuration enables the transmitter and receiver and exposes polling-based byte operations. Messages are emitted with `\r\n` after telemetry frames.

## Telemetry

The application constructs telemetry as:

```json
{"distance":45,"angle":90,"speed":50,"direction":"FORWARD","mode":"MANUAL"}
```

The active mode string observed in the source includes `MANUAL` and `PHONE_AUTONOMY`. Direction values include `FORWARD`, `BACKWARD`, `LEFT`, `RIGHT`, and `STOP`. Numeric fields are serialized incrementally through `BTM_SendNumber`.

## Radar event

The expected radar event shape is:

```json
{"event":"radar","angle":75,"distance":32}
```

Whether the event is emitted on every scan step, and its exact terminator, must be verified against the active control/radar implementation before writing a client parser.

## Command handling

Bluetooth receive availability is exposed by `BTM_IsDataAvailable()`, while command interpretation resides above the transport wrapper. A robust client should send complete framed commands, tolerate partial reads, reject malformed values, and avoid assuming that one UART read equals one application message.

## Heartbeat and safety

The safety policy defines `SAFETY_HEARTBEAT_TIMEOUT_MS=1000U`. If the age of the last valid control command exceeds that limit, the intended decision is `SAFETY_STOP_LINK`. The physical stop path must be validated with the motors disconnected first. The source should be reviewed to confirm that every valid command refreshes the heartbeat timestamp and that timeout checks run in every control mode.

## Recommended protocol hardening

Add explicit message framing, a maximum length, checksum or CRC, acknowledgement/error responses, sequence numbers, and a parser state machine. These changes would protect against truncation and noise without changing the underlying USART driver.
