# STM32F446RE INA219 Power and Temperature Monitoring

## Overview

This firmware experiment combines DS18B20 temperature sensing with INA219 electrical monitoring on the STM32F446RE.

The STM32 reads:
- Temperature from the DS18B20
- Bus voltage from the INA219
- Load current from the INA219
- Calculated power from the INA219

The values are displayed on the SSD1306 OLED and transmitted through USART2 for serial monitoring.

The implementation uses direct STM32F446RE register access rather than HAL.

## Hardware

- STM32 NUCLEO-F446RE
- INA219 current/power monitoring module
- DS18B20 temperature sensor
- 0.96-inch SSD1306 I2C OLED, 128x64
- Piezo buzzer
- Green LED
- 200 ohm resistor for the external LED
- 4.7 kOhm pull-up resistor for the DS18B20 data line
- Breadboard and jumper wires
- USB cable

## Pin Configuration

| Device | STM32F446RE | Function |
|---|---|---|
| DS18B20 DATA | PA6 | 1-Wire data |
| INA219 SCL | PB8 | I2C1 SCL |
| INA219 SDA | PB9 | I2C1 SDA |
| OLED SCL | PB8 | I2C1 SCL |
| OLED SDA | PB9 | I2C1 SDA |
| USART2 TX | PA2 | Serial output |
| Buzzer | PB0 | Temperature alert |
| External LED | GPIO output | Status/load indication |

The DS18B20 data line uses a 4.7 kOhm pull-up. The INA219 and OLED share the same I2C1 bus.

## INA219 Configuration

- I2C address: 0x40
- Shunt resistor: 0.1 ohm
- Calibration register: 4096
- Current LSB: 100 uA/bit
- Bus voltage range: 32 V
- Shunt PGA: /8
- ADC: 12-bit
- Conversion mode: continuous bus and shunt measurement

The driver reads the INA219 bus-voltage, shunt-voltage, current, and power registers and converts them into engineering units.

## Test Load

The INA219 was tested with a simple low-voltage LED load:

5 V Supply -> INA219 VIN+ -> INA219 Shunt -> INA219 VIN- -> 220 ohm -> LED -> GND

The INA219 logic interface is connected to the STM32 at 3.3 V, GND, PB8 (SCL), and PB9 (SDA).

## Temperature Alert

The firmware uses a project-level temperature threshold of approximately 30 deg C.

- Below 30 deg C: buzzer OFF
- At or above 30 deg C: buzzer ON

This is an application threshold for the demonstration, not a safety limit.

## Serial Monitoring

USART2 is configured for 115200 baud, 8-N-1.

Example output format:

Temperature: 28.6 C
INA219:
Bus Voltage: 3.27 V
Current: 0.023 A
Power: 0.076 W
-------------------------

The example values illustrate the output format and are not fixed expected measurements.

## Firmware Structure

05-ina219-power-monitoring/
├── main.c
├── delay.c / delay.h
├── onewire.c / onewire.h
├── ds18b20.c / ds18b20.h
├── i2c.c / i2c.h
├── ina219.c / ina219.h
├── oled.c / oled.h
├── uart.c / uart.h
├── buzzer.c / buzzer.h
├── images/
├── video/
└── README.md

### Module responsibilities

- main.c — application flow and sensor coordination
- delay.c/.h — SysTick-based timing
- onewire.c/.h — 1-Wire communication on PA6
- ds18b20.c/.h — DS18B20 temperature conversion and reading
- i2c.c/.h — STM32F446RE I2C1 communication on PB8/PB9
- ina219.c/.h — INA219 register configuration and measurement conversion
- oled.c/.h — SSD1306 display handling
- uart.c/.h — USART2 serial output
- buzzer.c/.h — PB0 buzzer control

## System Flow

DS18B20 (PA6) -> STM32F446RE -> I2C1 (PB8/PB9) -> INA219 + SSD1306 OLED
STM32F446RE -> USART2 PA2 -> PuTTY
STM32F446RE -> PB0 -> Buzzer

## Notes

- The INA219 driver is implemented specifically for the 0.1 ohm shunt and calibration value used in this experiment.
- The OLED and INA219 share I2C1.
- The project is intentionally kept at register level to demonstrate STM32 peripheral configuration and sensor communication.