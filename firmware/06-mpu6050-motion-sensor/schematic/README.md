# Schematic — MPU6050 Motion Sensor

This directory is reserved for the circuit schematic and supporting design files for the MPU6050 motion-sensor firmware in this project.

## Files to add

- Editable KiCad schematic (`.kicad_sch`), if designed in KiCad
- Exported schematic PDF for easy viewing and documentation
- Optional circuit images or notes that explain the wiring

## Documentation checklist

Before committing the schematic, verify and label:

- MPU6050 power supply and ground
- I2C connections (`SCL` and `SDA`) to the correct STM32 pins
- Any required pull-up resistors and the logic-voltage compatibility of the sensor breakout
- Optional `INT` connection, if used
- Connector labels and pin assignments consistent with the firmware

**Note:** This README creates and documents the folder; the actual schematic file should be added here once it is available and verified against the implemented wiring.
