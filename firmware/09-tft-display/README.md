# 09 — TFT Display Integration

## Overview

The TFT display is now fully integrated into the STM32-based motor monitoring system. What started as a basic colour-fill test has developed into a working on-device interface that brings the system's sensor readings together on one screen.

The 2.8-inch SPI TFT is driven by the **ST7789 display controller** and connected directly to the **STM32 NUCLEO-F446RE**. The STM32 firmware now displays the motor and multisensor data on the TFT, making it easier to monitor the system locally without relying on a separate computer or cloud dashboard.

## Hardware used

- STM32 NUCLEO-F446RE
- 2.8-inch SPI TFT display (240 × 320, ST7789 controller)
- Motor with encoder
- DS18B20 temperature sensor
- INA219 voltage and current sensor
- MPU6050 accelerometer and gyroscope for vibration-related measurements
- Jumper wires and required connections

## TFT pin connections

| TFT signal | STM32 pin | Purpose |
|---|---|---|
| VCC | 3.3 V, if supported by the module | Power |
| GND | GND | Common ground |
| SCK / CLK | PB13 | SPI2 clock |
| SDI / MOSI | PB15 | SPI2 data to display |
| SDO / MISO | PB14 | Display data output; may not be required for write-only operation |
| CS | PB12 | Display chip select |
| DC / RS | PC0 | Command/data selection |
| RESET / RST | PC1 | Display reset |
| LED / BL | Module-dependent | Backlight; follow the display board's specifications |

## Data displayed on the TFT

The display brings the main live monitoring values together in one interface:

- **Motor speed:** encoder-based RPM reading
- **Temperature:** DS18B20 sensor reading
- **Electrical measurements:** INA219 voltage, current and power readings
- **Vibration-related data:** MPU6050 accelerometer/gyroscope readings processed by the firmware

These readings are acquired and handled by the STM32 firmware before being presented on the display. The TFT therefore acts as the system's local monitoring interface.

## Current implementation status

- ST7789-based TFT initialised and communicating with the STM32 over SPI2.
- Initial colour and display tests completed.
- TFT driver integrated into the project firmware.
- Motor encoder data displayed on the screen.
- Temperature, electrical and vibration sensor data integrated into the TFT monitoring interface.
- Main monitoring values brought together on a single display.

## Test evidence and source code

- [Hardware setup and display photos](./images/)
- [Four-colour TFT test video](./videos/06-tft-four-colour-display-test.mp4)
- [Complete ST7789 TFT test firmware](./complete-code/)
- [TFT display driver documentation](./Drivers/)

The earlier colour-test evidence documents the initial bring-up process. The completed firmware and current hardware setup demonstrate the progression from a display test to an integrated monitoring interface.

## Notes

- The display uses the ST7789 controller. Its initialisation sequence, colour format and screen offsets should match the specific TFT module.
- TFT power and backlight connections are module-dependent; follow the display board's specifications.
- The TFT integration completes the local display interface. Further work on data validation, presentation and the project's anomaly-detection and predictive-maintenance features is tracked separately.

## Next improvements

The display integration is complete. Future refinements can focus on improving the screen layout and readability, making updates smoother, and presenting additional diagnostic information as the firmware evolves.
