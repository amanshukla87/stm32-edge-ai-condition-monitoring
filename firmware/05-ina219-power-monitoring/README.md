# STM32F446RE INA219 Power and Temperature Monitoring

## Overview

This firmware experiment extends the STM32F446RE monitoring setup by combining DS18B20 temperature measurement with INA219 electrical measurements over I2C.

The system continuously monitors temperature, bus voltage, current, and calculated power. The measured values are also transmitted through USART2 for live monitoring using a serial terminal such as PuTTY.

**Serial configuration:** USART2 at **115200 baud, 8-N-1**.

## Hardware

- STM32 NUCLEO-F446RE
- INA219 current and power monitoring module
- DS18B20 waterproof temperature sensor
- 0.96-inch SSD1306 I2C OLED display
- Green LED
- Resistor for the DS18B20 data line
- Jumper wires
- USB cable
- Breadboard / prototyping connections

## Interfaces

| Device | STM32F446RE Pin | Interface / Function |
|---|---|---|
| DS18B20 DATA | PA6 | 1-Wire temperature interface |
| OLED SCL | PB8 | I2C1 SCL |
| OLED SDA | PB9 | I2C1 SDA |
| INA219 SCL | PB8 | I2C1 SCL |
| INA219 SDA | PB9 | I2C1 SDA |
| UART TX | PA2 | USART2 serial output |
| Green LED | GPIO output | Status indication |

The OLED and INA219 share the STM32F446RE I2C1 bus through PB8 (SCL) and PB9 (SDA). The DS18B20 uses a dedicated 1-Wire connection on PA6 with an external pull-up resistor on the data line.

## Measurements

The INA219 provides:

- Bus voltage
- Load current
- Power calculated from the measured electrical parameters

The DS18B20 provides the temperature measurement.

## Example Serial Output

```text
Temperature: 28.6 C
INA219:
Bus Voltage: 3.27 V
Current: 0.023 A
Power: 0.076 W
--------------------
```

The values above are representative of the observed output during the hardware test and are included to document the demonstrated measurement format.

## System Flow

```text
DS18B20 ──1-Wire──> STM32F446RE
                       │
                       ├── I2C1 ──> SSD1306 OLED
                       │
                       ├── I2C1 ──> INA219
                       │
                       └── USART2 ──> PuTTY
```

## Firmware

The implementation uses direct STM32F446RE peripheral register access for the GPIO, I2C, USART, and timing functions required by the monitoring system.

The firmware performs continuous temperature and electrical monitoring and reports the measured values through the configured serial interface.

## Project Files

```text
05-ina219-power-monitoring/
├── main.c
├── README.md
├── images/
│   ├── hardware-setup.jpg
│   ├── putty-output.jpg
│   └── circuit-diagram.jpg
└── video/
    └── live-ina219-temperature-monitoring.mp4
```

The image and video files document the actual hardware setup, circuit, serial output, and live operation of the system.
