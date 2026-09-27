#include <stdint.h>

/* ============================================================
 * STM32F446RE REGISTER DEFINITIONS
 * ============================================================ */

/* ---------------- RCC ---------------- */
#define RCC_BASE            0x40023800U
#define RCC_CR              (*(volatile uint32_t *)(RCC_BASE + 0x00U))
#define RCC_CFGR            (*(volatile uint32_t *)(RCC_BASE + 0x08U))
#define RCC_AHB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x40U))

/* ---------------- GPIOA ---------------- */
#define GPIOA_BASE          0x40020000U
#define GPIOA_MODER         (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OTYPER        (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
#define GPIOA_OSPEEDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x08U))
#define GPIOA_PUPDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_IDR           (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_BSRR          (*(volatile uint32_t *)(GPIOA_BASE + 0x18U))
#define GPIOA_AFRL          (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

/* ---------------- GPIOB ---------------- */
#define GPIOB_BASE          0x40020400U
#define GPIOB_MODER         (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_OTYPER        (*(volatile uint32_t *)(GPIOB_BASE + 0x04U))
#define GPIOB_OSPEEDR       (*(volatile uint32_t *)(GPIOB_BASE + 0x08U))
#define GPIOB_PUPDR         (*(volatile uint32_t *)(GPIOB_BASE + 0x0CU))
#define GPIOB_ODR           (*(volatile uint32_t *)(GPIOB_BASE + 0x14U))
#define GPIOB_AFRH          (*(volatile uint32_t *)(GPIOB_BASE + 0x24U))

/* ---------------- USART2 ---------------- */
#define USART2_BASE         0x40004400U
#define USART2_SR           (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR           (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR          (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1          (*(volatile uint32_t *)(USART2_BASE + 0x0CU))
#define USART2_CR2          (*(volatile uint32_t *)(USART2_BASE + 0x10U))
#define USART2_CR3          (*(volatile uint32_t *)(USART2_BASE + 0x14U))

/* ---------------- I2C1 ---------------- */
#define I2C1_BASE           0x40005400U
#define I2C1_CR1            (*(volatile uint32_t *)(I2C1_BASE + 0x00U))
#define I2C1_CR2            (*(volatile uint32_t *)(I2C1_BASE + 0x04U))
#define I2C1_OAR1           (*(volatile uint32_t *)(I2C1_BASE + 0x08U))
#define I2C1_DR             (*(volatile uint32_t *)(I2C1_BASE + 0x10U))
#define I2C1_SR1            (*(volatile uint32_t *)(I2C1_BASE + 0x14U))
#define I2C1_SR2            (*(volatile uint32_t *)(I2C1_BASE + 0x18U))
#define I2C1_CCR            (*(volatile uint32_t *)(I2C1_BASE + 0x1CU))
#define I2C1_TRISE          (*(volatile uint32_t *)(I2C1_BASE + 0x20U))

/* ---------------- SysTick ---------------- */
#define SYSTICK_BASE        0xE000E010U
#define SYSTICK_CTRL        (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD        (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL         (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))

/* ---------------- DS18B20 ---------------- */
#define DS18B20_PIN         6U

/* ---------------- OLED ---------------- */
#define OLED_ADDRESS        0x3CU

/* ---------------- BUZZER ---------------- */
#define BUZZER_PIN          0U

/* ---------------- INA219 ---------------- */
#define INA219_ADDRESS      0x40U
#define INA219_REG_CONFIG   0x00U
#define INA219_REG_SHUNT    0x01U
#define INA219_REG_BUS      0x02U
#define INA219_REG_POWER    0x03U
#define INA219_REG_CURRENT  0x04U
#define INA219_REG_CALIB    0x05U

/* ---------------- MPU6050 ---------------- */
#define MPU6050_ADDRESS          0x68U

#define MPU6050_REG_SMPLRT_DIV   0x19U
#define MPU6050_REG_CONFIG       0x1AU
#define MPU6050_REG_GYRO_CONFIG  0x1BU
#define MPU6050_REG_ACCEL_CONFIG 0x1CU

#define MPU6050_REG_ACCEL_XOUT_H 0x3BU
#define MPU6050_REG_TEMP_OUT_H   0x41U
#define MPU6050_REG_GYRO_XOUT_H  0x43U

#define MPU6050_REG_PWR_MGMT_1   0x6BU
#define MPU6050_REG_WHO_AM_I     0x75U

/* ============================================================
 * DELAY
 * ============================================================ */

static void delay_init(void)
{
    SYSTICK_LOAD = 16U - 1U;
    SYSTICK_VAL  = 0U;
    SYSTICK_CTRL = (1U << 2) | (1U << 0);
}

static void delay_us(uint32_t us)
{
    while (us--)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0U)
        {
        }
    }
}

static void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        delay_us(1000U);
    }
}

/* ============================================================
 * DS18B20 GPIO
 * ============================================================ */

static void pin_output(void)
{
    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
    GPIOA_MODER |=  (1U << (DS18B20_PIN * 2U));
    GPIOA_OTYPER |= (1U << DS18B20_PIN);
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
    pin_input();
}

static uint8_t one_wire_read_pin(void)
{
    return (GPIOA_IDR & (1U << DS18B20_PIN)) ? 1U : 0U;
}

/* ============================================================
 * DS18B20 RESET
 * ============================================================ */

static uint8_t ds18b20_reset(void)
{
    uint8_t presence;

    one_wire_low();
    delay_us(480);

    one_wire_release();
    delay_us(70);

    presence = (one_wire_read_pin() == 0U) ? 1U : 0U;

    delay_us(410);

    return presence;
}

/* ============================================================
 * DS18B20 WRITE BIT
 * ============================================================ */

static void ds18b20_write_bit(uint8_t bit)
{
    one_wire_low();

    if (bit)
    {
        delay_us(6);
        one_wire_release();
        delay_us(64);
    }
    else
    {
        delay_us(60);
        one_wire_release();
        delay_us(10);
    }
}

/* ============================================================
 * DS18B20 READ BIT
 * ============================================================ */

static uint8_t ds18b20_read_bit(void)
{
    uint8_t bit;

    one_wire_low();
    delay_us(2);

    one_wire_release();
    delay_us(10);

    bit = one_wire_read_pin();

    delay_us(50);

    return bit;
}

/* ============================================================
 * DS18B20 WRITE BYTE
 * ============================================================ */

static void ds18b20_write_byte(uint8_t data)
{
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        ds18b20_write_bit(data & 0x01U);
        data >>= 1;
    }
}

/* ============================================================
 * DS18B20 READ BYTE
 * ============================================================ */

static uint8_t ds18b20_read_byte(void)
{
    uint8_t data = 0U;
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        if (ds18b20_read_bit())
        {
            data |= (1U << i);
        }
    }

    return data;
}

/* ============================================================
 * DS18B20 READ TEMPERATURE
 * ============================================================ */

static int16_t ds18b20_read_temperature_raw(void)
{
    uint8_t temp_lsb;
    uint8_t temp_msb;
    int16_t raw_temp;

    if (!ds18b20_reset())
    {
        return -32768;
    }

    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0x44);

    delay_us(750000U);

    if (!ds18b20_reset())
    {
        return -32768;
    }

    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0xBE);

    temp_lsb = ds18b20_read_byte();
    temp_msb = ds18b20_read_byte();

    raw_temp = (int16_t)(((uint16_t)temp_msb << 8) | temp_lsb);

    return raw_temp;
}

/* ============================================================
 * USART2 INITIALIZATION
 * ============================================================ */

static void uart2_init(void)
{
    RCC_AHB1ENR |= (1U << 0);
    RCC_APB1ENR |= (1U << 17);

    GPIOA_MODER &= ~(3U << (2U * 2U));
    GPIOA_MODER |=  (2U << (2U * 2U));

    GPIOA_AFRL &= ~(0xFU << 8);
    GPIOA_AFRL |=  (7U << 8);

    GPIOA_PUPDR &= ~(3U << (2U * 2U));

    USART2_BRR = 139U;

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;

    USART2_CR1 |= (1U << 3);
    USART2_CR1 |= (1U << 13);
}

/* ============================================================
 * UART SEND CHARACTER
 * ============================================================ */

static void uart2_send_char(char c)
{
    while ((USART2_SR & (1U << 7)) == 0U)
    {
    }

    USART2_DR = (uint8_t)c;
}

/* ============================================================
 * UART SEND STRING
 * ============================================================ */

static void uart2_send_string(const char *str)
{
    while (*str)
    {
        uart2_send_char(*str++);
    }
}

/* ============================================================
 * UART SEND INTEGER
 * ============================================================ */

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

/* ============================================================
 * UART SEND SIGNED INTEGER
 * ============================================================ */

static void uart2_send_int(int32_t value)
{
    if (value < 0)
    {
        uart2_send_char('-');
        value = -value;
    }

    uart2_send_uint((uint32_t)value);
}

/* ============================================================
 * UART SEND TEMPERATURE
 * ============================================================ */

static void uart2_send_temperature(int16_t raw_temp)
{
    int32_t temp_x10;

    temp_x10 = ((int32_t)raw_temp * 10) / 16;

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

/* ============================================================
 * I2C1 INITIALIZATION
 * PB8 = SCL
 * PB9 = SDA
 * ============================================================ */

static void i2c1_init(void)
{
    RCC_AHB1ENR |= (1U << 1);
    RCC_APB1ENR |= (1U << 21);

    GPIOB_MODER &= ~((3U << 16) | (3U << 18));
    GPIOB_MODER |=  ((2U << 16) | (2U << 18));

    GPIOB_OTYPER |= ((1U << 8) | (1U << 9));

    GPIOB_OSPEEDR |= ((3U << 16) | (3U << 18));

    GPIOB_PUPDR &= ~((3U << 16) | (3U << 18));

    GPIOB_AFRH &= ~((0xFU << 0) | (0xFU << 4));
    GPIOB_AFRH |=  ((4U << 0) | (4U << 4));

    I2C1_CR1 = 0U;

    I2C1_CR1 |= (1U << 15);
    delay_us(10U);
    I2C1_CR1 &= ~(1U << 15);

    I2C1_CR2 = 16U;
    I2C1_OAR1 = (1U << 14);

    I2C1_CCR = 80U;
    I2C1_TRISE = 17U;

    I2C1_CR1 |= (1U << 0);
}

/* ============================================================
 * I2C START
 * ============================================================ */

static uint8_t i2c1_start(void)
{
    uint32_t timeout = 100000U;

    while (I2C1_SR2 & (1U << 1))
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    I2C1_CR1 &= ~(1U << 9);
    I2C1_CR1 |= (1U << 8);

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 0)) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    return 1U;
}

/* ============================================================
 * I2C STOP
 * ============================================================ */

static void i2c1_stop(void)
{
    I2C1_CR1 |= (1U << 9);
}

/* ============================================================
 * I2C ADDRESS
 * ============================================================ */

static uint8_t i2c1_address(uint8_t address)
{
    uint32_t timeout = 100000U;

    I2C1_DR = address;

    while ((I2C1_SR1 & ((1U << 1) | (1U << 10))) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    if (I2C1_SR1 & (1U << 10))
    {
        I2C1_SR1 &= ~(1U << 10);
        return 0U;
    }

    (void)I2C1_SR1;
    (void)I2C1_SR2;

    return 1U;
}

/* ============================================================
 * I2C WRITE
 * ============================================================ */

static uint8_t i2c1_write(uint8_t data)
{
    uint32_t timeout = 100000U;

    I2C1_DR = data;

    while ((I2C1_SR1 & (1U << 7)) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 2)) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    return 1U;
}

/* ============================================================
 * INA219 WRITE REGISTER
 * ============================================================ */

static uint8_t ina219_write_register(uint8_t reg, uint16_t value)
{
    if (!i2c1_start()) return 0U;

    if (!i2c1_address((INA219_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write((uint8_t)(value >> 8)))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write((uint8_t)(value & 0xFFU)))
    {
        i2c1_stop();
        return 0U;
    }

    i2c1_stop();

    return 1U;
}

/* ============================================================
 * INA219 READ REGISTER
 * ============================================================ */

static uint8_t ina219_read_register(uint8_t reg, uint16_t *value)
{
    uint32_t timeout;
    uint8_t high_byte;
    uint8_t low_byte;

    if (!i2c1_start()) return 0U;

    if (!i2c1_address((INA219_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    I2C1_CR1 |= (1U << 8);

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 0)) == 0U)
    {
        if (--timeout == 0U)
        {
            i2c1_stop();
            return 0U;
        }
    }

    I2C1_DR = (INA219_ADDRESS << 1) | 1U;

    timeout = 100000U;

    while ((I2C1_SR1 & ((1U << 1) | (1U << 10))) == 0U)
    {
        if (--timeout == 0U)
        {
            i2c1_stop();
            return 0U;
        }
    }

    if (I2C1_SR1 & (1U << 10))
    {
        I2C1_SR1 &= ~(1U << 10);
        i2c1_stop();
        return 0U;
    }

    I2C1_CR1 |= (1U << 10);

    (void)I2C1_SR1;
    (void)I2C1_SR2;

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 2)) == 0U)
    {
        if (--timeout == 0U)
        {
            I2C1_CR1 &= ~(1U << 10);
            i2c1_stop();
            return 0U;
        }
    }

    I2C1_CR1 &= ~(1U << 10);
    I2C1_CR1 |= (1U << 9);

    high_byte = (uint8_t)I2C1_DR;
    low_byte  = (uint8_t)I2C1_DR;

    *value = ((uint16_t)high_byte << 8) | low_byte;

    return 1U;
}

/* ============================================================
 * INA219 INITIALIZATION
 * ============================================================ */

static uint8_t ina219_init(void)
{
    if (!ina219_write_register(INA219_REG_CALIB, 4096U))
    {
        return 0U;
    }

    if (!ina219_write_register(INA219_REG_CONFIG, 0x199FU))
    {
        return 0U;
    }

    return 1U;
}

/* ============================================================
 * INA219 MEASUREMENTS
 * ============================================================ */

static uint8_t ina219_read_measurements(
    uint32_t *bus_voltage_mV,
    int32_t *shunt_voltage_uV,
    int32_t *current_mA,
    uint32_t *power_mW)
{
    uint16_t bus_raw;
    uint16_t shunt_raw_u;
    uint16_t current_raw_u;
    int16_t shunt_raw;
    int16_t current_raw;

    if (!ina219_read_register(INA219_REG_BUS, &bus_raw))
    {
        return 0U;
    }

    *bus_voltage_mV = (uint32_t)(bus_raw >> 3) * 4U;

    if (!ina219_read_register(INA219_REG_SHUNT, &shunt_raw_u))
    {
        return 0U;
    }

    shunt_raw = (int16_t)shunt_raw_u;
    *shunt_voltage_uV = (int32_t)shunt_raw * 10;

    if (!ina219_read_register(INA219_REG_CURRENT, &current_raw_u))
    {
        return 0U;
    }

    current_raw = (int16_t)current_raw_u;
    *current_mA = (int32_t)current_raw / 10;

    *power_mW =
        ((*bus_voltage_mV) *
         ((uint32_t)(*current_mA >= 0 ?
                     *current_mA :
                     -*current_mA))) / 1000U;

    return 1U;
}

/* ============================================================
 * UART INA219
 * ============================================================ */

static void uart2_send_ina219(
    uint32_t bus_voltage_mV,
    int32_t current_mA,
    uint32_t power_mW)
{
    uart2_send_string("INA219:\r\n");
    uart2_send_string("Bus Voltage: ");
    uart2_send_uint(bus_voltage_mV / 1000U);
    uart2_send_char('.');
    uart2_send_char(
        (char)('0' + ((bus_voltage_mV / 100U) % 10U)));
    uart2_send_char(
        (char)('0' + ((bus_voltage_mV / 10U) % 10U)));
    uart2_send_string(" V\r\n");

    uart2_send_string("Current: ");

    if (current_mA < 0)
    {
        uart2_send_char('-');
        current_mA = -current_mA;
    }

    uart2_send_uint((uint32_t)(current_mA / 1000));
    uart2_send_char('.');
    uart2_send_char(
        (char)('0' + ((current_mA / 100) % 10)));
    uart2_send_char(
        (char)('0' + ((current_mA / 10) % 10)));
    uart2_send_char(
        (char)('0' + (current_mA % 10)));
    uart2_send_string(" A\r\n");

    uart2_send_string("Power: ");
    uart2_send_uint(power_mW / 1000U);
    uart2_send_char('.');
    uart2_send_char(
        (char)('0' + ((power_mW / 100U) % 10U)));
    uart2_send_char(
        (char)('0' + ((power_mW / 10U) % 10U)));
    uart2_send_char(
        (char)('0' + (power_mW % 10U)));
    uart2_send_string(" W\r\n");

    uart2_send_string("-------------------------\r\n");
}

/* ============================================================
 * OLED COMMAND
 * ============================================================ */

static uint8_t oled_command(uint8_t command)
{
    if (!i2c1_start())
    {
        return 0U;
    }

    if (!i2c1_address(OLED_ADDRESS << 1))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(0x00U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(command))
    {
        i2c1_stop();
        return 0U;
    }

    i2c1_stop();

    return 1U;
}

/* ============================================================
 * OLED INITIALIZATION
 * ============================================================ */

static void oled_init(void)
{
    delay_us(100000U);

    oled_command(0xAE);
    oled_command(0xD5);
    oled_command(0x80);
    oled_command(0xA8);
    oled_command(0x3F);
    oled_command(0xD3);
    oled_command(0x00);
    oled_command(0x40);
    oled_command(0x8D);
    oled_command(0x14);
    oled_command(0x20);
    oled_command(0x00);
    oled_command(0xA1);
    oled_command(0xC8);
    oled_command(0xDA);
    oled_command(0x12);
    oled_command(0x81);
    oled_command(0x7F);
    oled_command(0xD9);
    oled_command(0xF1);
    oled_command(0xDB);
    oled_command(0x40);
    oled_command(0xA4);
    oled_command(0xA6);
    oled_command(0xAF);

    delay_us(100000U);
}

/* ============================================================
 * OLED SET POSITION
 * ============================================================ */

static void oled_set_position(uint8_t page, uint8_t column)
{
    oled_command(0xB0U | page);
    oled_command(0x00U | (column & 0x0FU));
    oled_command(0x10U | ((column >> 4) & 0x0FU));
}

/* ============================================================
 * OLED DATA START
 * ============================================================ */

static uint8_t oled_data_start(void)
{
    if (!i2c1_start())
    {
        return 0U;
    }

    if (!i2c1_address(OLED_ADDRESS << 1))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(0x40U))
    {
        i2c1_stop();
        return 0U;
    }

    return 1U;
}

/* ============================================================
 * OLED DATA STOP
 * ============================================================ */

static void oled_data_stop(void)
{
    i2c1_stop();
}

/* ============================================================
 * OLED FONT
 * ============================================================ */

static const uint8_t font_5x7[10][5] =
{
    {0x3E,0x51,0x49,0x45,0x3E},
    {0x00,0x42,0x7F,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46},
    {0x21,0x41,0x45,0x4B,0x31},
    {0x18,0x14,0x12,0x7F,0x10},
    {0x27,0x45,0x45,0x45,0x39},
    {0x3C,0x4A,0x49,0x49,0x30},
    {0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36},
    {0x06,0x49,0x49,0x29,0x1E}
};

static uint8_t get_font(char c, uint8_t *font)
{
    uint8_t i;

    if (c >= '0' && c <= '9')
    {
        uint8_t digit = (uint8_t)(c - '0');

        for (i = 0U; i < 5U; i++)
        {
            font[i] = font_5x7[digit][i];
        }

        return 1U;
    }

    if (c == 'T')
    {
        font[0]=0x01; font[1]=0x01; font[2]=0x7F; font[3]=0x01; font[4]=0x01;
        return 1U;
    }

    if (c == 'E')
    {
        font[0]=0x7F; font[1]=0x49; font[2]=0x49; font[3]=0x49; font[4]=0x41;
        return 1U;
    }

    if (c == 'M')
    {
        font[0]=0x7F; font[1]=0x02; font[2]=0x0C; font[3]=0x02; font[4]=0x7F;
        return 1U;
    }

    if (c == 'P')
    {
        font[0]=0x7F; font[1]=0x09; font[2]=0x09; font[3]=0x09; font[4]=0x06;
        return 1U;
    }

    if (c == 'C')
    {
        font[0]=0x3E; font[1]=0x41; font[2]=0x41; font[3]=0x41; font[4]=0x22;
        return 1U;
    }

    if (c == 'I')
    {
        font[0]=0x41; font[1]=0x41; font[2]=0x7F; font[3]=0x41; font[4]=0x41;
        return 1U;
    }

    if (c == 'N')
    {
        font[0]=0x7F; font[1]=0x06; font[2]=0x18; font[3]=0x60; font[4]=0x7F;
        return 1U;
    }

    if (c == 'V')
    {
        font[0]=0x07; font[1]=0x38; font[2]=0x40; font[3]=0x38; font[4]=0x07;
        return 1U;
    }

    if (c == 'A')
    {
        font[0]=0x7E; font[1]=0x09; font[2]=0x09; font[3]=0x09; font[4]=0x7E;
        return 1U;
    }

    if (c == 'W')
    {
        font[0]=0x7F; font[1]=0x30; font[2]=0x0C; font[3]=0x30; font[4]=0x7F;
        return 1U;
    }

    if (c == 'U')
    {
        font[0]=0x3F; font[1]=0x40; font[2]=0x40; font[3]=0x40; font[4]=0x3F;
        return 1U;
    }

    if (c == 'X')
    {
        font[0]=0x63; font[1]=0x14; font[2]=0x08; font[3]=0x14; font[4]=0x63;
        return 1U;
    }

    if (c == 'Y')
    {
        font[0]=0x03; font[1]=0x04; font[2]=0x78; font[3]=0x04; font[4]=0x03;
        return 1U;
    }

    if (c == 'Z')
    {
        font[0]=0x61; font[1]=0x51; font[2]=0x49; font[3]=0x45; font[4]=0x43;
        return 1U;
    }

    if (c == 'G')
    {
        font[0]=0x3E; font[1]=0x41; font[2]=0x49; font[3]=0x49; font[4]=0x7A;
        return 1U;
    }

    if (c == ' ')
    {
        font[0]=0x00; font[1]=0x00; font[2]=0x00; font[3]=0x00; font[4]=0x00;
        return 1U;
    }

    if (c == '.')
    {
        font[0]=0x00; font[1]=0x60; font[2]=0x60; font[3]=0x00; font[4]=0x00;
        return 1U;
    }

    if (c == '-')
    {
        font[0]=0x08; font[1]=0x08; font[2]=0x08; font[3]=0x08; font[4]=0x08;
        return 1U;
    }

    return 0U;
}

static void oled_print_string(const char *str)
{
    uint8_t font[5];
    uint8_t i;

    if (!oled_data_start())
    {
        return;
    }

    while (*str)
    {
        if (get_font(*str, font))
        {
            for (i = 0U; i < 5U; i++)
            {
                i2c1_write(font[i]);
            }

            i2c1_write(0x00U);
        }

        str++;
    }

    oled_data_stop();
}

static void oled_clear_page(uint8_t page)
{
    uint16_t i;

    oled_set_position(page, 0U);

    if (!oled_data_start())
    {
        return;
    }

    for (i = 0U; i < 128U; i++)
    {
        if (!i2c1_write(0x00U))
        {
            break;
        }
    }

    oled_data_stop();
}

static void oled_clear(void)
{
    uint8_t page;

    for (page = 0U; page < 8U; page++)
    {
        oled_clear_page(page);
    }
}

/* ============================================================
 * OLED TEMPERATURE
 * ============================================================ */

static void oled_show_temperature(int16_t raw_temp)
{
    int32_t temp_x10;
    char text[12];
    uint8_t pos = 0U;

    temp_x10 = ((int32_t)raw_temp * 10) / 16;

    oled_clear();

    oled_set_position(1U, 52U);
    oled_print_string("TEMP");

    if (temp_x10 < 0)
    {
        text[pos++] = '-';
        temp_x10 = -temp_x10;
    }

    if (temp_x10 >= 1000)
    {
        text[pos++] = (char)('0' + ((temp_x10 / 1000) % 10));
    }

    if (temp_x10 >= 100)
    {
        text[pos++] = (char)('0' + ((temp_x10 / 100) % 10));
    }

    text[pos++] = (char)('0' + ((temp_x10 / 10) % 10));
    text[pos++] = '.';
    text[pos++] = (char)('0' + (temp_x10 % 10));
    text[pos++] = ' ';
    text[pos++] = 'C';
    text[pos] = '\0';

    oled_set_position(3U, 43U);
    oled_print_string(text);
}

/* ============================================================
 * OLED INA219
 * ============================================================ */

static void oled_show_ina219(
    uint32_t bus_voltage_mV,
    int32_t current_mA,
    uint32_t power_mW)
{
    char text[20];
    uint8_t pos;

    oled_clear();

    oled_set_position(1U, 43U);
    oled_print_string("INA219");

    pos = 0U;
    text[pos++] = 'V';
    text[pos++] = ' ';
    text[pos++] = (char)('0' + ((bus_voltage_mV / 10000U) % 10U));
    text[pos++] = (char)('0' + ((bus_voltage_mV / 1000U) % 10U));
    text[pos++] = '.';
    text[pos++] = (char)('0' + ((bus_voltage_mV / 100U) % 10U));
    text[pos++] = (char)('0' + ((bus_voltage_mV / 10U) % 10U));
    text[pos++] = ' ';
    text[pos++] = 'V';
    text[pos] = '\0';

    oled_set_position(3U, 28U);
    oled_print_string(text);

    pos = 0U;
    text[pos++] = 'I';
    text[pos++] = ' ';

    if (current_mA < 0)
    {
        text[pos++] = '-';
        current_mA = -current_mA;
    }

    text[pos++] = (char)('0' + ((current_mA / 1000) % 10));
    text[pos++] = '.';
    text[pos++] = (char)('0' + ((current_mA / 100) % 10));
    text[pos++] = (char)('0' + ((current_mA / 10) % 10));
    text[pos++] = (char)('0' + (current_mA % 10));
    text[pos++] = ' ';
    text[pos++] = 'A';
    text[pos] = '\0';

    oled_set_position(5U, 28U);
    oled_print_string(text);

    pos = 0U;
    text[pos++] = 'P';
    text[pos++] = ' ';
    text[pos++] = (char)('0' + ((power_mW / 1000U) % 10U));
    text[pos++] = '.';
    text[pos++] = (char)('0' + ((power_mW / 100U) % 10U));
    text[pos++] = (char)('0' + ((power_mW / 10U) % 10U));
    text[pos++] = (char)('0' + (power_mW % 10U));
    text[pos++] = ' ';
    text[pos++] = 'W';
    text[pos] = '\0';

    oled_set_position(7U, 28U);
    oled_print_string(text);
}

/* ============================================================
 * OLED SIGNED INTEGER
 * ============================================================ */

static void oled_signed_int(int16_t value)
{
    char text[8];
    uint8_t pos = 0U;
    int32_t number = (int32_t)value;

    if (number < 0)
    {
        text[pos++] = '-';
        number = -number;
    }

    if (number >= 10000)
    {
        text[pos++] = (char)('0' + ((number / 10000) % 10));
    }

    if (number >= 1000)
    {
        text[pos++] = (char)('0' + ((number / 1000) % 10));
    }

    if (number >= 100)
    {
        text[pos++] = (char)('0' + ((number / 100) % 10));
    }

    if (number >= 10)
    {
        text[pos++] = (char)('0' + ((number / 10) % 10));
    }

    text[pos++] = (char)('0' + (number % 10));
    text[pos] = '\0';

    oled_print_string(text);
}

/* ============================================================
 * MPU6050 WRITE REGISTER
 * ============================================================ */

static uint8_t mpu6050_write_reg(uint8_t reg, uint8_t value)
{
    if (!i2c1_start())
    {
        return 0U;
    }

    if (!i2c1_address((MPU6050_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(value))
    {
        i2c1_stop();
        return 0U;
    }

    i2c1_stop();
    delay_ms(2U);

    return 1U;
}

/* ============================================================
 * MPU6050 READ REGISTER
 * ============================================================ */

static uint8_t mpu6050_read_reg(uint8_t reg, uint8_t *value)
{
    uint32_t timeout;

    I2C1_CR1 |= (1U << 10);

    if (!i2c1_start())
    {
        return 0U;
    }

    if (!i2c1_address((MPU6050_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    I2C1_CR1 |= (1U << 8);

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 0)) == 0U)
    {
        if (--timeout == 0U)
        {
            i2c1_stop();
            return 0U;
        }
    }

    I2C1_CR1 &= ~(1U << 10);
    I2C1_CR1 &= ~(1U << 11);

    I2C1_DR = (MPU6050_ADDRESS << 1) | 1U;

    timeout = 100000U;

    while ((I2C1_SR1 & ((1U << 1) | (1U << 10))) == 0U)
    {
        if (--timeout == 0U)
        {
            i2c1_stop();
            return 0U;
        }
    }

    if (I2C1_SR1 & (1U << 10))
    {
        I2C1_SR1 &= ~(1U << 10);
        i2c1_stop();
        return 0U;
    }

    (void)I2C1_SR1;
    (void)I2C1_SR2;

    I2C1_CR1 |= (1U << 9);

    timeout = 100000U;

    while ((I2C1_SR1 & (1U << 6)) == 0U)
    {
        if (--timeout == 0U)
        {
            I2C1_CR1 |= (1U << 10);
            return 0U;
        }
    }

    *value = (uint8_t)I2C1_DR;
    I2C1_CR1 |= (1U << 10);

    return 1U;
}

/* ============================================================
 * MPU6050 READ 16-BIT
 * ============================================================ */

static uint8_t mpu6050_read_16bit(uint8_t reg, int16_t *value)
{
    uint8_t high_byte;
    uint8_t low_byte;

    if (!mpu6050_read_reg(reg, &high_byte))
    {
        return 0U;
    }

    if (!mpu6050_read_reg(reg + 1U, &low_byte))
    {
        return 0U;
    }

    *value = (int16_t)(((uint16_t)high_byte << 8U) | low_byte);

    return 1U;
}

/* ============================================================
 * MPU6050 INITIALIZATION
 * ============================================================ */

static uint8_t mpu6050_init(void)
{
    if (!mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1, 0x00U))
    {
        return 0U;
    }

    if (!mpu6050_write_reg(MPU6050_REG_SMPLRT_DIV, 0x07U))
    {
        return 0U;
    }

    if (!mpu6050_write_reg(MPU6050_REG_CONFIG, 0x00U))
    {
        return 0U;
    }

    if (!mpu6050_write_reg(MPU6050_REG_GYRO_CONFIG, 0x00U))
    {
        return 0U;
    }

    if (!mpu6050_write_reg(MPU6050_REG_ACCEL_CONFIG, 0x00U))
    {
        return 0U;
    }

    return 1U;
}

/* ============================================================
 * MPU6050 READ SENSOR DATA
 * ============================================================ */

static uint8_t mpu6050_read_data(
    int16_t *accel_x,
    int16_t *accel_y,
    int16_t *accel_z,
    int16_t *temperature_raw,
    int16_t *gyro_x,
    int16_t *gyro_y,
    int16_t *gyro_z)
{
    if (!mpu6050_read_16bit(MPU6050_REG_ACCEL_XOUT_H, accel_x))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_ACCEL_XOUT_H + 2U, accel_y))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_ACCEL_XOUT_H + 4U, accel_z))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_TEMP_OUT_H, temperature_raw))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_GYRO_XOUT_H, gyro_x))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_GYRO_XOUT_H + 2U, gyro_y))
    {
        return 0U;
    }

    if (!mpu6050_read_16bit(MPU6050_REG_GYRO_XOUT_H + 4U, gyro_z))
    {
        return 0U;
    }

    return 1U;
}

/* ============================================================
 * UART MPU6050
 * ============================================================ */

static void uart2_send_mpu6050(
    int16_t accel_x,
    int16_t accel_y,
    int16_t accel_z,
    int16_t temperature_raw,
    int16_t gyro_x,
    int16_t gyro_y,
    int16_t gyro_z)
{
    uart2_send_string("\r\n");
    uart2_send_string("MPU6050 DATA\r\n");

    uart2_send_string("Accel X: ");
    uart2_send_int(accel_x);
    uart2_send_string("\r\n");

    uart2_send_string("Accel Y: ");
    uart2_send_int(accel_y);
    uart2_send_string("\r\n");

    uart2_send_string("Accel Z: ");
    uart2_send_int(accel_z);
    uart2_send_string("\r\n");

    uart2_send_string("Temperature raw: ");
    uart2_send_int(temperature_raw);
    uart2_send_string("\r\n");

    uart2_send_string("Gyro X: ");
    uart2_send_int(gyro_x);
    uart2_send_string("\r\n");

    uart2_send_string("Gyro Y: ");
    uart2_send_int(gyro_y);
    uart2_send_string("\r\n");

    uart2_send_string("Gyro Z: ");
    uart2_send_int(gyro_z);
    uart2_send_string("\r\n");

    uart2_send_string("-------------------------\r\n");
}

/* ============================================================
 * OLED MPU6050
 * ============================================================ */

static void oled_show_mpu6050(
    int16_t accel_x,
    int16_t accel_y,
    int16_t accel_z,
    int16_t temperature_raw,
    int16_t gyro_x,
    int16_t gyro_y,
    int16_t gyro_z)
{
    oled_clear();

    oled_set_position(0U, 43U);
    oled_print_string("MPU");

    oled_set_position(1U, 0U);
    oled_print_string("X ");
    oled_signed_int(accel_x);

    oled_set_position(2U, 0U);
    oled_print_string("Y ");
    oled_signed_int(accel_y);

    oled_set_position(3U, 0U);
    oled_print_string("Z ");
    oled_signed_int(accel_z);

    oled_set_position(4U, 0U);
    oled_print_string("T ");
    oled_signed_int(temperature_raw);

    oled_set_position(5U, 0U);
    oled_print_string("G X ");
    oled_signed_int(gyro_x);

    oled_set_position(6U, 0U);
    oled_print_string("G Y ");
    oled_signed_int(gyro_y);

    oled_set_position(7U, 0U);
    oled_print_string("G Z ");
    oled_signed_int(gyro_z);
}

/* ============================================================
 * OLED TEMPERATURE ERROR
 * ============================================================ */

static void oled_show_error(void)
{
    oled_clear();

    oled_set_position(1U, 52U);
    oled_print_string("TEMP");

    oled_set_position(3U, 49U);
    oled_print_string("ERR");
}

/* ============================================================
 * BUZZER
 * ============================================================ */

static void buzzer_init(void)
{
    RCC_AHB1ENR |= (1U << 1);

    GPIOB_MODER &= ~(3U << (BUZZER_PIN * 2U));
    GPIOB_MODER |=  (1U << (BUZZER_PIN * 2U));

    GPIOB_OTYPER &= ~(1U << BUZZER_PIN);
    GPIOB_ODR &= ~(1U << BUZZER_PIN);
}

static void buzzer_on(void)
{
    GPIOB_ODR |= (1U << BUZZER_PIN);
}

static void buzzer_off(void)
{
    GPIOB_ODR &= ~(1U << BUZZER_PIN);
}

/* ============================================================
 * MAIN
 * ============================================================ */

int main(void)
{
    int16_t temperature_raw;

    uint32_t bus_voltage_mV;
    int32_t shunt_voltage_uV;
    int32_t current_mA;
    uint32_t power_mW;

    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t mpu_temperature_raw;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;

    uint8_t ina219_ok;
    uint8_t mpu6050_ok;
    uint8_t mpu_data_ok;

    RCC_CR |= (1U << 0);
    RCC_CFGR &= ~(3U << 0);

    RCC_AHB1ENR |= (1U << 0);

    delay_init();
    uart2_init();
    buzzer_init();

    one_wire_release();

    i2c1_init();

    uart2_send_string("\r\n");
    uart2_send_string("================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 + INA219 + OLED + MPU6050\r\n");
    uart2_send_string("================================\r\n");

    uart2_send_string("\r\n");
    uart2_send_string("================================\r\n");
    uart2_send_string("STM32 INDUSTRIAL MONITORING\r\n");
    uart2_send_string("================================\r\n");

    buzzer_on();
    delay_ms(100);
    buzzer_off();

    oled_init();
    oled_clear();

    ina219_ok = ina219_init();

    if (!ina219_ok)
    {
        uart2_send_string("INA219 Initialization Failed!\r\n");
    }

    mpu6050_ok = mpu6050_init();

    if (!mpu6050_ok)
    {
        uart2_send_string("MPU6050 Initialization Failed!\r\n");
    }
    else
    {
        uart2_send_string("MPU6050 Initialized\r\n");
    }

    delay_ms(100);

    while (1)
    {
        temperature_raw = ds18b20_read_temperature_raw();

        uart2_send_string("\r\n");
        uart2_send_string("================================\r\n");
        uart2_send_string("DS18B20\r\n");
        uart2_send_string("================================\r\n");

        if (temperature_raw == -32768)
        {
            uart2_send_string("Temperature: SENSOR ERROR\r\n");
            oled_show_error();
        }
        else
        {
            uart2_send_string("Temperature: ");
            uart2_send_temperature(temperature_raw);
            oled_show_temperature(temperature_raw);
        }

        if (ina219_ok &&
            ina219_read_measurements(
                &bus_voltage_mV,
                &shunt_voltage_uV,
                &current_mA,
                &power_mW))
        {
            uart2_send_string("\r\n");
            uart2_send_ina219(
                bus_voltage_mV,
                current_mA,
                power_mW);

            delay_ms(1000);

            oled_show_ina219(
                bus_voltage_mV,
                current_mA,
                power_mW);
        }
        else
        {
            uart2_send_string("INA219 Read Error!\r\n");
        }

        mpu_data_ok = 0U;

        if (mpu6050_ok)
        {
            mpu_data_ok =
                mpu6050_read_data(
                    &accel_x,
                    &accel_y,
                    &accel_z,
                    &mpu_temperature_raw,
                    &gyro_x,
                    &gyro_y,
                    &gyro_z);
        }

        if (mpu_data_ok)
        {
            uart2_send_mpu6050(
                accel_x,
                accel_y,
                accel_z,
                mpu_temperature_raw,
                gyro_x,
                gyro_y,
                gyro_z);

            delay_ms(1000);

            oled_show_mpu6050(
                accel_x,
                accel_y,
                accel_z,
                mpu_temperature_raw,
                gyro_x,
                gyro_y,
                gyro_z);
        }
        else
        {
            uart2_send_string("MPU6050 Read Error!\r\n");
        }

        delay_ms(1000);
    }
}
