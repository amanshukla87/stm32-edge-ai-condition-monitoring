# STM32F446RE DS18B20 + OLED + Buzzer Temperature Monitor

## Overview

This firmware stage extends the DS18B20 + SSD1306 OLED temperature monitor by adding an **active buzzer temperature alert**.

The system uses the **STM32F446RE** with register-level C programming:
- **DS18B20** for digital temperature sensing over 1-Wire
- **SSD1306 128x64 OLED** for local temperature display over I2C
- **Active buzzer** for threshold-based temperature alert
- **USART2** for serial monitoring through the PC

The firmware is written without the STM32 HAL or external sensor/display libraries.

---

## Hardware

- STM32 NUCLEO-F446RE
- DS18B20 digital temperature sensor
- 0.96-inch SSD1306 I2C OLED, 128x64
- Active buzzer
- 4.7 kΩ pull-up resistor
- Jumper wires
- USB connection for programming, debugging, and serial monitoring

---

## Pin Connections

| Component | STM32F446RE | Function |
|---|---|---|
| DS18B20 DATA | **PA6** | 1-Wire data |
| DS18B20 VCC | **3.3 V** | Sensor supply |
| DS18B20 GND | **GND** | Ground |
| 4.7 kΩ resistor | **PA6 → 3.3 V** | 1-Wire pull-up |
| OLED SCL | **PB8** | I2C1 SCL |
| OLED SDA | **PB9** | I2C1 SDA |
| OLED VCC | **3.3 V** | Display supply |
| OLED GND | **GND** | Ground |
| Buzzer signal | **PB0** | Digital output |
| Buzzer GND | **GND** | Ground |
| USART2 TX | **PA2** | Serial output |

### OLED Configuration

- Controller: SSD1306
- Resolution: 128 × 64
- I2C address: `0x3C`
- Interface: I2C1
- SCL: PB8
- SDA: PB9
- Alternate function: AF4

### Serial Configuration

- Peripheral: USART2
- TX: PA2
- Baud rate: **115200**
- Format: **8-N-1**

On the NUCLEO-F446RE, serial output can be observed through the board's **ST-LINK Virtual COM Port** when connected/configured for VCP operation.

---

## System Architecture

```text
                 DS18B20
                    │
                 1-Wire
                    │ PA6
                    ▼
             ┌──────────────┐
             │ STM32F446RE  │
             │              │
             │  Register-   │
             │   level C    │
             └──────┬───────┘
                    │
          ┌─────────┼─────────┐
          │         │         │
       I2C1       GPIO      USART2
     PB8/PB9       PB0        PA2
          │         │          │
          ▼         ▼          ▼
      SSD1306     Active     PC / PuTTY
       OLED       Buzzer
```

---

## Firmware Operation

The main loop performs the following sequence:

1. Reset and detect the DS18B20.
2. Start a temperature conversion.
3. Wait for the conversion to complete.
4. Read the DS18B20 temperature register.
5. Convert the raw sensor value to °C.
6. Display the temperature on the SSD1306 OLED.
7. Transmit the temperature through USART2.
8. Compare the temperature with the programmed alert threshold.
9. Turn the buzzer ON when the threshold is exceeded.
10. Repeat the measurement cycle.

A DS18B20 communication failure results in an `ERR` indication on the OLED and the buzzer is switched OFF.

---

## Temperature Alert

The current firmware uses a threshold of approximately **27 °C**.

| Condition | Buzzer |
|---|---|
| Temperature ≤ 27 °C | OFF |
| Temperature > 27 °C | ON |
| DS18B20 not detected | OFF |

The threshold is defined directly in the firmware and can be changed for testing or application requirements.

---

## OLED Display

During normal operation, the OLED displays the temperature in this format:

```text
TEMP

28.7 C
```

If the DS18B20 is not detected:

```text
TEMP

ERR
```

---

## Serial Output

At startup, the firmware sends an identification message similar to:

```text
================================
STM32F446RE DS18B20 + OLED + Buzzer
UART2: 115200 8N1
DS18B20: PA6
OLED: I2C1 PB8/PB9
Buzzer: PB0
================================
```

Normal temperature readings are then transmitted as:

```text
Temperature: 28.7 C
```

If the sensor is not detected:

```text
DS18B20 not detected!
```

---

## Firmware Implementation

The `main.c` file contains direct register-level implementations for:

- GPIO configuration
- SysTick-based microsecond delays
- DS18B20 1-Wire reset/read/write operations
- DS18B20 temperature conversion
- USART2 serial transmission
- I2C1 communication
- SSD1306 initialization and display control
- Temperature formatting
- Active buzzer control
- Temperature threshold detection
- Sensor error handling

No STM32 HAL or external sensor/display library is used.

---

## Project Status

**Firmware stage:** DS18B20 + SSD1306 OLED + buzzer integration

This folder represents an **incremental peripheral-integration stage** of the larger STM32 industrial condition-monitoring project.

The firmware demonstrates temperature sensing, local display, serial monitoring, and threshold-based alerting. It is not yet the final multisensor Edge AI condition-monitoring firmware.

Further stages can integrate additional sensors such as the MPU6050 and later combine multiple sensor streams for condition-monitoring and Edge AI processing.

---

## Source Code

- [`main.c`](main.c) — complete bare-metal firmware implementation
- [`images/`](images/) — hardware/demo assets for this firmware stage

## Related Project

This firmware is part of:

**STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring**

The complete project repository contains the hardware bring-up stages, firmware experiments, and planned Edge AI workflow.