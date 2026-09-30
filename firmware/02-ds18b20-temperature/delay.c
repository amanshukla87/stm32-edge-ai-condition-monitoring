#include "delay.h"

#define SYSTICK_BASE 0xE000E010U

#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL  (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))

void delay_init(void)
{
    /* 16 MHz HSI: one SysTick tick = 1 us. */
    SYSTICK_LOAD = 16U - 1U;
    SYSTICK_VAL  = 0U;
    SYSTICK_CTRL = (1U << 2) | (1U << 0);
}

void delay_us(uint32_t us)
{
    while (us--)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0U)
        {
        }
    }
}
