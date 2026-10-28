# Schematic — N20 Motor, Encoder, L298N, OLED and UART

This directory stores the circuit schematic and supporting design files for firmware module `07-n20-motor-encoder-l298n-oled-uart`.

## Files to add

- `*.kicad_sch` — editable KiCad schematic source.
- `*.pdf` — exported schematic for easy viewing and report documentation.
- `images/` — optional schematic preview or wiring photographs.

## Circuit documentation checklist

When adding the schematic, verify and label:

- N20 motor connections through the L298N motor-driver output terminals.
- Motor supply and logic supply, including a common ground where required by the circuit.
- Encoder power, ground, and signal outputs, with the actual STM32 GPIO/timer pins clearly marked.
- STM32 control signals connected to the L298N input and enable pins; document any PWM-enabled pin used for speed control.
- OLED power, ground, I²C SCL/SDA lines, and its configured I²C address.
- UART TX/RX connections, including the relevant STM32 pins and the external interface used, if any.
- Connector names, pin numbers, supply-voltage limits, and any required protection or pull-up components.

## Important

This README documents the intended contents of the directory; it is not an electrical schematic. Add the actual KiCad source and/or exported PDF after checking the wiring against the implemented hardware and firmware pin assignments. Do not assume pin numbers or supply connections that have not been verified.
