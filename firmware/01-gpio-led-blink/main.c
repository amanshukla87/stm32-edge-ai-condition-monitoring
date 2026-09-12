#include <stdint.h>

/* Register addresses for RCC and GPIOA (STM32F446RE) */
#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     *(volatile uint32_t *)(RCC_BASE + 0x30U)

#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     *(volatile uint32_t *)(GPIOA_BASE + 0x00U)
#define GPIOA_ODR       *(volatile uint32_t *)(GPIOA_BASE + 0x14U)

int main(void)
{
    /* 1. Enable clock for GPIOA (Bit 0 of RCC_AHB1ENR) */
    RCC_AHB1ENR |= (1U << 0);

    /* 2. Set PA6 as General Purpose Output Mode (Bits [13:12] = 01) */
    GPIOA_MODER &= ~(3U << (6 * 2)); // Clear bits 12 and 13
    GPIOA_MODER |=  (1U << (6 * 2)); // Set bit 12 to 1

    /* Infinite loop */
    while (1)
    {
        /* 3. Toggle PA6 (Bit 6 of ODR) */
        GPIOA_ODR ^= (1U << 6);

        /* 4. Simple delay loop */
        for (volatile uint32_t i = 0; i < 500000; i++);
    }
}
