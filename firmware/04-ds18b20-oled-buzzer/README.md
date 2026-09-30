# STM32F446RE DS18B20 + SSD1306 OLED + Buzzer

Bare-metal STM32F446RE firmware extending the DS18B20 temperature monitor with an SSD1306 I2C OLED and threshold-based active buzzer alert.

## Connections

| Component | STM32F446RE | Function |
|---|---|---|
| DS18B20 DATA | PA6 | 1-Wire |
| OLED SCL | PB8 | I2C1 SCL |
| OLED SDA | PB9 | I2C1 SDA |
| Buzzer signal | PB0 | Digital output |
| USART2 TX | PA2 | Serial output |

DS18B20 VCC → 3.3 V, GND → GND. OLED address: **0x3C**. USART2: **115200, 8-N-1**.

## Firmware Structure

~~~text
04-ds18b20-oled-buzzer/
├── main.c
├── delay.c / delay.h
├── onewire.c / onewire.h
├── ds18b20.c / ds18b20.h
├── i2c.c / i2c.h
├── oled.c / oled.h
├── uart.c / uart.h
├── buzzer.c / buzzer.h
├── images/
└── README.md
~~~

### Module responsibilities

- **main.c** — application flow and threshold decision.
- **delay.c/.h** — SysTick timing.
- **onewire.c/.h** — 1-Wire transactions on PA6.
- **ds18b20.c/.h** — DS18B20 temperature conversion.
- **i2c.c/.h** — register-level I2C1 master on PB8/PB9.
- **oled.c/.h** — SSD1306 initialization and display.
- **uart.c/.h** — USART2 serial output.
- **buzzer.c/.h** — PB0 buzzer control and alert threshold.

## Temperature Alert

The current test threshold is approximately **27 °C**.

| Condition | Buzzer |
|---|---|
| ≤ 27 °C | OFF |
| > 27 °C | ON |
| Sensor not detected | OFF |

The threshold is defined as `BUZZER_THRESHOLD_RAW` in `buzzer.h`.

## Operation

1. Initialize the timing, UART, DS18B20, OLED, and buzzer drivers.
2. Read the DS18B20 temperature.
3. Display and transmit the result.
4. Turn the buzzer ON above the configured threshold.
5. Turn the buzzer OFF at or below the threshold.
6. On sensor failure, show `ERR` and keep the buzzer OFF.

## Design Approach

This stage separates actual hardware responsibilities into drivers. `main.c` contains the application sequence, while sensor, communication, display, timing, and alert-control code remain in their respective modules.

The implementation uses direct STM32F446RE register programming without STM32 HAL or external sensor/display libraries.

## Project Status

**Firmware stage:** DS18B20 + SSD1306 OLED + buzzer integration.
