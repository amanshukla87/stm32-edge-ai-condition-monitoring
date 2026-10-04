# STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring

**BEC 753 – Project I | Electronics Engineering**

Firmware development on the **STM32 NUCLEO-F446RE / STM32F446RE**, progressing from register-level GPIO and sensor interfacing to a combined multisensor monitoring application.

The repository documents the implemented firmware stages in order. The current implementation is at the **MPU6050 integration stage**; Edge AI inference is not yet part of the firmware.

## Hardware

- STM32 NUCLEO-F446RE / STM32F446RE
- DS18B20 digital temperature sensor
- INA219 voltage/current/power monitor
- MPU6050 accelerometer and gyroscope
- SSD1306 0.96-inch 128×64 I2C OLED
- External LED
- Buzzer
- USB / ST-LINK virtual COM interface

## Firmware Stages

| Stage | Firmware | Main function |
|---|---|---|
| 01 | `01-gpio-led-blink` | Register-level GPIO output and LED control |
| 02 | `02-ds18b20-temperature` | DS18B20 temperature acquisition over 1-Wire |
| 03 | `03-ds18b20-oled-temperature` | Temperature display on SSD1306 OLED |
| 04 | `04-ds18b20-oled-buzzer` | Temperature threshold and buzzer indication |
| 05 | `05-ina219-power-monitoring` | Voltage, current and power measurement |
| 06 | `06-mpu6050-motion-sensor` | MPU6050 motion sensing with the existing monitoring stack |

Each stage is kept as a separate firmware example so the progression from a simple peripheral test to the integrated system can be followed from the source code.

## Current Firmware

Stage 06 combines the previously implemented devices:

```text
DS18B20 ────────┐
INA219 ─────────┤
MPU6050 ────────┤
                ▼
          STM32F446RE
                │
        ┌───────┴───────┐
        ▼               ▼
   SSD1306 OLED       USART2
                        │
                      PuTTY
```

The I2C devices share **I2C1**:

| Device | Interface | Connection / Address |
|---|---|---|
| DS18B20 | 1-Wire | PA6 |
| INA219 | I2C1 | PB8/PB9, address 0x40 |
| SSD1306 OLED | I2C1 | PB8/PB9, address 0x3C |
| MPU6050 | I2C1 | PB8/PB9, address 0x68 |
| Buzzer | GPIO | PB0 |
| USART2 | UART | PA2 TX, 115200 baud |

The DS18B20 data line uses a **4.7 kΩ pull-up resistor**.

## MPU6050

The current motion-sensing firmware configures the MPU6050 through register-level I2C communication.

Implemented:

- MPU6050 initialization
- Accelerometer X/Y/Z acquisition
- Gyroscope X/Y/Z acquisition
- Raw internal temperature register acquisition
- UART output
- OLED output
- Integration with DS18B20 and INA219 measurements

The current firmware reports MPU6050 accelerometer and gyroscope values as **raw 16-bit readings**. Tilt-angle calculation and higher-level motion features are not currently implemented.

## Firmware Structure

The firmware is organized by responsibility rather than keeping the complete implementation in a single `main.c`.

Typical stage structure:

```text
main.c
*.c / *.h driver modules
README.md
images/
video/
```

For the integrated Stage 06 firmware:

```text
06-mpu6050-motion-sensor/
├── main.c
├── delay.c / delay.h
├── i2c.c / i2c.h
├── mpu6050.c / mpu6050.h
├── ina219.c / ina219.h
├── ds18b20.c / ds18b20.h
├── onewire.c / onewire.h
├── oled.c / oled.h
├── uart.c / uart.h
├── buzzer.c / buzzer.h
├── images/
├── video/
└── README.md
```

`main.c` contains the application sequence and coordinates the modules. Device communication and hardware-specific implementation remain in their respective source files.

## Development Approach

The firmware examples use **C and direct STM32F446RE register access**. STM32 HAL and external sensor libraries are not used in these examples.

Main areas covered in the repository:

- GPIO configuration
- 1-Wire communication
- I2C1 communication
- USART2 communication
- SysTick-based timing
- Sensor register configuration
- Sensor data acquisition
- OLED display control
- Basic threshold-based indication
- Modular embedded firmware structure

## Tools

- **STM32CubeIDE v2.2.0** — development, build and debugging
- **ST-LINK** — programming and debugging
- **PuTTY** — UART monitoring
- **Git / GitHub** — version control and documentation

## Current Status

**Current stage: multisensor firmware integration through MPU6050.**

Implemented through Stage 06:

- GPIO / LED control
- DS18B20 temperature measurement
- SSD1306 OLED display
- Buzzer indication
- INA219 voltage, current and power monitoring
- MPU6050 accelerometer and gyroscope acquisition
- UART monitoring
- Modular driver-based firmware structure

The next development work can build on this verified sensing layer for data collection, signal processing, feature extraction, communication interfaces, and eventually lightweight Edge AI deployment.

## Project Information

**Course:** BEC 753 – Project I  
**Program:** B.Tech Electronics Engineering  
**Project:** STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring  
**Platform:** STM32 NUCLEO-F446RE / STM32F446RE  
**Repository:** `amanshukla87/stm32-edge-ai-condition-monitoring`

## Project Team

- **Aman Shukla**
- **Govind Shukla**

