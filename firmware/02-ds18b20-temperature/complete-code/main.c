/*
 * STM32F446RE - Standalone DS18B20 Temperature Monitor
 * Stage: 02-ds18b20-temperature
 *
 * DS18B20 DATA: PA6, with an external 4.7 kOhm pull-up to 3.3 V.
 * UART2 TX: PA2 (AF7), 115200 baud; use a compatible USB-UART/board
 * virtual COM connection to view output in a serial terminal.
 *
 * This is a single-file, register-level example. It does not use HAL.
 * Timing and UART baud settings assume the MCU/peripheral clock is 16 MHz HSI.
 * Build with the project's appropriate startup file and linker script.
 */

#include <stdint.h>

/* RCC */
#define RCC_BASE       0x40023800U
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x40U))
#define RCC_CR        (*(volatile uint32_t *)(RCC_BASE + 0x00U))
#define RCC_CFGR      (*(volatile uint32_t *)(RCC_BASE + 0x08U))

/* GPIOA */
#define GPIOA_BASE     0x40020000U
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OTYPER  (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
#define GPIOA_PUPDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_IDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_AFRL    (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

/* USART2 */
#define USART2_BASE    0x40004400U
#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR    (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1    (*(volatile uint32_t *)(USART2_BASE + 0x0CU))
#define USART2_CR2    (*(volatile uint32_t *)(USART2_BASE + 0x10U))
#define USART2_CR3    (*(volatile uint32_t *)(USART2_BASE + 0x14U))

/* SysTick */
#define SYSTICK_BASE  0xE000E010U
#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL  (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))

#define DS18B20_PIN 6U

/* SysTick time base: one COUNTFLAG per microsecond at 16 MHz. */
static void delay_init(void)
{
    SYSTICK_LOAD = 16U - 1U;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = (1U << 2) | (1U << 0); /* processor clock, no IRQ, enable */
}

static void delay_us(uint32_t us)
{
    while (us--)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0U)
        {
            /* Wait for COUNTFLAG. */
        }
    }
}

/* DS18B20 one-wire GPIO on PA6; external 4.7 kOhm pull-up is required. */
static void pin_output(void)
{
    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
    GPIOA_MODER |=  (1U << (DS18B20_PIN * 2U));
    GPIOA_OTYPER |= (1U << DS18B20_PIN); /* Open-drain */
}

static void pin_input(void)
{
    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
}

static void one_wire_low(void)
{
    pin_output();
    GPIOA_ODR &= ~(1U << DS18B20_PIN);
}

static void one_wire_release(void)
{
    /* Input mode releases the bus; the external resistor pulls it high. */
    pin_input();
}

static uint8_t one_wire_read_pin(void)
{
    return (GPIOA_IDR & (1U << DS18B20_PIN)) ? 1U : 0U;
}

static uint8_t ds18b20_reset(void)
{
    uint8_t presence;

    one_wire_low();
    delay_us(480U);
    one_wire_release();
    delay_us(70U);
    presence = (one_wire_read_pin() == 0U) ? 1U : 0U;
    delay_us(410U);

    return presence;
}

static void ds18b20_write_bit(uint8_t bit)
{
    one_wire_low();

    if (bit)
    {
        delay_us(6U);
        one_wire_release();
        delay_us(64U);
    }
    else
    {
        delay_us(60U);
        one_wire_release();
        delay_us(10U);
    }
}

static uint8_t ds18b20_read_bit(void)
{
    uint8_t bit;

    one_wire_low();
    delay_us(2U);
    one_wire_release();
    delay_us(10U);
    bit = one_wire_read_pin();
    delay_us(50U);

    return bit;
}

static void ds18b20_write_byte(uint8_t data)
{
    for (uint8_t i = 0U; i < 8U; i++)
    {
        ds18b20_write_bit(data & 0x01U);
        data >>= 1;
    }
}

static uint8_t ds18b20_read_byte(void)
{
    uint8_t data = 0U;

    for (uint8_t i = 0U; i < 8U; i++)
    {
        if (ds18b20_read_bit())
        {
            data |= (uint8_t)(1U << i);
        }
    }

    return data;
}

/* USART2 TX on PA2, AF7, 115200 baud assuming PCLK1 = 16 MHz. */
static void uart2_init(void)
{
    RCC_AHB1ENR |= (1U << 0);  /* GPIOA clock */
    RCC_APB1ENR |= (1U << 17); /* USART2 clock */

    GPIOA_MODER &= ~(3U << (2U * 2U));
    GPIOA_MODER |=  (2U << (2U * 2U)); /* PA2 alternate-function mode */

    GPIOA_AFRL &= ~(0xFU << 8);
    GPIOA_AFRL |=  (7U << 8); /* AF7 = USART2_TX */

    GPIOA_PUPDR &= ~(3U << (2U * 2U));

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;
    USART2_BRR = 139U; /* 16 MHz / 115200 approximately */
    USART2_CR1 |= (1U << 3);  /* TE: transmitter enable */
    USART2_CR1 |= (1U << 13); /* UE: USART enable */
}

static void uart2_send_char(char c)
{
    while ((USART2_SR & (1U << 7)) == 0U)
    {
        /* Wait for TXE. */
    }

    USART2_DR = (uint8_t)c;
}

static void uart2_send_string(const char *str)
{
    while (*str)
    {
        uart2_send_char(*str++);
    }
}

static void uart2_send_uint(uint32_t value)
{
    char buffer[10];
    uint8_t i = 0U;

    if (value == 0U)
    {
        uart2_send_char('0');
        return;
    }

    while (value > 0U)
    {
        buffer[i++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (i > 0U)
    {
        uart2_send_char(buffer[--i]);
    }
}

/* DS18B20 raw temperature uses units of 1/16 degree C; print 1 decimal. */
static void uart2_send_temperature(int16_t raw_temp)
{
    int32_t temp_x10 = ((int32_t)raw_temp * 10) / 16;

    if (temp_x10 < 0)
    {
        uart2_send_char('-');
        temp_x10 = -temp_x10;
    }

    uart2_send_uint((uint32_t)(temp_x10 / 10));
    uart2_send_char('.');
    uart2_send_char((char)('0' + (temp_x10 % 10)));
    uart2_send_string(" C\r\n");
}

/* Returns -32768 if the sensor is not detected. */
static int16_t ds18b20_read_temperature_raw(void)
{
    uint8_t temp_lsb;
    uint8_t temp_msb;
    int16_t raw_temp;

    if (!ds18b20_reset())
    {
        return (int16_t)-32768;
    }

    ds18b20_write_byte(0xCCU); /* Skip ROM: assumes one sensor on the bus */
    ds18b20_write_byte(0x44U); /* Start temperature conversion */
    delay_us(750000U);         /* Max conversion time at 12-bit resolution */

    if (!ds18b20_reset())
    {
        return (int16_t)-32768;
    }

    ds18b20_write_byte(0xCCU); /* Skip ROM */
    ds18b20_write_byte(0xBEU); /* Read Scratchpad */

    temp_lsb = ds18b20_read_byte();
    temp_msb = ds18b20_read_byte();
    raw_temp = (int16_t)(((uint16_t)temp_msb << 8) | temp_lsb);

    return raw_temp;
}

int main(void)
{
    int16_t temperature_raw;

    /* Use the default 16 MHz HSI system clock. */
    RCC_CR |= (1U << 0);
    RCC_CFGR &= ~(3U << 0);

    RCC_AHB1ENR |= (1U << 0); /* GPIOA clock */
    delay_init();
    uart2_init();
    one_wire_release();

    uart2_send_string("\r\nSTM32F446RE DS18B20 Temperature Monitor\r\n");
    uart2_send_string("---------------------------------------\r\n");

    while (1)
    {
        temperature_raw = ds18b20_read_temperature_raw();

        if (temperature_raw == (int16_t)-32768)
        {
            uart2_send_string("DS18B20 not detected!\r\n");
        }
        else
        {
            uart2_send_string("Temperature: ");
            uart2_send_temperature(temperature_raw);
        }

        delay_us(1000000U);
    }
}
