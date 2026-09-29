# MPU6050 Motion Sensor — Test Images

Images from the current MPU6050 integration and motion-testing work on the **STM32 NUCLEO-F446RE**.

## Hardware and display

- `stm32-multisensor-hardware-setup.jpeg` — STM32 multisensor hardware setup used for testing.
- `oled-mpu6050-motion-display.jpeg` — SSD1306 OLED showing sensor data during MPU6050 testing.
- `stm32-pc-monitoring-setup.jpeg` — STM32 connected to the PC for serial monitoring.

## UART / PuTTY test output

- `putty-stm32-sensor-output.png` — Integrated STM32 sensor UART output.
- `putty-mpu6050-sensor-output-01.png` — MPU6050 sensor output during testing.
- `putty-mpu6050-sensor-output-02.png` — MPU6050 output captured during physical motion testing.
- `putty-mpu6050-sensor-output-03.png` — MPU6050 output captured during physical motion / tilt testing.

## What these images show

The images document the current hardware bring-up and testing of the MPU6050 with the existing STM32 sensor setup.

The tests include:
- Reading raw accelerometer and gyroscope data.
- Checking sensor response when the board is physically moved or tilted.
- Observing the readings on the SSD1306 OLED.
- Monitoring the same data through UART using PuTTY.
- Testing sideways orientation changes as part of the motion check.

At the current stage, the firmware is focused on **raw sensor data acquisition and verification**. A calculated tilt angle and further motion-processing features are planned for later development.

