# 08 — Integrated Multisensor Industrial Condition Monitoring

## Purpose
Integrated STM32F446RE firmware combining the previously tested sensor and output modules into one application.

## Integrated hardware
- DS18B20 temperature sensor
- INA219 voltage, current and power monitor (V / I / P)
- LED with current-limiting resistor
- MPU6050 accelerometer and gyroscope
- N20 DC geared motor with encoder
- L298N motor driver
- OLED display
- USART serial output monitored in PuTTY
- Passive buzzer for combined-condition alerts, where implemented

## Intended application flow
1. Initialize the clock, GPIO, I2C, UART and other peripherals used by the actual firmware.
2. Read temperature, INA219 electrical measurements and MPU6050 acceleration/gyroscope data.
3. Read encoder counts and calculate motor speed using the calibrated encoder scaling.
4. Update the OLED and serial output with sensor and motor readings.
5. Evaluate configured thresholds and combined conditions for alerts, if present in the tested source.

## Status
The project owner reports that the combined DS18B20 + INA219 + LED/resistor + MPU6050 + N20 motor/encoder + L298N + OLED + PuTTY setup is working. This folder currently documents the integration stage; the actual tested source files and verified pin map still need to be committed here.

## Engineering notes
- INA219 measures bus voltage and current through its shunt arrangement; the measured load and wiring must match the actual circuit.
- The LED needs a series current-limiting resistor.
- The MPU6050 provides accelerometer and gyroscope measurements; tilt/orientation calculations depend on the algorithm used in firmware.
- A passive buzzer requires a driven waveform/PWM to generate different tones. Do not describe variable-frequency alerts as implemented until verified in the source.
- Keep measured hardware results distinct from simulated dashboard values.
