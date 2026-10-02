#include "delay.h"

#define SYSTICK_BASE 0xE000E010U
#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL  (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))

void delay_init(void)
{
    SYSTICK_LOAD = 15U;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = (1U << 2) | 1U;
}

void delay_us(uint32_t us)
{
    while (us--)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0U) {}
    }
}
