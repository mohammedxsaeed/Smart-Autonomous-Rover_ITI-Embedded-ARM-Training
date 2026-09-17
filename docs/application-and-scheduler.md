# Application and Scheduler

## Scheduler model

The scheduler uses a ten-entry `Task_t` array indexed by priority. Each entry stores a callback, periodicity, first-delay counter, and state. `OS_CreateTask()` writes the entry, `Start_OS()` configures periodic SysTick, and `OS_Update()` invokes `Scheduler()` when the ISR flag is set.

Priority is represented by array order: lower indexes execute first. There is no context switching and no task preemption. A long callback delays every later callback and can cause missed periods. `OS_SuspendTask()` and `OS_ResumeTask()` provide dynamic state control, but there is no separate elapsed-time accumulator; `FirstDelay` is reloaded to `Periodicity - 1` after execution.

## Timing model

With `OS_TICK_TIME_MS=1`, a task configured with periodicity `N` is intended to run approximately every `N` milliseconds. The actual interval depends on callback execution time and the clock assumptions in `Start_OS()`. The task function runs in foreground context, while SysTick only sets `OS_u8TickFlag`.

## Application responsibilities

The active application combines manual and phone-autonomy modes. Control logic receives Bluetooth commands, updates direction and speed, applies heartbeat/safety behavior, and stops the rover when the ultrasonic distance reaches the stop threshold. Telemetry sends distance, servo angle, speed, direction, and mode as a CRLF-terminated JSON object. The display task updates scrolling text when direction changes and advances the frame periodically. The radar task steps the servo and broadcasts scan events where implemented; the TFT task renders the scan.

## Expected task inventory

The final task table must be read from the `OS_CreateTask` calls in `main.c`. The source is the authority; do not infer periods from function names or comments. Record for each task its priority, period, first delay, callback, shared variables, driver calls, and safety effect. The documented callbacks include buzzer, control, display, telemetry, radar, ultrasonic, and TFT radar functions, but their exact configured periods should be copied from the source during maintenance.

## Safety behavior

`SAFETY_EvaluateFrontDistance()` distinguishes clear, caution, sensor-timeout, obstacle-stop, link-stop, and invalid-command decisions. Current constants are 30 cm for obstacle stop, 60 cm for caution, 250 ms for sensor timeout, and 1000 ms for heartbeat timeout. `App_UltrasonicTask` directly calls `StopRover()` when distance is at or below the stop threshold.

## Concurrency risks

The scheduler flag and several application state variables are shared between interrupt and foreground contexts. Variables written by ISRs must remain `volatile`, and multi-byte reads/writes should be reviewed for atomicity. The TIM1 shift-register ISR accesses an auto-refresh buffer while the foreground may update it; a double-buffer or critical section would make frame changes safer.
