#include "i2c.h"
#include "delay.h"

#define AHB1ENR (*(volatile uint32_t *)(0x40023800U + 0x30U))
#define APB1ENR (*(volatile uint32_t *)(0x40023800U + 0x40U))
#define MODER   (*(volatile uint32_t *)(0x40020400U + 0x00U))
#define OTYPER  (*(volatile uint32_t *)(0x40020400U + 0x04U))
#define OSPEEDR (*(volatile uint32_t *)(0x40020400U + 0x08U))
#define PUPDR   (*(volatile uint32_t *)(0x40020400U + 0x0CU))
#define AFRH    (*(volatile uint32_t *)(0x40020400U + 0x24U))
#define CR1     (*(volatile uint32_t *)(0x40005400U + 0x00U))
#define CR2     (*(volatile uint32_t *)(0x40005400U + 0x04U))
#define OAR1    (*(volatile uint32_t *)(0x40005400U + 0x08U))
#define DR      (*(volatile uint32_t *)(0x40005400U + 0x10U))
#define SR1     (*(volatile uint32_t *)(0x40005400U + 0x14U))
#define SR2     (*(volatile uint32_t *)(0x40005400U + 0x18U))
#define CCR     (*(volatile uint32_t *)(0x40005400U + 0x1CU))
#define TRISE   (*(volatile uint32_t *)(0x40005400U + 0x20U))

#define I2C_CR1_PE   (1U << 0)
#define I2C_CR1_START (1U << 8)
#define I2C_CR1_STOP  (1U << 9)
#define I2C_CR1_ACK   (1U << 10)
#define I2C_CR1_POS   (1U << 11)
#define I2C_SR1_SB    (1U << 0)
#define I2C_SR1_ADDR  (1U << 1)
#define I2C_SR1_BTF   (1U << 2)
#define I2C_SR1_TXE   (1U << 7)
#define I2C_SR1_AF    (1U << 10)

static uint8_t wait_sr1(uint32_t mask)
{
    uint32_t timeout = 100000U;
    while ((SR1 & mask) == 0U)
    {
        if (--timeout == 0U)
            return 0U;
    }
    return 1U;
}

void i2c1_init(void)
{
    AHB1ENR |= (1U << 1);
    APB1ENR |= (1U << 21);

    MODER &= ~((3U << 16) | (3U << 18));
    MODER |=  ((2U << 16) | (2U << 18));
    OTYPER |= ((1U << 8) | (1U << 9));
    OSPEEDR |= ((3U << 16) | (3U << 18));
    PUPDR &= ~((3U << 16) | (3U << 18));

    AFRH &= ~((0xFU << 0) | (0xFU << 4));
    AFRH |=  ((4U << 0) | (4U << 4));

    CR1 = 0U;
    CR1 |= (1U << 15);
    delay_us(10U);
    CR1 &= ~(1U << 15);

    /* Standard-mode I2C at 16 MHz PCLK1: 100 kHz. */
    CR2 = 16U;
    OAR1 = (1U << 14);
    CCR = 80U;
    TRISE = 17U;
    CR1 = I2C_CR1_PE | I2C_CR1_ACK;
}

uint8_t i2c1_start(void)
{
    uint32_t timeout = 100000U;

    while (SR2 & (1U << 1))
        if (--timeout == 0U) return 0U;

    CR1 &= ~I2C_CR1_STOP;
    CR1 |= I2C_CR1_START;

    if (!wait_sr1(I2C_SR1_SB))
        return 0U;

    return 1U;
}

void i2c1_stop(void)
{
    CR1 |= I2C_CR1_STOP;
}

uint8_t i2c1_address(uint8_t address)
{
    uint32_t timeout = 100000U;

    DR = address;

    while ((SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0U)
        if (--timeout == 0U) return 0U;

    if (SR1 & I2C_SR1_AF)
    {
        SR1 &= ~I2C_SR1_AF;
        return 0U;
    }

    (void)SR1;
    (void)SR2;
    return 1U;
}

uint8_t i2c1_write(uint8_t data)
{
    DR = data;

    if (!wait_sr1(I2C_SR1_TXE))
        return 0U;

    if (!wait_sr1(I2C_SR1_BTF))
        return 0U;

    return 1U;
}

uint8_t i2c1_read_register_2bytes(uint8_t address, uint8_t reg,
                                  uint8_t *high, uint8_t *low)
{
    uint32_t timeout = 100000U;

    if (high == 0 || low == 0)
        return 0U;

    if (!i2c1_start())
        return 0U;

    if (!i2c1_address((uint8_t)(address << 1)))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    /* Repeated START and switch to receiver. */
    CR1 |= I2C_CR1_START;
    if (!wait_sr1(I2C_SR1_SB))
    {
        i2c1_stop();
        return 0U;
    }

    DR = (uint8_t)((address << 1) | 1U);

    timeout = 100000U;
    while ((SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0U)
    {
        if (--timeout == 0U)
        {
            i2c1_stop();
            return 0U;
        }
    }

    if (SR1 & I2C_SR1_AF)
    {
        SR1 &= ~I2C_SR1_AF;
        i2c1_stop();
        return 0U;
    }

    /*
     * STM32F4 I2C peripheral: for a two-byte receive, set POS and
     * clear ACK before clearing ADDR. Wait for BTF, generate STOP,
     * then read DR twice. This avoids the previous RXNE/two-read
     * sequence, which could return incorrect INA219 register values.
     */
    CR1 |= I2C_CR1_POS;
    CR1 &= ~I2C_CR1_ACK;
    (void)SR1;
    (void)SR2;

    timeout = 100000U;
    while ((SR1 & I2C_SR1_BTF) == 0U)
    {
        if (--timeout == 0U)
        {
            CR1 |= I2C_CR1_ACK;
            CR1 &= ~I2C_CR1_POS;
            i2c1_stop();
            return 0U;
        }
    }

    CR1 |= I2C_CR1_STOP;
    *high = (uint8_t)DR;
    *low = (uint8_t)DR;

    CR1 |= I2C_CR1_ACK;
    CR1 &= ~I2C_CR1_POS;

    return 1U;
}
