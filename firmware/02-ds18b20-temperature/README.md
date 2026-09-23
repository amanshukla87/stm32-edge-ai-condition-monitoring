# STM32F446RE DS18B20 Temperature Monitoring

Bare-metal firmware for reading temperature from a **DS18B20** digital temperature sensor over the 1-Wire interface and displaying the measured value through USART2.

## Hardware

- STM32 NUCLEO-F446RE
- DS18B20 temperature sensor
- 4.7 kΩ pull-up resistor
- USB connection for serial monitoring

## Pin Connections

| Device | STM32F446RE |
|---|---|
| DS18B20 DATA | PA6 |
| DS18B20 VCC | 3.3 V |
| DS18B20 GND | GND |
| USART2 TX | PA2 |

## Firmware

- 1-Wire communication using GPIO bit-banging
- 12-bit temperature resolution
- USART2 at 115200 baud, 8-N-1
- SysTick-based timing
- Direct register-level C
- No STM32 HAL or external sensor library

## Serial Output

The measured temperature is transmitted once per second:

```text
================================
STM32F446RE DS18B20 MONITOR
UART2: 115200 8N1
DS18B20: PA6
================================
Temperature: 28.1 C
```

If the sensor is not detected:

```text
DS18B20 not detected!
```

## Hardware Test

The firmware was tested on an STM32F446RE with the DS18B20 connected to PA6.

![STM32F446RE and DS18B20 hardware setup](images/01-stm32-ds18b20-hardware.jpeg)

## Serial Monitoring

Temperature readings are monitored through **USART2** using PuTTY at **115200 baud, 8-N-1**.

![PuTTY live temperature output](images/02-putty-temperature-output.png)

## Verification

The temperature reading was verified on physical STM32F446RE hardware using the DS18B20 sensor and USART2 serial output.
