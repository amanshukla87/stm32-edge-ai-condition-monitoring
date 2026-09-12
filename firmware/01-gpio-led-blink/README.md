# STM32F446RE Register-Level LED Blink

## Objective

Configure PA6 of the STM32F446RE as a GPIO output and blink an external LED using direct register-level programming.

## Hardware

- STM32 NUCLEO-F446RE
- External LED
- 120 Ω resistor
- Breadboard
- Jumper wires

## Connections

| STM32 | Component |
|---|---|
| PA6 | LED anode through 120 Ω resistor |
| GND | LED cathode |

## Registers Used

- `RCC_AHB1ENR` — enables the GPIOA peripheral clock.
- `GPIOA_MODER` — configures PA6 as a general-purpose output.
- `GPIOA_ODR` — toggles the PA6 output state.

## Working

1. Enable the GPIOA clock through `RCC_AHB1ENR`.
2. Configure PA6 as a general-purpose output using `GPIOA_MODER`.
3. Toggle PA6 through `GPIOA_ODR`.
4. Use a simple software delay between state changes.

## Programming Approach

This experiment uses direct register-level programming without STM32 HAL.

## Result

The external LED connected to PA6 blinks continuously.

## Media

Hardware photographs and the working demonstration video will be added as the experiment is documented.
