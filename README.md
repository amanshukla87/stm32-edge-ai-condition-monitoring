# STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML

**BEC 753 – Project I | B.Tech Electronics Engineering**

This repository follows the firmware development of a multisensor condition-monitoring system built around the **STM32 NUCLEO-F446RE (STM32F446RE)**. It started with small peripheral tests and has gradually grown to include temperature, electrical and motion sensing, motor-speed measurement, serial monitoring, and display experiments.

I have kept the development stages separate so it is easier to see what has been tested, how the firmware is organized, and what is still being worked on. The current repository includes an integrated multisensor firmware example and a separate TFT display bring-up project. **TinyML inference and predictive-maintenance decisions have not yet been implemented.**

## Project hardware

- STM32 NUCLEO-F446RE development board
- DS18B20 digital temperature sensor
- INA219 voltage, current and power monitor
- MPU6050 accelerometer and gyroscope
- N20 geared DC motor with encoder
- L298N motor driver
- SSD1306 0.96-inch, 128×64 I2C OLED
- 2.8-inch, 240×320 SPI TFT module (display bring-up in progress)
- LED with current-limiting resistor and buzzer
- USB / ST-LINK virtual COM port for serial monitoring

## Firmware development stages

| Stage | Folder | What it covers |
|---|---|---|
| 01 | `01-gpio-led-blink` | Register-level GPIO configuration and LED control |
| 02 | `02-ds18b20-temperature` | Temperature acquisition using the 1-Wire protocol |
| 03 | `03-ds18b20-oled-temperature` | Displaying DS18B20 readings on the SSD1306 OLED |
| 04 | `04-ds18b20-oled-buzzer` | Temperature threshold indication using a buzzer |
| 05 | `05-ina219-power-monitoring` | INA219 voltage, current and power measurement |
| 06 | `06-mpu6050-motion-sensor` | MPU6050 accelerometer and gyroscope data with the existing sensor display/serial setup |
| 07 | `07-n20-motor-encoder-l298n-oled-uart` | N20 motor control, encoder pulse counting and RPM monitoring |
| 08 | `08-integrated-multisensor-monitor` | Combined temperature, power, motion and motor-monitoring firmware |
| 09 | `09-tft-display` | Standalone SPI TFT initialization and colour-fill testing |

Each folder represents a development step or a focused hardware test. The later stages build on the earlier work, but a standalone test should not be mistaken for a feature already integrated into the complete application.

## Current system overview

The integrated monitoring work brings together the following measurement paths:

```text
 DS18B20 ───────────── Temperature ───────┐
 INA219 ────────────── Voltage / Current ─┤
 MPU6050 ───────────── Accel. / Gyro ─────┤
 N20 + Encoder ─────── Pulse count / RPM ─┤
                                          ▼
                                   STM32F446RE
                                          │
                              ┌───────────┴───────────┐
                              ▼                       ▼
                         SSD1306 OLED             USART2
                                                      │
                                                    PuTTY

                  2.8-inch SPI TFT: separate display
                         bring-up / test stage
```

The integrated firmware source is available in [Stage 08](./firmware/08-integrated-multisensor-monitor/). The motor and encoder test is documented separately in [Stage 07](./firmware/07-n20-motor-encoder-l298n-oled-uart/).

## Stage 06: sensor interfaces and connections

The documented Stage 06 setup uses I2C1 for the INA219, SSD1306 OLED and MPU6050. The DS18B20 uses a separate 1-Wire data line.

| Device | Interface | Pin / address |
|---|---|---|
| DS18B20 | 1-Wire | PA6 |
| INA219 | I2C1 | PB8 (SCL), PB9 (SDA), address 0x40 |
| SSD1306 OLED | I2C1 | PB8 (SCL), PB9 (SDA), address 0x3C |
| MPU6050 | I2C1 | PB8 (SCL), PB9 (SDA), address 0x68 |
| Buzzer | GPIO | PB0 |
| USART2 | UART | PA2 (TX), 115200 baud |

The DS18B20 data line uses a **4.7 kΩ pull-up resistor**. Check the wiring and logic-voltage requirements of each breakout board before connecting it to the NUCLEO board.

## Stage 07–08: motor and multisensor monitoring

The motor test uses an **N20 geared DC motor with an encoder** and an L298N driver. The firmware counts encoder pulses and calculates an RPM value over a measurement interval. The Stage 07 README records the motor details and notes that the encoder's effective counts per output revolution must be confirmed for the exact motor and counting method.

Stage 08 brings the sensing and motor-monitoring work into one firmware example. The checked-in source includes routines for DS18B20 temperature acquisition, INA219 measurements, MPU6050 accelerometer/gyroscope reads, OLED output, USART2 reporting, and motor PWM/encoder pulse counting.

The MPU6050 values are currently handled as raw sensor readings in the documented implementation. More advanced vibration analysis, filtering, feature extraction, and health classification are future work—not completed predictive-maintenance functions.

## Stage 09: 2.8-inch SPI TFT display

A separate display experiment is underway using the 2.8-inch, 240×320 SPI TFT module marked **“2.8\\" TFT SPI 240x320 V1.2”**. The purpose is to develop a larger local interface for motor and sensor information.

The repository currently contains:
- A standalone TFT test source file in [`complete-code/`](./firmware/09-tft-display/complete-code/).
- Hardware setup and colour-test photographs in [`images/`](./firmware/09-tft-display/images/).
- A four-colour display-test video in [`videos/`](./firmware/09-tft-display/videos/).
- A `Drivers/` folder reserved for display-driver files and documentation.

The checked-in standalone test source uses **SPI2** and contains a colour-fill test with an ST7789-style initialization sequence. The exact controller fitted to the display module has not yet been independently confirmed, so this should be treated as an experimental driver configuration rather than confirmed controller identification.

| TFT signal | STM32 pin | Function |
|---|---|---|
| SCK / CLK | PB13 | SPI2 clock |
| MOSI / SDI | PB15 | SPI2 data to display |
| MISO / SDO | PB14 | Data from display; not required for the current write-only test |
| CS | PB12 | Chip select |
| DC / RS | PC0 | Command / data selection |
| RESET / RST | PC1 | Display reset |
| VCC | As supported by the exact module | Power |
| GND | GND | Common ground |
| LED / BL | Module-dependent | Backlight; follow the board specifications |

**Current TFT status:** basic colour-fill testing is documented, but the display is not yet part of the integrated multisensor application. The current tracked `complete-code/main.c` is a standalone colour-test program; it should not be taken as evidence that motor or all sensor readings are already being displayed on the TFT. The controller, final electrical requirements and any backlight connection still need to be verified against the actual module.

Next, I will verify the controller and power/backlight requirements, confirm stable SPI operation, test text and numeric rendering, and then move selected sensor and motor values onto the TFT. The SSD1306 OLED remains the display used by the earlier monitoring firmware.

## Firmware approach and tools

Most of the earlier peripheral examples use **C with direct STM32F446RE register access**, rather than STM32 HAL or external sensor libraries. The standalone TFT test is a separate HAL-based SPI experiment, so the repository currently contains both approaches.

Tools used:
- **STM32CubeIDE** — firmware editing, building and debugging
- **ST-LINK** — programming and debugging the NUCLEO board
- **PuTTY** — viewing USART2 output
- **Git and GitHub** — source control and project documentation

The code is organized into separate source and header files where appropriate, with application logic kept distinct from device-level routines. Some development stages are still represented by a single main source file.

## What is implemented and what comes next

**Implemented / documented in the repository**
- Register-level GPIO and LED control
- DS18B20 temperature acquisition
- SSD1306 OLED output
- Basic buzzer indication
- INA219 voltage, current and power reads
- MPU6050 accelerometer and gyroscope acquisition
- N20 motor control and encoder pulse counting / RPM calculation
- USART2 serial monitoring
- An integrated multisensor firmware example
- Standalone SPI TFT colour-fill test

**Still in development**
- Final verification and documentation of the motor encoder calibration
- Reliable TFT controller identification and display-driver validation
- TFT text/numeric rendering and integration with sensor data
- Consistent sensor sampling, filtering and vibration-feature extraction
- Collection and labelling of operating-condition data
- Lightweight TinyML model development, quantization and on-device inference
- Validation of anomaly-detection and predictive-maintenance behaviour

The aim is to develop these capabilities on the STM32 itself. Until the data pipeline, model and on-device inference are implemented and tested, the project should be described as a multisensor monitoring system under development—not as a completed AI predictive-maintenance product.

## Project information

- **Course:** BEC 753 – Project I
- **Program:** B.Tech Electronics Engineering
- **Project:** STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML
- **Platform:** STM32 NUCLEO-F446RE / STM32F446RE
- **Repository:** [amanshukla87/stm32-edge-ai-condition-monitoring](https://github.com/amanshukla87/stm32-edge-ai-condition-monitoring)

## Project guide

**Dr. Anupam Soni**  
Assistant Professor, BRSVBP Rajkiya Engineering College, Basti  
Ph.D., Indian Institute of Technology (IIT) Kanpur

## Project team

- **Aman Shukla**
- **Govind Shukla**
