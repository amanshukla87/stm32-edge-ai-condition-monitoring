# MPU6050 Motion Sensor

## Purpose

This stage integrates the **MPU6050** motion sensor with the **STM32 NUCLEO-F446RE** as part of the multisensor industrial monitoring system.

The firmware reads:
- 3-axis accelerometer data
- 3-axis gyroscope data
- MPU6050 internal temperature raw data

The same firmware stage also keeps the previously verified **DS18B20**, **INA219**, **SSD1306 OLED**, and **UART** monitoring functions.

## Hardware

| Component | Interface / Details |
|---|---|
| MCU | STM32 NUCLEO-F446RE / STM32F446RE |
| Motion sensor | MPU6050 |
| MPU6050 address | 0x68 |
| Temperature sensor | DS18B20 on PA6 |
| Power monitor | INA219 at 0x40 |
| OLED | SSD1306 128x64 at 0x3C |
| I2C bus | I2C1 — PB8 (SCL), PB9 (SDA) |
| UART | USART2 — PA2 TX, 115200 baud |
| Buzzer | PB0 |

The I2C bus is shared by the MPU6050, INA219 and SSD1306.

## MPU6050 Configuration

| Register | Setting |
|---|---|
| PWR_MGMT_1 | 0x00 |
| SMPLRT_DIV | 0x07 |
| CONFIG | 0x00 |
| GYRO_CONFIG | 0x00 |
| ACCEL_CONFIG | 0x00 |

With `GYRO_CONFIG = 0x00` and `ACCEL_CONFIG = 0x00`, the device uses its default full-scale settings.

The firmware reports the accelerometer and gyroscope as **raw 16-bit register values**. The MPU6050 temperature is also reported as its raw register value; it is not presented as a converted °C measurement.

## Firmware Architecture

The original monolithic implementation has been separated into application and driver modules.

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

### Module responsibilities

- **main.c** — system initialization, sensor sequencing and application flow
- **mpu6050.c/.h** — MPU6050 register configuration and sensor-data acquisition
- **ina219.c/.h** — INA219 configuration and voltage/current/power measurements
- **i2c.c/.h** — register-level I2C1 communication on PB8/PB9
- **ds18b20.c/.h + onewire.c/.h** — DS18B20 temperature acquisition
- **oled.c/.h** — SSD1306 display control
- **uart.c/.h** — USART2 serial output
- **buzzer.c/.h** — PB0 buzzer control
- **delay.c/.h** — SysTick-based timing

No STM32 HAL or external sensor library is used in this example.

## Verified Test Output

The MPU6050 integration was previously verified through PuTTY and the SSD1306 OLED. Example raw readings recorded during testing include:

```text
Temperature: 27.6 C

INA219:
Bus Voltage: 2.64 V
Current: 0.005 A
Power: 0.013 W

MPU6050 DATA
Accel X: 17096
Accel Y: -96
Accel Z: -1516
Temperature raw: 2528
Gyro X: -46
Gyro Y: 470
Gyro Z: -61
```

A subsequent reading also showed MPU6050 accelerometer, gyroscope and raw temperature data changing with sensor position.

## Verification Status

- [x] MPU6050 I2C communication verified at 0x68
- [x] MPU6050 initialization verified
- [x] Accelerometer data acquisition verified
- [x] Gyroscope data acquisition verified
- [x] MPU6050 internal temperature raw data verified
- [x] UART/PuTTY output verified
- [x] SSD1306 OLED output verified
- [x] DS18B20 integrated
- [x] INA219 integrated
- [x] Buzzer startup indication integrated

## Media

Existing test evidence is kept in:

```text
images/
video/
```

The source tree is intentionally organized so the hardware drivers can be inspected independently while `main.c` remains focused on application behavior.

## Status

**Status: MPU6050 hardware integration and sensor-data verification completed.**