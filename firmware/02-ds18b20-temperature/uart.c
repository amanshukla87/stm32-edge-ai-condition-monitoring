#include "uart.h"

#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x40U))

#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

#define USART2_BASE     0x40004400U
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0CU))
#define USART2_CR2      (*(volatile uint32_t *)(USART2_BASE + 0x10U))
#define USART2_CR3      (*(volatile uint32_t *)(USART2_BASE + 0x14U))

void uart2_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    RCC_APB1ENR |= (1U << 17);

    /* PA2 = USART2_TX, alternate function 7. */
    GPIOA_MODER &= ~(3U << (2U * 2U));
    GPIOA_MODER |=  (2U << (2U * 2U));

    GPIOA_AFRL &= ~(0xFU << 8);
    GPIOA_AFRL |=  (7U << 8);

    GPIOA_PUPDR &= ~(3U << (2U * 2U));

    /* 16 MHz PCLK1, 115200 baud. */
    USART2_BRR = 139U;

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;

    USART2_CR1 |= (1U << 3);   /* TE */
    USART2_CR1 |= (1U << 13);  /* UE */
}

void uart2_send_char(char c)
{
    while ((USART2_SR & (1U << 7)) == 0U)
    {
    }

    USART2_DR = (uint8_t)c;
}

void uart2_send_string(const char *str)
{
    while (*str)
    {
        uart2_send_char(*str++);
    }
}

void uart2_send_temperature(int16_t raw_temp)
{
    int32_t temp_x10 = ((int32_t)raw_temp * 10) / 16;
    uint32_t integer_part;
    uint8_t fractional_part;

    if (temp_x10 < 0)
    {
        uart2_send_char('-');
        temp_x10 = -temp_x10;
    }

    integer_part = (uint32_t)(temp_x10 / 10);
    fractional_part = (uint8_t)(temp_x10 % 10);

    if (integer_part == 0U)
    {
        uart2_send_char('0');
    }
    else
    {
        char buffer[10];
        uint8_t i = 0U;

        while (integer_part > 0U)
        {
            buffer[i++] = (char)('0' + (integer_part % 10U));
            integer_part /= 10U;
        }

        while (i > 0U)
        {
            uart2_send_char(buffer[--i]);
        }
    }

    uart2_send_char('.');
    uart2_send_char((char)('0' + fractional_part));
    uart2_send_string(" C\r\n");
}
