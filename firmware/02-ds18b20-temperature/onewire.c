#include "onewire.h"
#include "delay.h"

#define RCC_BASE       0x40023800U
#define RCC_AHB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x30U))

#define GPIOA_BASE     0x40020000U
#define GPIOA_MODER    (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OTYPER   (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
#define GPIOA_IDR      (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))
#define GPIOA_ODR      (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))

#define ONEWIRE_PIN            6U
#define ONEWIRE_PIN_MASK       (1U << ONEWIRE_PIN)
#define ONEWIRE_PIN_MODE_POS   (ONEWIRE_PIN * 2U)

static void pin_output(void)
{
    GPIOA_MODER &= ~(3U << ONEWIRE_PIN_MODE_POS);
    GPIOA_MODER |=  (1U << ONEWIRE_PIN_MODE_POS);
    GPIOA_OTYPER |= ONEWIRE_PIN_MASK;
}

static void pin_input(void)
{
    GPIOA_MODER &= ~(3U << ONEWIRE_PIN_MODE_POS);
}

static void line_low(void)
{
    pin_output();
    GPIOA_ODR &= ~ONEWIRE_PIN_MASK;
}

static void line_release(void)
{
    pin_input();
}

static uint8_t read_pin(void)
{
    return (GPIOA_IDR & ONEWIRE_PIN_MASK) ? 1U : 0U;
}

void onewire_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    line_release();
}

uint8_t onewire_reset(void)
{
    uint8_t presence;

    line_low();
    delay_us(480U);

    line_release();
    delay_us(70U);

    presence = (read_pin() == 0U) ? 1U : 0U;

    delay_us(410U);

    return presence;
}

void onewire_write_bit(uint8_t bit)
{
    line_low();

    if (bit)
    {
        delay_us(6U);
        line_release();
        delay_us(64U);
    }
    else
    {
        delay_us(60U);
        line_release();
        delay_us(10U);
    }
}

uint8_t onewire_read_bit(void)
{
    uint8_t bit;

    line_low();
    delay_us(2U);

    line_release();
    delay_us(10U);

    bit = read_pin();

    delay_us(50U);

    return bit;
}

void onewire_write_byte(uint8_t data)
{
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        onewire_write_bit(data & 0x01U);
        data >>= 1;
    }
}

uint8_t onewire_read_byte(void)
{
    uint8_t data = 0U;
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        if (onewire_read_bit())
        {
            data |= (1U << i);
        }
    }

    return data;
}
