#include "i2c.h"
#include "delay.h"

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define RCC_APB1ENR (*(volatile uint32_t *)(0x40023800U + 0x40U))
#define GPIOB_MODER (*(volatile uint32_t *)(0x40020400U + 0x00U))
#define GPIOB_OTYPER (*(volatile uint32_t *)(0x40020400U + 0x04U))
#define GPIOB_OSPEEDR (*(volatile uint32_t *)(0x40020400U + 0x08U))
#define GPIOB_PUPDR (*(volatile uint32_t *)(0x40020400U + 0x0CU))
#define GPIOB_AFRH (*(volatile uint32_t *)(0x40020400U + 0x24U))
#define I2C1_CR1 (*(volatile uint32_t *)(0x40005400U + 0x00U))
#define I2C1_CR2 (*(volatile uint32_t *)(0x40005400U + 0x04U))
#define I2C1_OAR1 (*(volatile uint32_t *)(0x40005400U + 0x08U))
#define I2C1_DR (*(volatile uint32_t *)(0x40005400U + 0x10U))
#define I2C1_SR1 (*(volatile uint32_t *)(0x40005400U + 0x14U))
#define I2C1_SR2 (*(volatile uint32_t *)(0x40005400U + 0x18U))
#define I2C1_CCR (*(volatile uint32_t *)(0x40005400U + 0x1CU))
#define I2C1_TRISE (*(volatile uint32_t *)(0x40005400U + 0x20U))

void i2c1_init(void)
{
    RCC_AHB1ENR |= (1U << 1);
    RCC_APB1ENR |= (1U << 21);
    GPIOB_MODER &= ~((3U << 16) | (3U << 18));
    GPIOB_MODER |= ((2U << 16) | (2U << 18));
    GPIOB_OTYPER |= (1U << 8) | (1U << 9);
    GPIOB_OSPEEDR |= (3U << 16) | (3U << 18);
    GPIOB_PUPDR &= ~((3U << 16) | (3U << 18));
    GPIOB_AFRH &= ~((0xFU << 0) | (0xFU << 4));
    GPIOB_AFRH |= (4U << 0) | (4U << 4);
    I2C1_CR1 = 0U;
    I2C1_CR1 |= (1U << 15);
    delay_us(10U);
    I2C1_CR1 &= ~(1U << 15);
    I2C1_CR2 = 16U;
    I2C1_OAR1 = (1U << 14);
    I2C1_CCR = 80U;
    I2C1_TRISE = 17U;
    I2C1_CR1 |= 1U;
}
uint8_t i2c1_start(void)
{
    uint32_t timeout = 100000U;
    while (I2C1_SR2 & (1U << 1)) if (--timeout == 0U) return 0U;
    I2C1_CR1 &= ~(1U << 9);
    I2C1_CR1 |= (1U << 8);
    timeout = 100000U;
    while ((I2C1_SR1 & 1U) == 0U) if (--timeout == 0U) return 0U;
    return 1U;
}
void i2c1_stop(void) { I2C1_CR1 |= (1U << 9); }
uint8_t i2c1_address(uint8_t address)
{
    uint32_t timeout = 100000U;
    I2C1_DR = address;
    while ((I2C1_SR1 & ((1U << 1) | (1U << 10))) == 0U)
        if (--timeout == 0U) return 0U;
    if (I2C1_SR1 & (1U << 10)) { I2C1_SR1 &= ~(1U << 10); return 0U; }
    (void)I2C1_SR1; (void)I2C1_SR2;
    return 1U;
}
uint8_t i2c1_write(uint8_t data)
{
    uint32_t timeout = 100000U;
    I2C1_DR = data;
    while ((I2C1_SR1 & (1U << 7)) == 0U) if (--timeout == 0U) return 0U;
    timeout = 100000U;
    while ((I2C1_SR1 & (1U << 2)) == 0U) if (--timeout == 0U) return 0U;
    return 1U;
}
