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
#define GPIOA_PUPDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_IDR           (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
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

    raw_temp =
        (int16_t)(((uint16_t)temp_msb << 8) | temp_lsb);

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
    GPIOA_MODER |= (2U << (2U * 2U));

    GPIOA_AFRL &= ~(0xFU << 8);
    GPIOA_AFRL |= (7U << 8);

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
        buffer[i++] =
            (char)('0' + (value % 10U));

        value /= 10U;
    }

    while (i > 0U)
    {
        uart2_send_char(buffer[--i]);
    }
}

/* ============================================================
 * UART SEND TEMPERATURE
 * ============================================================ */

static void uart2_send_temperature(int16_t raw_temp)
{
    int32_t temp_x10;

    temp_x10 =
        ((int32_t)raw_temp * 10) / 16;

    if (temp_x10 < 0)
    {
        uart2_send_char('-');
        temp_x10 = -temp_x10;
    }

    uart2_send_uint(
        (uint32_t)(temp_x10 / 10)
    );

    uart2_send_char('.');

    uart2_send_char(
        (char)('0' + (temp_x10 % 10))
    );

    uart2_send_string(" C\r\n");
}

/* ============================================================
 * I2C1 INITIALIZATION
 *
 * PB8 = SCL
 * PB9 = SDA
 * AF4
 * ============================================================ */

static void i2c1_init(void)
{
    RCC_AHB1ENR |= (1U << 1);
    RCC_APB1ENR |= (1U << 21);

    GPIOB_MODER &= ~((3U << 16) | (3U << 18));
    GPIOB_MODER |= ((2U << 16) | (2U << 18));

    GPIOB_OTYPER |= ((1U << 8) | (1U << 9));

    GPIOB_OSPEEDR |= ((3U << 16) | (3U << 18));

    GPIOB_PUPDR &= ~(
        (3U << 16) |
        (3U << 18)
    );

    GPIOB_AFRH &= ~(
        (0xFU << 0) |
        (0xFU << 4)
    );

    GPIOB_AFRH |=
        ((4U << 0) |
         (4U << 4));

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
 * I2C SEND ADDRESS
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
 * I2C WRITE BYTE
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

static void oled_set_position(
    uint8_t page,
    uint8_t column)
{
    oled_command(0xB0U | page);

    oled_command(
        0x00U | (column & 0x0FU)
    );

    oled_command(
        0x10U | ((column >> 4) & 0x0FU)
    );
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
 * 5x7 FONT
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

/* ============================================================
 * GET CHARACTER FONT
 * ============================================================ */

static uint8_t get_font(
    char c,
    uint8_t *font)
{
    uint8_t i;

    if (c >= '0' && c <= '9')
    {
        uint8_t digit =
            (uint8_t)(c - '0');

        for (i = 0U; i < 5U; i++)
        {
            font[i] =
                font_5x7[digit][i];
        }

        return 1U;
    }

    if (c == 'T')
    {
        font[0] = 0x01;
        font[1] = 0x01;
        font[2] = 0x7F;
        font[3] = 0x01;
        font[4] = 0x01;

        return 1U;
    }

    if (c == 'E')
    {
        font[0] = 0x7F;
        font[1] = 0x49;
        font[2] = 0x49;
        font[3] = 0x49;
        font[4] = 0x41;

        return 1U;
    }

    if (c == 'M')
    {
        font[0] = 0x7F;
        font[1] = 0x02;
        font[2] = 0x0C;
        font[3] = 0x02;
        font[4] = 0x7F;

        return 1U;
    }

    if (c == 'P')
    {
        font[0] = 0x7F;
        font[1] = 0x09;
        font[2] = 0x09;
        font[3] = 0x09;
        font[4] = 0x06;

        return 1U;
    }

    if (c == 'C')
    {
        font[0] = 0x3E;
        font[1] = 0x41;
        font[2] = 0x41;
        font[3] = 0x41;
        font[4] = 0x22;

        return 1U;
    }

    if (c == ' ')
    {
        font[0] = 0x00;
        font[1] = 0x00;
        font[2] = 0x00;
        font[3] = 0x00;
        font[4] = 0x00;

        return 1U;
    }

    if (c == '.')
    {
        font[0] = 0x00;
        font[1] = 0x60;
        font[2] = 0x60;
        font[3] = 0x00;
        font[4] = 0x00;

        return 1U;
    }

    if (c == '-')
    {
        font[0] = 0x08;
        font[1] = 0x08;
        font[2] = 0x08;
        font[3] = 0x08;
        font[4] = 0x08;

        return 1U;
    }

    return 0U;
}

/* ============================================================
 * OLED PRINT STRING
 * ============================================================ */

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

/* ============================================================
 * OLED CLEAR ONE PAGE
 * ============================================================ */

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

/* ============================================================
 * OLED CLEAR SCREEN
 * ============================================================ */

static void oled_clear(void)
{
    uint8_t page;

    for (page = 0U; page < 8U; page++)
    {
        oled_clear_page(page);
    }
}

/* ============================================================
 * OLED SHOW TEMPERATURE
 * ============================================================ */

static void oled_show_temperature(
    int16_t raw_temp)
{
    int32_t temp_x10;
    char text[12];
    uint8_t pos = 0U;

    temp_x10 =
        ((int32_t)raw_temp * 10) / 16;

    oled_clear_page(3U);

    oled_set_position(1U, 52U);
    oled_print_string("TEMP");

    if (temp_x10 < 0)
    {
        text[pos++] = '-';
        temp_x10 = -temp_x10;
    }

    if (temp_x10 >= 1000)
    {
        text[pos++] =
            (char)('0' + ((temp_x10 / 1000) % 10));
    }

    if (temp_x10 >= 100)
    {
        text[pos++] =
            (char)('0' + ((temp_x10 / 100) % 10));
    }

    text[pos++] =
        (char)('0' + ((temp_x10 / 10) % 10));

    text[pos++] = '.';

    text[pos++] =
        (char)('0' + (temp_x10 % 10));

    text[pos++] = ' ';
    text[pos++] = 'C';
    text[pos] = '\0';

    oled_set_position(3U, 43U);
    oled_print_string(text);
}

/* ============================================================
 * OLED ERROR
 * ============================================================ */

static void oled_show_error(void)
{
    oled_clear_page(3U);

    oled_set_position(1U, 52U);
    oled_print_string("TEMP");

    oled_set_position(3U, 49U);
    oled_print_string("ERR");
}

/* ============================================================
 * BUZZER PB0
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

    RCC_AHB1ENR |= (1U << 0);

    RCC_CR |= (1U << 0);
    RCC_CFGR &= ~(3U << 0);

    delay_init();

    uart2_init();
    buzzer_init();
    one_wire_release();

    uart2_send_string("\r\n");
    uart2_send_string("================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 + OLED + Buzzer\r\n");
    uart2_send_string("UART2: 115200 8N1\r\n");
    uart2_send_string("DS18B20: PA6\r\n");
    uart2_send_string("OLED: I2C1 PB8/PB9\r\n");
    uart2_send_string("Buzzer: PB0\r\n");
    uart2_send_string("================================\r\n");

    i2c1_init();
    oled_init();
    oled_clear();

    while (1)
    {
        temperature_raw =
            ds18b20_read_temperature_raw();

        if (temperature_raw == -32768)
        {
            uart2_send_string(
                "DS18B20 not detected!\r\n"
            );

            oled_show_error();
            buzzer_off();
        }
        else
        {
            uart2_send_string(
                "Temperature: "
            );

            uart2_send_temperature(
                temperature_raw
            );

            oled_show_temperature(
                temperature_raw
            );

            if (temperature_raw > (27 * 16))
            {
                buzzer_on();
            }
            else
            {
                buzzer_off();
            }
        }

        delay_us(1000000U);
    }
}
