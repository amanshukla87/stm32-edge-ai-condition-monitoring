#include <stdint.h>

/* ============================================================
 * STM32F446RE REGISTER DEFINITIONS
 * ============================================================ */

/* ---------------- RCC ---------------- */
#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))

/* ---------------- GPIOA ---------------- */
#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))

int main(void)
{
    /* 1. Enable clock for GPIOA */
    RCC_AHB1ENR |= (1U << 0);

    /* 2. Configure PA6 as General Purpose Output */
    GPIOA_MODER &= ~(3U << (6 * 2));
    GPIOA_MODER |=  (1U << (6 * 2));

    /* 3. Continuously toggle PA6 */
    while (1)
    {
        GPIOA_ODR ^= (1U << 6);

        /* Simple delay */
        for (volatile uint32_t i = 0; i < 500000; i++);
    }
}
