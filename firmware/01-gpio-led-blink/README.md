# STM32F446RE Register-Level LED Blink

A minimal STM32F446RE GPIO example using direct register-level programming without STM32 HAL.

## Objective

Configure **PA5** of the STM32F446RE as a GPIO output and blink an external LED using direct register access.

## Hardware

- STM32 NUCLEO-F446RE
- External LED
- Current-limiting resistor
- Breadboard
- Jumper wires

## Connections

| STM32F446RE | Component |
|---|---|
| PA5 | LED anode through current-limiting resistor |
| GND | LED cathode |

## Firmware Structure

```text
01-gpio-led-blink/
├── main.c
├── gpio.c
├── gpio.h
├── README.md
└── circuit-diagram.png
```

### File responsibilities

- **main.c** — application flow and software delay.
- **gpio.c** — STM32F446RE GPIOA register configuration and PA5 control.
- **gpio.h** — public GPIO interface used by `main.c`.

## Circuit Diagram

![STM32F446RE GPIO LED Blink Circuit](circuit-diagram.png)

## Registers Used

- `RCC_AHB1ENR` — enables the GPIOA peripheral clock.
- `GPIOA_MODER` — configures PA5 as a general-purpose output.
- `GPIOA_ODR` — controls the PA5 output state.

## Working

1. Enable the GPIOA peripheral clock through `RCC_AHB1ENR`.
2. Configure PA5 as a general-purpose output through `GPIOA_MODER`.
3. Toggle PA5 through `GPIOA_ODR`.
4. Apply a simple software delay between state changes.

## Programming Approach

This example uses **direct register-level programming without STM32 HAL**. The GPIO register definitions and driver functions are kept separate from the application logic so that `main.c` remains focused on the program flow.

## Result

The external LED connected to **PA5** blinks continuously.

## Note

The software delay is intentionally simple and is used only for this introductory register-level GPIO demonstration. It is not intended as a precise timing mechanism.
