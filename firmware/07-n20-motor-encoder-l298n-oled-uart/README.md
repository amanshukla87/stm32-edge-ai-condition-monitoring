# 07 — N20 Motor, Encoder, L298N, OLED and UART

## Purpose
Standalone STM32F446RE firmware stage for testing the N20 DC geared motor and its encoder through an L298N motor driver, with readings shown on the OLED and in a serial terminal (PuTTY).

## Hardware
- STM32 NUCLEO-F446RE
- N20 6 V, 100 RPM metal-geared DC motor with quadrature encoder
- L298N motor driver
- OLED display
- USB-UART / USART serial output to PuTTY
- Motor supply suitable for the motor and driver

## Motor and encoder reference
- Nominal motor voltage: 6 V
- Rated output speed: 100 RPM
- Encoder resolution listed by supplier: 3 PPR (confirm whether this is measured before or after the gearbox and how both channels/edges are counted)
- Wheel diameter: 43 mm
- Encoder leads: black = encoder VCC, blue = encoder GND, yellow and green = encoder signal channels; red and white = motor terminals. Verify against the exact unit and its datasheet before wiring.

## Functionality
- Motor control through the L298N
- Encoder pulse counting
- RPM calculation
- OLED output and UART/PuTTY monitoring

## Status and verification
The project owner reports that motor/encoder counting and RPM output work with the STM32, L298N, OLED and PuTTY. The source code and exact pin map should be added to this folder from the tested project version.

## Important
Do not assume pin assignments or encoder scaling from this README. Record the actual CubeMX/register configuration and measured counts-per-revolution in the source documentation. Use a separate suitable motor supply and connect grounds as required; do not power the motor directly from an STM32 GPIO.
