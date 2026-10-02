#include "uart.h"

#define AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define APB1ENR (*(volatile uint32_t *)(0x40023800U + 0x40U))
#define MODER   (*(volatile uint32_t *)(0x40020000U + 0x00U))
#define PUPDR   (*(volatile uint32_t *)(0x40020000U + 0x0CU))
#define AFRL    (*(volatile uint32_t *)(0x40020000U + 0x20U))
#define SR      (*(volatile uint32_t *)(0x40004400U + 0x00U))
#define DR      (*(volatile uint32_t *)(0x40004400U + 0x04U))
#define BRR     (*(volatile uint32_t *)(0x40004400U + 0x08U))
#define CR1     (*(volatile uint32_t *)(0x40004400U + 0x0CU))
#define CR2     (*(volatile uint32_t *)(0x40004400U + 0x10U))
#define CR3     (*(volatile uint32_t *)(0x40004400U + 0x14U))

void uart2_init(void)
{
    AHB1ENR |= 1U;
    APB1ENR |= (1U << 17);

    MODER &= ~(3U << 4);
    MODER |=  (2U << 4);
    AFRL &= ~(0xFU << 8);
    AFRL |=  (7U << 8);
    PUPDR &= ~(3U << 4);

    BRR = 139U;
    CR1 = 0U;
    CR2 = 0U;
    CR3 = 0U;
    CR1 |= (1U << 3) | (1U << 13);
}

void uart2_send_char(char c)
{
    while ((SR & (1U << 7)) == 0U) {}
    DR = (uint8_t)c;
}

void uart2_send_string(const char *s)
{
    while (*s) uart2_send_char(*s++);
}

static void send_uint(uint32_t value)
{
    char buffer[10];
    uint8_t i = 0U;

    if (value == 0U)
    {
        uart2_send_char('0');
        return;
    }

    while (value)
    {
        buffer[i++] = (char)('0' + value % 10U);
        value /= 10U;
    }

    while (i)
        uart2_send_char(buffer[--i]);
}

void uart2_send_temperature(int16_t raw)
{
    int32_t x = ((int32_t)raw * 10) / 16;

    if (x < 0)
    {
        uart2_send_char('-');
        x = -x;
    }

    send_uint((uint32_t)(x / 10));
    uart2_send_char('.');
    uart2_send_char((char)('0' + (x % 10)));
    uart2_send_string(" C\r\n");
}

void uart2_send_ina219(uint32_t bus_mV, int32_t current_mA, uint32_t power_mW)
{
    uart2_send_string("INA219:\r\n");

    uart2_send_string("Bus Voltage: ");
    send_uint(bus_mV / 1000U);
    uart2_send_char('.');
    uart2_send_char((char)('0' + ((bus_mV / 100U) % 10U)));
    uart2_send_char((char)('0' + ((bus_mV / 10U) % 10U)));
    uart2_send_string(" V\r\n");

    uart2_send_string("Current: ");
    if (current_mA < 0)
    {
        uart2_send_char('-');
        current_mA = -current_mA;
    }
    send_uint((uint32_t)(current_mA / 1000));
    uart2_send_char('.');
    uart2_send_char((char)('0' + ((current_mA / 100) % 10)));
    uart2_send_char((char)('0' + ((current_mA / 10) % 10)));
    uart2_send_char((char)('0' + (current_mA % 10)));
    uart2_send_string(" A\r\n");

    uart2_send_string("Power: ");
    send_uint(power_mW / 1000U);
    uart2_send_char('.');
    uart2_send_char((char)('0' + ((power_mW / 100U) % 10U)));
    uart2_send_char((char)('0' + ((power_mW / 10U) % 10U)));
    uart2_send_char((char)('0' + (power_mW % 10U)));
    uart2_send_string(" W\r\n");
    uart2_send_string("-------------------------\r\n");
}
