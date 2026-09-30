#include "onewire.h"
#include "delay.h"

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define GPIOA_MODER (*(volatile uint32_t *)(0x40020000U + 0x00U))
#define GPIOA_OTYPER (*(volatile uint32_t *)(0x40020000U + 0x04U))
#define GPIOA_IDR (*(volatile uint32_t *)(0x40020000U + 0x10U))
#define GPIOA_ODR (*(volatile uint32_t *)(0x40020000U + 0x14U))
#define PIN 6U
#define MASK (1U << PIN)

static void output_mode(void)
{
    GPIOA_MODER &= ~(3U << (PIN * 2U));
    GPIOA_MODER |= (1U << (PIN * 2U));
    GPIOA_OTYPER |= MASK;
}
static void input_mode(void) { GPIOA_MODER &= ~(3U << (PIN * 2U)); }
static void low(void) { output_mode(); GPIOA_ODR &= ~MASK; }
static void release(void) { input_mode(); }
static uint8_t read_pin(void) { return (GPIOA_IDR & MASK) ? 1U : 0U; }

void onewire_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    release();
}
uint8_t onewire_reset(void)
{
    uint8_t presence;
    low(); delay_us(480U); release(); delay_us(70U);
    presence = (read_pin() == 0U) ? 1U : 0U;
    delay_us(410U);
    return presence;
}
static void write_bit(uint8_t bit)
{
    low();
    if (bit) { delay_us(6U); release(); delay_us(64U); }
    else { delay_us(60U); release(); delay_us(10U); }
}
static uint8_t read_bit(void)
{
    uint8_t bit;
    low(); delay_us(2U); release(); delay_us(10U);
    bit = read_pin(); delay_us(50U);
    return bit;
}
void onewire_write_byte(uint8_t data)
{
    for (uint8_t i = 0U; i < 8U; i++) { write_bit(data & 1U); data >>= 1; }
}
uint8_t onewire_read_byte(void)
{
    uint8_t data = 0U;
    for (uint8_t i = 0U; i < 8U; i++) if (read_bit()) data |= (1U << i);
    return data;
}
