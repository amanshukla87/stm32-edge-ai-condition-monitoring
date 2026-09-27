# STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring

**BEC 753 – Project I | 7th Semester Project**

**Project Team**
- **Aman Shukla** — Project Team Lead
- **Govind Shukla** — Project Team Member

## Overview

This project is being developed around the **STM32 NUCLEO-F446RE** as a step-by-step multisensor monitoring system.

The current work focuses on getting the hardware and firmware working reliably first. So far, the project has progressed from basic GPIO and temperature sensing to OLED display, audible indication, electrical measurements, and finally **MPU6050 motion sensing**.

The repository is kept as a record of the actual development work. Planned features are not marked as completed.

## Software and Development Tools

| Tool | Use |
|---|---|
| **STM32CubeIDE v2.2.0** | STM32 firmware development, build, flash and debugging |
| **PuTTY** | UART/serial monitoring from the PC |
| **Embedded C** | Firmware development |
| **Git/GitHub** | Version control and project documentation |

The current UART monitoring uses **115200 baud, 8-N-1**. The COM port depends on the connected PC and ST-LINK virtual COM interface.

## Hardware Progress

### Working and verified

The STM32 setup has been tested progressively with:

- **STM32 NUCLEO-F446RE**
- **DS18B20** — temperature measurement
- **INA219** — bus voltage, current and power measurement
- **SSD1306 0.96-inch 128×64 OLED** — local display
- **Buzzer** — audible indication
- **LED + 200 Ω resistor** — visual indication
- **MPU6050** — accelerometer and gyroscope data
- **PuTTY** — serial monitoring

### Current milestone

**DS18B20 + INA219 + SSD1306 OLED + buzzer + LED + MPU6050 + UART monitoring are working in the current STM32 firmware.**

The MPU6050 is connected through **I2C1** and uses address **0x68**.

## Current Sensor Interfaces

| Device | Interface | STM32 connection / address |
|---|---|---|
| DS18B20 | 1-Wire | PA6, with **4.7 kΩ pull-up resistor** |
| INA219 | I2C | 0x40 |
| SSD1306 OLED | I2C | 0x3C, PB8/PB9 |
| MPU6050 | I2C | 0x68, PB8/PB9 |
| Buzzer | GPIO | PB0 |
| External LED | GPIO | PA5 through **200 Ω resistor** |
| Serial monitoring | UART | USART2 / PuTTY |

The **4.7 kΩ pull-up resistor is used on the DS18B20 data line**, while the **200 Ω resistor is used in series with the external LED**.

The I2C devices share the STM32 I2C1 bus on **PB8 (SCL)** and **PB9 (SDA)**.

## MPU6050 Integration

The MPU6050 was first checked independently and then integrated into the STM32 firmware.

The current STM32 implementation performs:

- MPU6050 initialization through register-level I2C communication
- Device configuration
- Accelerometer X/Y/Z reading
- Gyroscope X/Y/Z reading
- Raw internal temperature register reading
- UART output through PuTTY
- OLED display of MPU6050 data
- Operation together with the existing DS18B20 and INA219 measurements

The current firmware uses the MPU6050 default I2C address **0x68**.

### Verified STM32 output

A typical verified UART output is:

```text
====================================
DS18B20
====================================
Temperature: 27.6 C

INA219:
Bus Voltage: 2.64 V
Current: 0.005 A
Power: 0.013 W
------------------------------------

MPU6050 DATA
Accel X: 17096
Accel Y: -96
Accel Z: -1516
Temperature raw: 2528
Gyro X: -46
Gyro Y: 470
Gyro Z: -61
------------------------------------
```

The same sensor information is also displayed locally on the OLED during testing.

**Note:** The MPU6050 temperature value shown above is the raw sensor register value. It is not presented as a converted temperature in °C in the current firmware. Accelerometer and gyroscope values are also currently shown as raw 16-bit readings.

## Current System Flow

```text
                 STM32 NUCLEO-F446RE
                         |
        +----------------+----------------+
        |                |                |
     DS18B20           INA219          MPU6050
   Temperature      V/I/Power       Motion Data
        |                |                |
        +----------------+----------------+
                         |
                  Sensor Processing
                         |
             +-----------+-----------+
             |                       |
        SSD1306 OLED              UART
             |                       |
       Local Display              PuTTY
             |
       Buzzer / LED
```

This represents the current working monitoring stage. The project has not yet moved to the final Edge AI inference stage.

## Firmware Progress

The firmware folders follow the actual development sequence:

```text
firmware/
├── 01-gpio-led-blink/
├── 02-ds18b20-temperature/
├── 03-ds18b20-oled-temperature/
├── 04-ds18b20-oled-buzzer/
├── 05-ina219-power-monitoring/
└── 06-mpu6050-motion-sensor/
```

The **06-mpu6050-motion-sensor** folder contains the current integrated firmware and MPU6050-specific documentation and test material.

## Current Project Status

**Status: Ongoing — multisensor hardware bring-up and firmware integration**

### Completed up to MPU6050

- GPIO and LED output
- DS18B20 temperature sensing
- SSD1306 OLED display
- Buzzer indication
- INA219 voltage/current/power monitoring
- UART monitoring through PuTTY
- MPU6050 I2C integration
- MPU6050 accelerometer and gyroscope data reading
- Combined DS18B20 + INA219 + MPU6050 monitoring in the current STM32 firmware

### Not yet completed

The following remain part of the broader project plan:

- Long-duration multisensor data collection
- Signal preprocessing and feature extraction
- CAN / RS485 integration
- Motor and encoder integration
- TinyML / Edge AI model development
- Model optimization
- STM32 on-device inference
- Final condition-classification logic

These will be added only after they are actually implemented and tested.

## Planned Edge AI Direction

The longer-term goal is to use the collected multisensor data for lightweight condition monitoring on the STM32.

```text
Sensor Data
    ↓
Data Collection
    ↓
Preprocessing
    ↓
Feature Extraction
    ↓
Model Development
    ↓
Model Optimization
    ↓
STM32 Deployment
    ↓
On-Device Inference
```

This is the planned direction, not a claim that Edge AI inference is already running on the STM32.

## Project Media

Current hardware and test images are maintained with the relevant firmware stages. The MPU6050 stage includes photos of the hardware, OLED output, PC/PuTTY monitoring, and the current complete hardware demonstration.

## Academic Information

**Course:** BEC 753 – Project I  
**Semester:** 7th Semester  
**Program:** Bachelor of Technology – Electronics Engineering  
**Project Type:** Academic Project  
**Status:** Ongoing

---

> This project is being built incrementally. I am documenting what is actually tested on the hardware at each stage rather than treating planned features as completed.
