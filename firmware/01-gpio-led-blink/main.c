#include <stdint.h>
#include "gpio.h"

static void delay(volatile uint32_t count)
{
    while (count--)
    {
        /* Simple software delay for this register-level demonstration. */
    }
}

int main(void)
{
    GPIOA_PA5_Output_Init();

    while (1)
    {
        GPIOA_PA5_Toggle();
        delay(500000U);
    }
}
