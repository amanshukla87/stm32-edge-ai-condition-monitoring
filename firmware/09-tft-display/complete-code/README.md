# Complete Code — ST7789 TFT Colour Test

## Contents
- `main.c`: STM32F446RE HAL firmware for a basic ST7789 colour-fill test.

## Before building
1. Create an STM32CubeIDE project for **NUCLEO-F446RE / STM32F446RE** with the STM32 HAL driver enabled.
2. Replace the project's generated `main.c` with this file (do not keep two `main()` functions).
3. Ensure the project startup/system files and HAL drivers are present.
4. Connect the module using the pin map in the parent `firmware/09-tft-display/README.md`.
5. Build, flash, and observe red, green, blue, white, and black screens.

## Pin map used by this code
| TFT signal | STM32 pin |
|---|---|
| SCK / CLK | PB13 (SPI2 SCK, AF5) |
| SDI / MOSI | PB15 (SPI2 MOSI, AF5) |
| CS | PB12 |
| DC / RS | PC0 |
| RST | PC1 |
| GND | GND |
| VCC / BL | According to the exact module specification |

## Important limitations
- This is a generic **ST7789 240×320** starting point. It has not been verified on your physical display.
- If your module has a different resolution, update `TFT_WIDTH` and `TFT_HEIGHT`.
- Some panels require non-zero `TFT_X_OFFSET` / `TFT_Y_OFFSET`, a different MADCTL value, or a vendor-specific initialization sequence.
- If the screen remains white, verify the controller, pinout, reset, SPI wiring, power and initialization sequence. Backlight glow alone does not confirm SPI is working.
- Use the module's documented supply requirements; do not assume every VCC/backlight pin can be connected directly to 3.3 V.
