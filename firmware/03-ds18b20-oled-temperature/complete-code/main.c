#include <stdint.h>

/* STM32F446RE register-level DS18B20 + SSD1306 OLED test.
 * DS18B20 DATA: PA6 with external 4.7k pull-up to 3.3V
 * OLED SSD1306 I2C: PB8=SCL, PB9=SDA, address 0x3C
 * UART2 TX: PA2, 115200 baud, 8N1
 * OLED test behavior: turns every pixel ON; it does not print the
 * temperature on the display. Temperature is reported over UART.
 */

#define RCC_BASE        0x40023800U
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00U))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x08U))
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40U))

#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OTYPER    (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08U))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_IDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

#define GPIOB_BASE      0x40020400U
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_OTYPER    (*(volatile uint32_t *)(GPIOB_BASE + 0x04U))
#define GPIOB_OSPEEDR   (*(volatile uint32_t *)(GPIOB_BASE + 0x08U))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + 0x0CU))
#define GPIOB_AFRH      (*(volatile uint32_t *)(GPIOB_BASE + 0x24U))

#define USART2_BASE     0x40004400U
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0CU))
#define USART2_CR2      (*(volatile uint32_t *)(USART2_BASE + 0x10U))
#define USART2_CR3      (*(volatile uint32_t *)(USART2_BASE + 0x14U))

#define I2C1_BASE       0x40005400U
#define I2C1_CR1        (*(volatile uint32_t *)(I2C1_BASE + 0x00U))
#define I2C1_CR2        (*(volatile uint32_t *)(I2C1_BASE + 0x04U))
#define I2C1_OAR1       (*(volatile uint32_t *)(I2C1_BASE + 0x08U))
#define I2C1_DR         (*(volatile uint32_t *)(I2C1_BASE + 0x10U))
#define I2C1_SR1        (*(volatile uint32_t *)(I2C1_BASE + 0x14U))
#define I2C1_SR2        (*(volatile uint32_t *)(I2C1_BASE + 0x18U))
#define I2C1_CCR        (*(volatile uint32_t *)(I2C1_BASE + 0x1CU))
#define I2C1_TRISE      (*(volatile uint32_t *)(I2C1_BASE + 0x20U))

#define SYSTICK_BASE    0xE000E010U
#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))

#define DS18B20_PIN     6U
#define OLED_ADDRESS    0x3CU

static void delay_init(void);
static void delay_us(uint32_t us);
static void uart2_init(void);
static void uart2_send_char(char c);
static void uart2_send_string(const char *str);
static void uart2_send_uint(uint32_t value);
static void uart2_send_temperature(int16_t raw);
static void one_wire_release(void);
static void one_wire_low(void);
static uint8_t one_wire_read(void);
static void one_wire_write_bit(uint8_t bit);
static uint8_t one_wire_read_bit(void);
static void one_wire_write_byte(uint8_t data);
static uint8_t one_wire_read_byte(void);
static uint8_t one_wire_reset(void);
static int16_t ds18b20_read_temperature_raw(void);
static void oled_i2c_init(void);
static uint8_t oled_i2c_start(void);
static void oled_i2c_stop(void);
static uint8_t oled_i2c_address(void);
static uint8_t oled_i2c_write(uint8_t data);
static uint8_t oled_command(uint8_t command);
static void oled_init(void);
static void oled_set_position(uint8_t page, uint8_t column);
static uint8_t oled_data(uint8_t data);
static void oled_test(void);

static void delay_init(void)
{
    SYSTICK_LOAD = 15U; /* 16 MHz / 16 clocks = 1 us */
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = (1U << 2) | (1U << 0);
}

static void delay_us(uint32_t us)
{
    while (us--)
    {
        SYSTICK_VAL = 0U;
        while ((SYSTICK_CTRL & (1U << 16)) == 0U) { }
    }
}

static void uart2_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    RCC_APB1ENR |= (1U << 17);

    GPIOA_MODER &= ~(3U << 4U);
    GPIOA_MODER |=  (2U << 4U);
    GPIOA_AFRL &= ~(0xFU << 8U);
    GPIOA_AFRL |=  (7U << 8U);
    GPIOA_OTYPER &= ~(1U << 2U);
    GPIOA_OSPEEDR |= (3U << 4U);
    GPIOA_PUPDR &= ~(3U << 4U);

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;
    USART2_BRR = 139U; /* PCLK1=16 MHz, 115200 baud */
    USART2_CR1 = (1U << 3U) | (1U << 13U);
}

static void uart2_send_char(char c)
{
    while ((USART2_SR & (1U << 7U)) == 0U) { }
    USART2_DR = (uint8_t)c;
}

static void uart2_send_string(const char *str)
{
    while (*str) uart2_send_char(*str++);
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
    while (i > 0U) uart2_send_char(buffer[--i]);
}

static void uart2_send_temperature(int16_t raw)
{
    int32_t temp_x10 = ((int32_t)raw * 10) / 16;

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

/* DS18B20 One-Wire interface on PA6. Use an external 4.7k pull-up. */
static void one_wire_release(void)
{
    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
    GPIOA_OTYPER |= (1U << DS18B20_PIN);
    GPIOA_PUPDR &= ~(3U << (DS18B20_PIN * 2U));
}

static void one_wire_low(void)
{
    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
    GPIOA_MODER |=  (1U << (DS18B20_PIN * 2U));
    GPIOA_OTYPER |= (1U << DS18B20_PIN);
    GPIOA_ODR &= ~(1U << DS18B20_PIN);
}

static uint8_t one_wire_read(void)
{
    return (GPIOA_IDR & (1U << DS18B20_PIN)) ? 1U : 0U;
}

static void one_wire_write_bit(uint8_t bit)
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

static uint8_t one_wire_read_bit(void)
{
    uint8_t bit;
    one_wire_low();
    delay_us(3U);
    one_wire_release();
    delay_us(10U);
    bit = one_wire_read();
    delay_us(50U);
    return bit;
}

static void one_wire_write_byte(uint8_t data)
{
    uint8_t i;
    for (i = 0U; i < 8U; i++)
    {
        one_wire_write_bit(data & 0x01U);
        data >>= 1U;
    }
}

static uint8_t one_wire_read_byte(void)
{
    uint8_t i;
    uint8_t data = 0U;
    for (i = 0U; i < 8U; i++)
    {
        if (one_wire_read_bit()) data |= (1U << i);
    }
    return data;
}

static uint8_t one_wire_reset(void)
{
    uint8_t presence;
    one_wire_low();
    delay_us(480U);
    one_wire_release();
    delay_us(70U);
    presence = (one_wire_read() == 0U) ? 1U : 0U;
    delay_us(410U);
    return presence;
}

static int16_t ds18b20_read_temperature_raw(void)
{
    uint8_t lsb, msb;
    int16_t raw;

    if (!one_wire_reset()) return -32768;

    one_wire_write_byte(0xCCU); /* Skip ROM: one sensor on bus */
    one_wire_write_byte(0x44U); /* Convert T */
    delay_us(750000U);          /* Max 12-bit conversion time */

    if (!one_wire_reset()) return -32768;

    one_wire_write_byte(0xCCU);
    one_wire_write_byte(0xBEU); /* Read Scratchpad */
    lsb = one_wire_read_byte();
    msb = one_wire_read_byte();
    raw = (int16_t)(((uint16_t)msb << 8U) | lsb);
    return raw;
}

/* SSD1306 I2C1: PB8=SCL, PB9=SDA, standard mode 100 kHz. */
static void oled_i2c_init(void)
{
    RCC_AHB1ENR |= (1U << 1U);
    RCC_APB1ENR |= (1U << 21U);

    GPIOB_MODER &= ~((3U << 16U) | (3U << 18U));
    GPIOB_MODER |=  ((2U << 16U) | (2U << 18U));
    GPIOB_OTYPER |= (1U << 8U) | (1U << 9U);
    GPIOB_OSPEEDR |= (3U << 16U) | (3U << 18U);
    GPIOB_PUPDR &= ~((3U << 16U) | (3U << 18U));
    GPIOB_AFRH &= ~((0xFU << 0U) | (0xFU << 4U));
    GPIOB_AFRH |=  ((4U << 0U) | (4U << 4U));

    I2C1_CR1 = 0U;
    I2C1_CR2 = 16U;          /* APB1 clock MHz */
    I2C1_OAR1 = (1U << 14U);
    I2C1_CCR = 80U;          /* 16 MHz / (2*80) = 100 kHz */
    I2C1_TRISE = 17U;
    I2C1_CR1 |= (1U << 0U);
}

static uint8_t oled_i2c_start(void)
{
    uint32_t timeout = 100000U;
    I2C1_CR1 |= (1U << 8U);
    while ((I2C1_SR1 & (1U << 0U)) == 0U)
    {
        if (--timeout == 0U) return 0U;
    }
    return 1U;
}

static void oled_i2c_stop(void)
{
    I2C1_CR1 |= (1U << 9U);
}

static uint8_t oled_i2c_address(void)
{
    uint32_t timeout = 100000U;
    I2C1_DR = (OLED_ADDRESS << 1U);

    while (1)
    {
        if (I2C1_SR1 & (1U << 1U))
        {
            volatile uint32_t temp = I2C1_SR1;
            temp = I2C1_SR2; /* Clear ADDR */
            (void)temp;
            return 1U;
        }
        if (I2C1_SR1 & (1U << 10U))
        {
            oled_i2c_stop();
            return 0U;
        }
        if (--timeout == 0U)
        {
            oled_i2c_stop();
            return 0U;
        }
    }
}

static uint8_t oled_i2c_write(uint8_t data)
{
    uint32_t timeout = 100000U;
    I2C1_DR = data;
    while ((I2C1_SR1 & (1U << 7U)) == 0U)
    {
        if (--timeout == 0U) return 0U;
    }

    timeout = 100000U;
    while ((I2C1_SR1 & (1U << 2U)) == 0U)
    {
        if (--timeout == 0U) return 0U;
    }
    return 1U;
}

static uint8_t oled_command(uint8_t command)
{
    if (!oled_i2c_start()) return 0U;
    if (!oled_i2c_address())
    {
        oled_i2c_stop();
        return 0U;
    }
    if (!oled_i2c_write(0x00U) || !oled_i2c_write(command))
    {
        oled_i2c_stop();
        return 0U;
    }
    oled_i2c_stop();
    return 1U;
}

static void oled_init(void)
{
    delay_us(100000U);
    oled_command(0xAEU); /* Display OFF */
    oled_command(0xD5U); oled_command(0x80U);
    oled_command(0xA8U); oled_command(0x3FU);
    oled_command(0xD3U); oled_command(0x00U);
    oled_command(0x40U);
    oled_command(0x8DU); oled_command(0x14U);
    oled_command(0x20U); oled_command(0x00U);
    oled_command(0xA1U);
    oled_command(0xC8U);
    oled_command(0xDAU); oled_command(0x12U);
    oled_command(0x81U); oled_command(0x7FU);
    oled_command(0xD9U); oled_command(0xF1U);
    oled_command(0xDBU); oled_command(0x40U);
    oled_command(0xA4U);
    oled_command(0xA6U);
    oled_command(0xAFU); /* Display ON */
    delay_us(100000U);
}

static void oled_set_position(uint8_t page, uint8_t column)
{
    oled_command(0xB0U | page);
    oled_command(0x00U | (column & 0x0FU));
    oled_command(0x10U | ((column >> 4U) & 0x0FU));
}

static uint8_t oled_data(uint8_t data)
{
    if (!oled_i2c_start()) return 0U;
    if (!oled_i2c_address())
    {
        oled_i2c_stop();
        return 0U;
    }
    if (!oled_i2c_write(0x40U) || !oled_i2c_write(data))
    {
        oled_i2c_stop();
        return 0U;
    }
    oled_i2c_stop();
    return 1U;
}

/* Diagnostic test: fill all 128x64 pixels (8 pages x 128 columns). */
static void oled_test(void)
{
    uint8_t page, column;
    for (page = 0U; page < 8U; page++)
    {
        oled_set_position(page, 0U);
        for (column = 0U; column < 128U; column++)
        {
            (void)oled_data(0xFFU);
        }
    }
}

int main(void)
{
    int16_t temperature_raw;

    RCC_AHB1ENR |= (1U << 0U);
    RCC_CR |= (1U << 0U);      /* Enable HSI 16 MHz */
    RCC_CFGR &= ~(3U << 0U);   /* Select HSI as SYSCLK */

    delay_init();
    uart2_init();
    one_wire_release();

    oled_i2c_init();
    oled_init();
    oled_test();               /* Screen stays fully ON for display test */

    uart2_send_string("\r\n================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 MONITOR\r\n");
    uart2_send_string("UART2: 115200 8N1\r\n");
    uart2_send_string("DS18B20: PA6\r\n");
    uart2_send_string("OLED: PB8=SCL PB9=SDA, address 0x3C\r\n");
    uart2_send_string("OLED test: all pixels ON\r\n");
    uart2_send_string("================================\r\n");

    while (1)
    {
        temperature_raw = ds18b20_read_temperature_raw();

        if (temperature_raw == -32768)
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
