# STM32F446RE DS18B20 Temperature Monitoring with OLED

## Overview

Bare-metal STM32F446RE firmware for reading temperature from a DS18B20 sensor and displaying the measured value on an SSD1306 OLED over I2C. The same temperature data is also transmitted to a PC through USART2 for monitoring in PuTTY.

This experiment extends the previous DS18B20 temperature-only implementation by adding an I2C OLED display.

## Hardware

- STM32 NUCLEO-F446RE
- DS18B20 temperature sensor
- SSD1306 I2C OLED display
- USB connection to PC for USART2 monitoring
- Breadboard and jumper wires

## Pin Configuration

| Device | STM32F446RE | Interface |
|---|---|---|
| DS18B20 DATA | PA6 | 1-Wire |
| OLED SCL | PB8 | I2C1 |
| OLED SDA | PB9 | I2C1 |
| USART2 TX | PA2 | UART |

OLED I2C address: `0x3C`

USART2 configuration: `115200 8N1`

## System Flow

```text
DS18B20
   │
   │ 1-Wire
   ▼
STM32F446RE
   │
   ├── I2C1 ──> SSD1306 OLED
   │
   └── USART2 ──> PC / PuTTY
```

## Firmware Features

- Direct register-level programming
- No STM32 HAL or external libraries
- DS18B20 1-Wire reset, read and temperature conversion
- SSD1306 OLED initialization and text display over I2C1
- USART2 serial output at 115200 baud
- SysTick-based microsecond delays
- Sensor error detection with `ERR` shown on the OLED
- Continuous temperature monitoring at approximately 1-second intervals

## Expected Output

### OLED

The OLED displays:

```text
TEMP
XX.X C
```

If the DS18B20 is not detected:

```text
TEMP
ERR
```

### PuTTY

The serial terminal displays the startup configuration followed by temperature readings such as:

```text
================================
STM32F446RE DS18B20 + OLED
UART2: 115200 8N1
DS18B20: PA6
OLED: I2C1 PB8/PB9
================================
Temperature: 25.6 C
```

## Source

- `main.c` — complete bare-metal implementation for DS18B20, I2C1/SSD1306 OLED and USART2.

## Media

Hardware photographs and PuTTY/OLED output screenshots will be added after the experiment is documented.
