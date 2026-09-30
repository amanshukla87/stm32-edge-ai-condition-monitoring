#include "buzzer.h"

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define GPIOB_MODER (*(volatile uint32_t *)(0x40020400U + 0x00U))
#define GPIOB_OTYPER (*(volatile uint32_t *)(0x40020400U + 0x04U))
#define GPIOB_ODR (*(volatile uint32_t *)(0x40020400U + 0x14U))

#define BUZZER_PIN 0U
#define BUZZER_MASK (1U << BUZZER_PIN)

void buzzer_init(void)
{
    RCC_AHB1ENR |= (1U << 1);
    GPIOB_MODER &= ~(3U << (BUZZER_PIN * 2U));
    GPIOB_MODER |= (1U << (BUZZER_PIN * 2U));
    GPIOB_OTYPER &= ~BUZZER_MASK;
    buzzer_off();
}

void buzzer_on(void)  { GPIOB_ODR |= BUZZER_MASK; }
void buzzer_off(void) { GPIOB_ODR &= ~BUZZER_MASK; }
