#include "gpio.h"

/* STM32F446RE GPIOA register definitions */
#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))

#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))

#define GPIOA_CLOCK_ENABLE   (1U << 0)
#define GPIOA_PIN5_MODE_POS  (5U * 2U)
#define GPIOA_PIN5            (1U << 5)

void GPIOA_PA5_Output_Init(void)
{
    /* Enable GPIOA peripheral clock. */
    RCC_AHB1ENR |= GPIOA_CLOCK_ENABLE;

    /* Configure PA5 as general-purpose output (MODER5 = 01). */
    GPIOA_MODER &= ~(3U << GPIOA_PIN5_MODE_POS);
    GPIOA_MODER |=  (1U << GPIOA_PIN5_MODE_POS);
}

void GPIOA_PA5_Toggle(void)
{
    GPIOA_ODR ^= GPIOA_PIN5;
}
