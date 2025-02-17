# TimedOutput Library

A simple, non-blocking Arduino/ESP library for managing timed digital outputs. 
Ideal for sending timed on/off pulses to devices such as LEDs, buzzers, motors, and more. 
This library simplifies and declutters code by internally handling timing and output logic for the assigned digital pin. 
Its non-blocking design minimizes interference with the main program loop.

## Features

- **Non-blocking `millis()`-based operation** which does not use `delay()` or timer interrupts.
- **Multiple output modes**:
    - Manual on/off control
    - Single (one-time) pulse
    - Repeating pulses (with or without a counter)
- **Configurable durations** for pulse control:
    - On/off durations
    - Cycle duration
    - Duty cycle
- **Configurable logic level inversion** for active-low devices.
- **16 byte memory footprint** per instance.

## Important Note

- For accurate timing and proper operation, the `update()` method must be called periodically, typically inside the `loop()` function. If `update()` is not called frequently enough, pulse durations may be longer than intended.

## Usage

### Example Sketch

```cpp
TimedOutput led(13); // Controls LED on pin 13

void setup(){
    led.init();
    led.repeat(100,900); // Repeat pulses 100ms ON, 900ms OFF
}

void loop(){
    led.update();
    // Main program code
}

```
More detailed example in the [examples](examples/) folder

## API Reference

#### `TimedOutput(uint8_t pin, bool invertLogic = false)`
- **Creates an instance** for controlling a timed output.
- `pin`: The output pin number.
- `invertLogic`: *(Optional)* `true` for active-low, `false` for active-high.

#### `void init()`
- **Must be called on `setup()`** to initialize output pin.

#### `void update()`
- **Must be called periodically** to process timing logic.

### Manual Operation

#### `void on()`
- Turns the output **ON indefinitely**, until `off()` or another mode is called.

#### `void off()`
- Turns the output **OFF immediately**.

### Single Pulse

#### `void single(uint32_t duration)`
- Turns the output ON for `duration` milliseconds, then automatically turns OFF.

### Repeating Pulses
Repeat ON/OFF cycles indefinitely, until `off()` or another mode is called.

#### `void repeat(uint32_t durOn, uint32_t durOff)`
- `durOn`: Time in milliseconds the output stays ON.
- `durOff`: Time in milliseconds the output stays OFF.

#### `void repeat(uint32_t durOn)`
- Uses the same ON/OFF duration

#### `void repeat()`
- Restarts the previous repeat cycle with the last used values.
- May have unexpected results if previous mode wasn't repeat. Use with caution.

#### `void repeatDutyCycle(double dutyCycle, uint32_t duration)`
- `dutyCycle`: Percentage of the cycle in which the output will be ON.
- `duration`: Total duration of ON/OFF cycle in milliseconds.


### Repeating Pulses (with counter)
Run a **limited** number of ON/OFF cycles (`times` times), then turn off.

#### `void repeatX(uint8_t times, uint32_t durOn, uint32_t durOff)`
- `times`: Number of repetitions.
- `durOn`: Time in milliseconds the output stays ON.
- `durOff`: Time in milliseconds the output stays OFF.
- **Maximum 255 repetitions**.

#### `void repeatX(uint8_t times, uint32_t durOn)`
- Uses the same ON/OFF duration

#### `void repeatX(uint8_t times)`
- Restarts the previous repeat cycle with the last used values.
- May have unexpected results if previous mode wasn't repeat. Use with caution.

#### `void repeatXDutyCycle(uint8_t times, double dutyCycle, uint32_t duration)`
- `dutyCycle`: Percentage of the cycle in which the output will be ON.
- `duration`: Total duration of ON/OFF cycle in milliseconds.

