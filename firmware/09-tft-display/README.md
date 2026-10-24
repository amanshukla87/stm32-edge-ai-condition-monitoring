# 09 — TFT Display

## What I've done so far

I've connected the 2.8-inch SPI TFT module to the STM32 NUCLEO-F446RE and worked through the basic display colour test. The wiring is in place, and the test video shows the screen outputs observed so far.

This is an early hardware bring-up stage. The colour test is a useful first check, but it doesn't yet mean the display integration is complete.

## Hardware and connections

- STM32 NUCLEO-F446RE
- 2.8-inch SPI TFT display module (240 × 320; controller details should be confirmed against the exact module)
- Jumper wires

| TFT signal | STM32 pin | Purpose |
|---|---|---|
| VCC | 3.3 V, if supported by the module | Power |
| GND | GND | Common ground |
| SCK / CLK | PB13 | SPI2 clock |
| SDI / MOSI | PB15 | SPI2 data to display |
| SDO / MISO | PB14 | SPI data from display; may not be needed for write-only tests |
| CS | PB12 | Display chip select |
| DC / RS | PC0 | Command/data selection |
| RESET / RST | PC1 | Display reset |
| LED / BL | Module-dependent | Backlight; follow the display board's specifications |

## Test evidence

- [Hardware setup and screen photos](./images/)
- [Four-colour display test video](./videos/06-tft-four-colour-display-test.mp4)

The photos and video document the current setup and the colour outputs observed during testing.

## Next step: show sensor readings on the TFT

Now that the basic colour test has been tried, the next task is to display live sensor data on the screen. I'll bring this in step by step:

1. Confirm the display controller, initialization, orientation, and usable screen area.
2. Add simple text rendering and check that text stays readable on the panel.
3. Read sensor values from the STM32 firmware and format them for display.
4. Build a small monitoring screen for the available readings, then refresh it without unnecessarily redrawing the whole display.
5. Test the displayed values against the sensor readings reported by the firmware.

The exact values and layout will depend on which sensor readings are integrated and verified first.

## Notes

- TFT modules can differ in controller, initialization sequence, colour order, and screen offsets. Confirm the exact controller before treating the driver setup as final.
- Check the module's own power and backlight requirements before connecting VCC or LED/BL.
- A lit screen or successful colour fill alone does not verify sensor-data display or complete TFT functionality.
