#include "onewire.h"
#include "delay.h"

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define GPIOA_MODER  (*(volatile uint32_t *)(0x40020000U + 0x00U))
#define GPIOA_OTYPER (*(volatile uint32_t *)(0x40020000U + 0x04U))
#define GPIOA_IDR    (*(volatile uint32_t *)(0x40020000U + 0x10U))
#define GPIOA_ODR    (*(volatile uint32_t *)(0x40020000U + 0x14U))
#define DS18B20_MASK (1U << 6)

static void output_mode(void)
{
    GPIOA_MODER &= ~(3U << 12);
    GPIOA_MODER |=  (1U << 12);
    GPIOA_OTYPER |= DS18B20_MASK;
}

static void input_mode(void)
{
    GPIOA_MODER &= ~(3U << 12);
}

static void drive_low(void)
{
    output_mode();
    GPIOA_ODR &= ~DS18B20_MASK;
}

static void release_line(void)
{
    input_mode();
}

static uint8_t read_pin(void)
{
    return (GPIOA_IDR & DS18B20_MASK) ? 1U : 0U;
}

void onewire_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    release_line();
}

uint8_t onewire_reset(void)
{
    uint8_t presence;

    drive_low();
    delay_us(480);
    release_line();
    delay_us(70);

    presence = (read_pin() == 0U) ? 1U : 0U;

    delay_us(410);
    return presence;
}

static void write_bit(uint8_t bit)
{
    drive_low();

    if (bit)
    {
        delay_us(6);
        release_line();
        delay_us(64);
    }
    else
    {
        delay_us(60);
        release_line();
        delay_us(10);
    }
}

static uint8_t read_bit(void)
{
    uint8_t bit;

    drive_low();
    delay_us(2);
    release_line();
    delay_us(10);

    bit = read_pin();
    delay_us(50);

    return bit;
}

void onewire_write_byte(uint8_t data)
{
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        write_bit(data & 1U);
        data >>= 1;
    }
}

uint8_t onewire_read_byte(void)
{
    uint8_t data = 0U;
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        if (read_bit())
            data |= (1U << i);
    }

    return data;
}
