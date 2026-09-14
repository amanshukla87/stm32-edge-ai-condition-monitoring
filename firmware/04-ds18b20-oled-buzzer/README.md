# DS18B20 Temperature Monitoring with OLED and Buzzer

## Overview

This project implements a bare-metal temperature monitoring system using the STM32F446RE and a DS18B20 digital temperature sensor.

The measured temperature is displayed on a 0.96-inch SSD1306 OLED through I2C. An active buzzer provides a temperature alert when the measured temperature exceeds the threshold defined in the firmware.

The firmware is written in C using direct STM32F446RE register-level programming without the STM32 HAL.

## Hardware

- STM32 NUCLEO-F446RE
- DS18B20 waterproof temperature sensor
- 0.96-inch SSD1306 I2C OLED, 128x64, address `0x3C`
- Active buzzer
- 4.7 kΩ pull-up resistor for the DS18B20 data line
- Jumper wires
- USB cable for board power and programming/debugging

## Pin Connections

| Component | STM32F446RE Pin | Connection / Function |
|---|---|---|
| DS18B20 DATA | PA6 | 1-Wire data |
| DS18B20 VCC | 3.3 V | Sensor supply |
| DS18B20 GND | GND | Ground |
| 4.7 kΩ resistor | PA6 to 3.3 V | 1-Wire pull-up |
| OLED SCL | PB8 | I2C1 SCL |
| OLED SDA | PB9 | I2C1 SDA |
| OLED VCC | 3.3 V | Display supply |
| OLED GND | GND | Ground |
| Buzzer signal | PB0 | Digital output |
| Buzzer GND | GND | Ground |

The OLED uses I2C1 with PB8 configured as SCL and PB9 configured as SDA using alternate function AF4.

## System Operation

```text
              DS18B20
                 │
              1-Wire
                 │ PA6
                 ▼
          STM32F446RE
           │         │
       I2C1 │         │ GPIO
           │ PB8/PB9  │ PB0
           ▼         ▼
       SSD1306     Active
        OLED       Buzzer
```

The STM32 performs the following sequence continuously:

1. Resets and communicates with the DS18B20 over the 1-Wire interface.
2. Starts a temperature conversion.
3. Reads the temperature value from the sensor.
4. Converts the raw DS18B20 value to degrees Celsius.
5. Displays the temperature on the SSD1306 OLED.
6. Sends the temperature through USART2 for serial monitoring.
7. Activates the buzzer when the programmed temperature threshold is exceeded.

## Temperature Alert

The current firmware uses a threshold of approximately **27 °C**.

- Temperature above 27 °C → buzzer ON
- Temperature at or below 27 °C → buzzer OFF
- DS18B20 communication error → buzzer OFF and `ERR` displayed on the OLED

The threshold can be changed in the firmware according to the required application.

## Serial Monitoring

The firmware initializes USART2 with:

- TX: PA2
- Baud rate: 115200
- Data format: 8-N-1

Temperature readings are transmitted as text in degrees Celsius. On the NUCLEO-F446RE, USART2 can be monitored through the ST-LINK virtual COM interface when the board's VCP connection is enabled.

## Firmware Implementation

The firmware contains register-level implementations for:

- STM32F446RE GPIO configuration
- SysTick-based microsecond delays
- DS18B20 1-Wire communication
- USART2 serial transmission
- I2C1 communication
- SSD1306 OLED control
- Active buzzer control
- Temperature threshold monitoring

No STM32 HAL or external sensor/display library is used.

## Expected Output

During normal operation, the OLED displays the measured temperature, for example:

```text
TEMP
28.7 C
```

When the temperature exceeds the programmed threshold, the active buzzer is enabled while the temperature continues to be displayed on the OLED.

## Hardware Setup

The complete setup uses the STM32 NUCLEO-F446RE, DS18B20 temperature probe, SSD1306 OLED, active buzzer, 4.7 kΩ pull-up resistor, jumper wires, and USB connection to the development board.

![STM32F446RE DS18B20 OLED and buzzer hardware setup](images/hardware-setup.jpg)

## Demonstration

The `images` directory contains the live demonstration video of the temperature monitoring and buzzer operation.

[Live Temperature and Buzzer Demonstration](images/live-temperature-buzzer-demo.mp4)

## Source Code

[main.c](main.c) contains the complete bare-metal implementation for DS18B20 temperature sensing, SSD1306 OLED display, USART2 serial output, and buzzer control.