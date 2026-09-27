# MPU6050 Motion Sensor

## Purpose

This stage adds MPU6050 motion sensing to the STM32 multisensor monitoring project. The sensor provides accelerometer, gyroscope, and temperature data for motion and vibration monitoring.

## I2C

The tested MPU6050 module responds at:

```text
0x68
```

## STM32 Connection

| MPU6050 | STM32 NUCLEO-F446RE |
|---|---|
| VCC | Sensor supply according to the module used |
| GND | GND |
| SCL | I2C SCL |
| SDA | I2C SDA |

## Bring-Up

1. Verify sensor power and common ground.
2. Check I2C wiring.
3. Confirm the MPU6050 at address **0x68**.
4. Read and verify sensor registers.
5. Configure the accelerometer and gyroscope.
6. Read accelerometer, gyroscope, and temperature data.
7. Display or transmit the sensor values for verification.

## Status

**Status:** MPU6050 hardware and STM32 integration stage.
