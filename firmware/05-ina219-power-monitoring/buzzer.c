#include "buzzer.h"
#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define GPIOB_MODER  (*(volatile uint32_t *)(0x40020400U + 0x00U))
#define GPIOB_OTYPER (*(volatile uint32_t *)(0x40020400U + 0x04U))
#define GPIOB_ODR    (*(volatile uint32_t *)(0x40020400U + 0x14U))
#define BUZZER_MASK  (1U << 0)

void buzzer_init(void)
{
    RCC_AHB1ENR |= (1U << 1);
    GPIOB_MODER &= ~(3U << 0);
    GPIOB_MODER |=  (1U << 0);
    GPIOB_OTYPER &= ~BUZZER_MASK;
    GPIOB_ODR &= ~BUZZER_MASK;
}

void buzzer_on(void)  { GPIOB_ODR |= BUZZER_MASK; }
void buzzer_off(void) { GPIOB_ODR &= ~BUZZER_MASK; }
}