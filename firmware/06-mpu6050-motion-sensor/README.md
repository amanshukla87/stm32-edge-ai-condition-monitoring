# MPU6050 Motion Sensor

## Purpose

This stage integrates an MPU6050 motion sensor with the STM32 NUCLEO-F446RE as part of the multisensor industrial monitoring system.

The MPU6050 provides:
- 3-axis accelerometer data
- 3-axis gyroscope data
- Internal temperature sensor raw output

The sensor data is read through I2C and verified on both **PuTTY (UART)** and the **SSD1306 OLED**.

## Hardware

| Component | Details |
|---|---|
| MCU | STM32 NUCLEO-F446RE |
| Motion sensor | MPU6050 |
| Interface | I2C |
| MPU6050 address | 0x68 |
| OLED | SSD1306, 128x64, I2C |
| OLED address | 0x3C |
| UART terminal | PuTTY |

## MPU6050 I2C

The tested MPU6050 responds at:

```text
0x68
```

The firmware configures the MPU6050 through its standard registers and reads the sensor output registers for acceleration, internal temperature, and gyroscope data.

## STM32 Connection

| MPU6050 | STM32 NUCLEO-F446RE |
|---|---|
| VCC | Sensor supply according to the module used |
| GND | GND |
| SCL | I2C1 SCL — PB8 |
| SDA | I2C1 SDA — PB9 |

The same I2C bus is also used by the SSD1306 OLED and INA219.

## Firmware

The implementation is written in register-level C in:

```text
main.c
```

The firmware includes:
- STM32F446RE register definitions
- I2C1 initialization on PB8/PB9
- MPU6050 register write/read functions
- MPU6050 initialization
- 16-bit accelerometer, temperature, and gyroscope data reads
- UART/PuTTY output
- SSD1306 OLED output
- Integration with the existing DS18B20 and INA219 monitoring functions

### MPU6050 configuration

The current firmware initializes the sensor with:

| Register | Setting |
|---|---|
| PWR_MGMT_1 | 0x00 |
| SMPLRT_DIV | 0x07 |
| CONFIG | 0x00 |
| GYRO_CONFIG | 0x00 |
| ACCEL_CONFIG | 0x00 |

The accelerometer and gyroscope are therefore read using the configured default full-scale settings.

## Verified PuTTY Output

The following output was obtained during STM32 testing:

```text
================================
DS18B20
================================
Temperature: 27.6 C

INA219:
Bus Voltage: 2.64 V
Current: 0.005 A
Power: 0.013 W
-------------------------

MPU6050 DATA
Accel X: 17096
Accel Y: -96
Accel Z: -1516
Temperature raw: 2528
Gyro X: -46
Gyro Y: 470
Gyro Z: -61
-------------------------
```

A subsequent reading was:

```text
================================
DS18B20
================================
Temperature: 27.6 C

INA219:
Bus Voltage: 2.63 V
Current: 0.005 A
Power: 0.013 W
-------------------------

MPU6050 DATA
Accel X: 17052
Accel Y: -232
Accel Z: -1308
Temperature raw: 2640
Gyro X: 3
Gyro Y: 474
Gyro Z: 42
-------------------------
```

These values were also displayed on the SSD1306 OLED during the test.

> **Note:** The MPU6050 temperature shown above is the sensor's raw register value, not a converted temperature in °C. Accelerometer and gyroscope values are also shown as raw 16-bit sensor readings.

## Verification Status

- [x] MPU6050 powered and connected
- [x] I2C communication verified at 0x68
- [x] MPU6050 initialized from STM32
- [x] Accelerometer data read
- [x] Gyroscope data read
- [x] MPU6050 internal temperature raw data read
- [x] Sensor values transmitted through UART
- [x] Sensor values displayed on SSD1306 OLED
- [x] MPU6050 integrated with DS18B20 and INA219 monitoring

## Project Structure

```text
06-mpu6050-motion-sensor/
├── main.c
├── README.md
├── images/
│   ├── README.md
│   ├── oled-mpu6050-motion-display.jpeg
│   ├── putty-stm32-sensor-output.png
│   └── stm32-mpu6050-tilt-test.jpeg
└── video/
    ├── README.md
    └── stm32-mpu6050-motion-test.mp4
```

## Status

**Status: MPU6050 hardware integration and sensor-data verification completed.**

The current stage demonstrates working MPU6050 data acquisition on STM32 with simultaneous integration of DS18B20, INA219, and SSD1306 OLED monitoring.
