# 09 — TFT Display (ST7789)

## Objective

Add an SPI TFT display to the STM32F446RE condition-monitoring project. This stage starts with a basic colour-screen test before integrating sensor readings into the display.

## Hardware

- STM32 NUCLEO-F446RE
- ST7789-based SPI TFT module
- Jumper wires

## Pin Connections

| TFT signal | NUCLEO-F446RE pin | Notes |
|---|---|---|
| VCC | 3.3V, if supported by the module | Verify the exact module's supply requirements |
| GND | GND | Common ground |
| LED / BL | Verified 3.3V backlight supply | Check the module's backlight circuit |
| SCK / CLK | PB13 | SPI2_SCK, AF5 |
| SDI / MOSI | PB15 | SPI2_MOSI, AF5 |
| SDO / MISO | PB14 | Optional for write-only display operation |
| CS | PB12 | GPIO output, software-controlled chip select |
| DC / RS | PC0 | GPIO output |
| RESET / RST | PC1 | GPIO output |

## Firmware Status

This folder is the place for the standalone TFT test firmware and supporting notes. The first test should initialise the ST7789 and cycle through solid red, green, blue, white, and black screens.

**Important:** ST7789 modules vary by resolution, panel offsets, colour order, and initialization requirements. Confirm the exact module before treating a generic initialization sequence as final. ILI9341 initialization commands are not interchangeable with ST7789 commands.

## Test Checklist

- [ ] Confirm VCC, GND, and backlight wiring.
- [ ] Confirm SPI2 GPIO configuration: PB13 SCK and PB15 MOSI, AF5.
- [ ] Confirm CS (PB12), DC (PC0), and RESET (PC1) are configured as GPIO outputs.
- [ ] Build and flash the standalone test firmware.
- [ ] Verify that each test colour fills the visible panel correctly.
- [ ] Record the actual panel resolution and any required X/Y offsets.
- [ ] After the standalone test works, plan integration with the multisensor monitor.

## Safety and Debugging Notes

- Do not assume every TFT board supports 3.3V on every power/backlight pin; verify the module's specifications.
- A glowing backlight does not prove that SPI communication or display initialization is working.
- If the panel stays white or black, verify the exact controller/module, SPI pin setup, reset wiring, and initialization sequence before changing other project hardware.
