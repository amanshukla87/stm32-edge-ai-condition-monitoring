/*
 * STM32F446RE - Register-Level LED Blink
 * Stage: 01-gpio-led-blink
 *
 * Target: STM32F446RE / NUCLEO-F446RE
 * PA5 drives the onboard user LED (LD2) on the NUCLEO-F446RE.
 *
 * This standalone example uses direct memory-mapped register access;
 * it does not use the STM32 HAL.
 * Build with a suitable startup file, linker script, and project setup.
 * The software delay is approximate and is not a calibrated time base.
 */

#include <stdint.h>

/* STM32F446RE register definitions */
#define RCC_AHB1ENR    (*(volatile uint32_t *)0x40023830U)
#define GPIOA_MODER    (*(volatile uint32_t *)0x40020000U)
#define GPIOA_OTYPER   (*(volatile uint32_t *)0x40020004U)
#define GPIOA_OSPEEDR  (*(volatile uint32_t *)0x40020008U)
#define GPIOA_PUPDR    (*(volatile uint32_t *)0x4002000CU)
#define GPIOA_ODR      (*(volatile uint32_t *)0x40020014U)

#define LED_PIN        5U
#define LED_PIN_MASK   (1U << LED_PIN)

int main(void)
{
    /* 1. Enable the GPIOA peripheral clock. */
    RCC_AHB1ENR |= (1U << 0);

    /* Read back the clock-enable register before accessing GPIOA. */
    (void)RCC_AHB1ENR;

    /* 2. Configure PA5 as general-purpose output mode (MODER5 = 01). */
    GPIOA_MODER &= ~(3U << (LED_PIN * 2U));
    GPIOA_MODER |=  (1U << (LED_PIN * 2U));

    /* 3. Configure PA5 as push-pull, low speed, with no pull resistors. */
    GPIOA_OTYPER  &= ~LED_PIN_MASK;
    GPIOA_OSPEEDR &= ~(3U << (LED_PIN * 2U));
    GPIOA_PUPDR   &= ~(3U << (LED_PIN * 2U));

    /* Start with the LED off. */
    GPIOA_ODR &= ~LED_PIN_MASK;

    /* 4. Toggle the onboard LED continuously. */
    while (1)
    {
        GPIOA_ODR ^= LED_PIN_MASK;

        /* Approximate software delay. */
        for (volatile uint32_t i = 0; i < 500000U; i++)
        {
            /* Delay loop */
        }
    }
}
