# TFT Display Drivers

This directory is intended for the display-driver source and supporting files used by the TFT display module on the STM32 NUCLEO-F446RE.

## Suggested contents

- Display controller driver source and header files (for example, the verified ILI9341 or ST7789 driver for the actual panel)
- SPI communication and display initialization code, if maintained separately
- Required font or graphics support files, with their original licenses preserved
- A short note describing the controller, resolution, SPI configuration, and pin mapping actually tested

## Before adding files

1. Confirm the controller fitted to the TFT module; do not assume ILI9341 or ST7789 from a seller listing alone.
2. Check the driver's license and retain its copyright/license notice.
3. Keep STM32-specific pin assignments and SPI settings consistent with the tested CubeMX project.
4. Do not describe a driver or display as working until it has been tested on the hardware.

This README creates and documents the `Drivers` directory. Add the actual driver files here when they are ready.