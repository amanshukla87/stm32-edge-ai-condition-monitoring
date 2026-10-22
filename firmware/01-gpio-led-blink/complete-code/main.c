/*
 * STM32F446RE - Register-Level LED Blink
 * Stage: 01-gpio-led-blink
 *
 * This is a self-contained main.c example. It uses direct memory-mapped
 * register access and does not require gpio.c or gpio.h.
 *
 * Wiring:
 *   PA5 -> current-limiting resistor -> LED anode
 *   LED cathode -> GND
 *
 * Target: STM32F446RE / NUCLEO-F446RE
 * Note: This file is provided as source code; compile and verify it with
 * the project's selected startup/linker configuration before flashing.
 */

#include <stdint.h>

/* STM32F446RE peripheral register addresses */
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830UL)
#define GPIOA_MODER   (*(volatile uint32_t *)0x40020000UL)
#define GPIOA_OTYPER  (*(volatile uint32_t *)0x40020004UL)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)0x40020008UL)
#define GPIOA_PUPDR   (*(volatile uint32_t *)0x4002000CUL)
#define GPIOA_ODR     (*(volatile uint32_t *)0x40020014UL)

#define LED_PIN       5U
#define LED_PIN_MASK  (1UL << LED_PIN)

static void delay(volatile uint32_t count)
{
    while (count > 0U)
    {
        count--;
    }
}

static void led_gpio_init(void)
{
    /* Enable the GPIOA peripheral clock. */
    RCC_AHB1ENR |= (1UL << 0);

    /* Read back the clock-enable register before accessing GPIOA. */
    (void)RCC_AHB1ENR;

    /* Configure PA5 as general-purpose output (MODER5 = 01). */
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2U));
    GPIOA_MODER |=  (1UL << (LED_PIN * 2U));

    /* Push-pull output, low speed, no pull-up/pull-down. */
    GPIOA_OTYPER  &= ~LED_PIN_MASK;
    GPIOA_OSPEEDR &= ~(3UL << (LED_PIN * 2U));
    GPIOA_PUPDR   &= ~(3UL << (LED_PIN * 2U));

    /* Start with the LED off. */
    GPIOA_ODR &= ~LED_PIN_MASK;
}

int main(void)
{
    led_gpio_init();

    while (1)
    {
        /* Toggle PA5. */
        GPIOA_ODR ^= LED_PIN_MASK;

        /* Approximate software delay; not a calibrated time base. */
        delay(500000U);
    }
}
