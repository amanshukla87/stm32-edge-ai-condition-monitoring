# 09 — TFT Display

## Current progress

The 2.8-inch SPI TFT module is connected to the STM32 NUCLEO-F446RE. After the initial colour-fill tests, I have progressed to displaying **motor encoder data on the TFT from the STM32 firmware**.

This is an important step toward the project's local motor-monitoring interface. The display work is still in progress; the next stages are to refine the layout and integrate additional sensor readings as they are tested.

## Hardware and connections

- STM32 NUCLEO-F446RE
- 2.8-inch SPI TFT display module (240 × 320; confirm the exact controller against the module)
- Motor and encoder used for the current test
- Jumper wires

| TFT signal | STM32 pin | Purpose |
|---|---|---|
| VCC | 3.3 V, if supported by the module | Power |
| GND | GND | Common ground |
| SCK / CLK | PB13 | SPI2 clock |
| SDI / MOSI | PB15 | SPI2 data to display |
| SDO / MISO | PB14 | Display data output; may not be needed for write-only tests |
| CS | PB12 | Display chip select |
| DC / RS | PC0 | Command/data selection |
| RESET / RST | PC1 | Display reset |
| LED / BL | Module-dependent | Backlight; follow the display board's specifications |

## Test evidence

- [Hardware setup and screen photos](./images/)
- [Four-colour display test video](./videos/06-tft-four-colour-display-test.mp4)
- [Complete TFT test firmware](./complete-code/)

The existing photos and video record the hardware setup and colour-test stage. Add a screenshot or video of the motor encoder data on the TFT to this folder when available, so the latest functionality is documented alongside the earlier tests.

## What is working now

- TFT module connected to the STM32 NUCLEO-F446RE over SPI2.
- Basic colour-fill tests performed.
- Motor encoder data displayed on the TFT from the STM32 firmware.

## Next steps

1. Verify that the displayed encoder value updates consistently with motor movement and matches the firmware's measured value.
2. Improve text positioning, readability, screen layout, and refresh behaviour.
3. Add other sensor readings, such as temperature, current, voltage, or vibration, only after each reading is integrated and verified in firmware.
4. Build a compact motor-condition monitoring screen with clear labels and units.
5. Capture updated photos or a short video showing the motor encoder data on the TFT.

## Notes

- TFT modules can differ in controller, initialization sequence, colour order, and screen offsets. Confirm the exact controller before treating the driver setup as final.
- Check the module's power and backlight requirements before connecting VCC or LED/BL.
- Displaying motor encoder data is a milestone, but the full multisensor monitoring interface and predictive-maintenance functionality remain separate integration tasks.
