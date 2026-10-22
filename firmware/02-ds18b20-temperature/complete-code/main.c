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


/* ---------------- USART2 ---------------- */

#define USART2_BASE         0x40004400U

#define USART2_SR           (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR           (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR          (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1          (*(volatile uint32_t *)(USART2_BASE + 0x0CU))
#define USART2_CR2          (*(volatile uint32_t *)(USART2_BASE + 0x10U))
#define USART2_CR3          (*(volatile uint32_t *)(USART2_BASE + 0x14U))


/* ---------------- SysTick ---------------- */

#define SYSTICK_BASE        0xE000E010U

#define SYSTICK_CTRL        (*(volatile uint32_t *)(SYSTICK_BASE + 0x00U))
#define SYSTICK_LOAD        (*(volatile uint32_t *)(SYSTICK_BASE + 0x04U))
#define SYSTICK_VAL         (*(volatile uint32_t *)(SYSTICK_BASE + 0x08U))


/* ---------------- DS18B20 ---------------- */

#define DS18B20_PIN         6U       /* PA6 */


/* ============================================================
 * DELAY
 *
 * MCU clock = 16 MHz HSI
 *
 * SysTick = 1 us
 * ============================================================ */

static void delay_init(void)
{
    /*
     * 16 MHz / 1,000,000 = 16 clock cycles per microsecond
     */

    SYSTICK_LOAD = 16U - 1U;
    SYSTICK_VAL  = 0U;

    /*
     * CLKSOURCE = processor clock
     * TICKINT   = 0
     * ENABLE    = 1
     */

    SYSTICK_CTRL = (1U << 2) | (1U << 0);
}


static void delay_us(uint32_t us)
{
    while (us--)
    {
        /*
         * Wait for COUNTFLAG.
         */

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
    /*
     * PA6 = General Purpose Output
     */

    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
    GPIOA_MODER |=  (1U << (DS18B20_PIN * 2U));

    /*
     * Open-drain output
     */

    GPIOA_OTYPER |= (1U << DS18B20_PIN);
}


static void pin_input(void)
{
    /*
     * PA6 = Input
     */

    GPIOA_MODER &= ~(3U << (DS18B20_PIN * 2U));
}


static void one_wire_low(void)
{
    pin_output();

    GPIOA_ODR &= ~(1U << DS18B20_PIN);
}


static void one_wire_release(void)
{
    /*
     * Input mode releases the One-Wire bus.
     *
     * External 4.7 kOhm resistor pulls DATA HIGH.
     */

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

    /*
     * Master pulls bus LOW.
     */

    one_wire_low();

    delay_us(480);

    /*
     * Release bus.
     */

    one_wire_release();

    delay_us(70);

    /*
     * DS18B20 pulls bus LOW if present.
     */

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
        /*
         * Write 1
         */

        delay_us(6);

        one_wire_release();

        delay_us(64);
    }
    else
    {
        /*
         * Write 0
         */

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

    for (i = 0; i < 8U; i++)
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

    for (i = 0; i < 8U; i++)
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
 *
 * Returns:
 *
 *   Raw temperature value
 *
 *   Example:
 *   25.0 C = 400 decimal
 *
 *   -32768 = sensor not detected
 * ============================================================ */

static int16_t ds18b20_read_temperature_raw(void)
{
    uint8_t temp_lsb;
    uint8_t temp_msb;

    int16_t raw_temp;

    /*
     * Check sensor presence.
     */

    if (!ds18b20_reset())
    {
        return -32768;
    }

    /*
     * Skip ROM.
     *
     * Only one DS18B20 is connected.
     */

    ds18b20_write_byte(0xCC);

    /*
     * Start temperature conversion.
     */

    ds18b20_write_byte(0x44);

    /*
     * Maximum conversion time
     * at 12-bit resolution.
     */

    delay_us(750000U);

    /*
     * Reset before reading scratchpad.
     */

    if (!ds18b20_reset())
    {
        return -32768;
    }

    /*
     * Skip ROM.
     */

    ds18b20_write_byte(0xCC);

    /*
     * Read Scratchpad.
     */

    ds18b20_write_byte(0xBE);

    /*
     * Temperature LSB.
     */

    temp_lsb = ds18b20_read_byte();

    /*
     * Temperature MSB.
     */

    temp_msb = ds18b20_read_byte();

    raw_temp =
        (int16_t)(((uint16_t)temp_msb << 8) | temp_lsb);

    return raw_temp;
}


/* ============================================================
 * USART2 INITIALIZATION
 *
 * PA2 = USART2_TX
 *
 * PCLK1 = 16 MHz
 *
 * Baud = 115200
 * ============================================================ */

static void uart2_init(void)
{
    /*
     * Enable GPIOA clock.
     */

    RCC_AHB1ENR |= (1U << 0);

    /*
     * Enable USART2 clock.
     */

    RCC_APB1ENR |= (1U << 17);

    /*
     * PA2 = Alternate Function.
     */

    GPIOA_MODER &= ~(3U << (2U * 2U));

    GPIOA_MODER |= (2U << (2U * 2U));

    /*
     * PA2 = AF7 = USART2_TX.
     */

    GPIOA_AFRL &= ~(0xFU << 8);

    GPIOA_AFRL |= (7U << 8);

    /*
     * No pull-up / pull-down.
     */

    GPIOA_PUPDR &= ~(3U << (2U * 2U));

    /*
     * 16 MHz / 115200 ≈ 138.89
     *
     * BRR = 139
     */

    USART2_BRR = 139U;

    /*
     * 8 data bits
     * 1 stop bit
     * No parity
     */

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;

    /*
     * TE = transmitter enable.
     */

    USART2_CR1 |= (1U << 3);

    /*
     * UE = USART enable.
     */

    USART2_CR1 |= (1U << 13);
}


/* ============================================================
 * UART SEND CHARACTER
 * ============================================================ */

static void uart2_send_char(char c)
{
    /*
     * TXE = SR bit 7.
     */

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
 * UART SEND UNSIGNED INTEGER
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
 *
 * Raw / 16 = Celsius
 * ============================================================ */

static void uart2_send_temperature(int16_t raw_temp)
{
    int32_t temp_x10;

    /*
     * Convert to Celsius x10.
     *
     * Example:
     *
     * raw = 400
     *
     * 400 * 10 / 16 = 250
     *
     * 250 = 25.0 C
     */

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
 * MAIN
 * ============================================================ */

int main(void)
{
    int16_t temperature_raw;

    /*
     * Enable GPIOA clock.
     */

    RCC_AHB1ENR |= (1U << 0);

    /*
     * Enable HSI 16 MHz.
     */

    RCC_CR |= (1U << 0);

    /*
     * Select HSI as system clock.
     *
     * SW = 00
     */

    RCC_CFGR &= ~(3U << 0);

    /*
     * Initialize 1 us delay.
     */

    delay_init();

    /*
     * Initialize USART2.
     */

    uart2_init();

    /*
     * Release DS18B20 bus.
     */

    one_wire_release();

    /*
     * Startup message.
     */

    uart2_send_string("\r\n");
    uart2_send_string(
        "================================\r\n"
    );

    uart2_send_string(
        "STM32F446RE DS18B20 MONITOR\r\n"
    );

    uart2_send_string(
        "UART2: 115200 8N1\r\n"
    );

    uart2_send_string(
        "DS18B20: PA6\r\n"
    );

    uart2_send_string(
        "================================\r\n"
    );

    /*
     * Main loop.
     */

    while (1)
    {
        /*
         * Read temperature.
         */

        temperature_raw =
            ds18b20_read_temperature_raw();

        /*
         * Sensor not detected.
         */

        if (temperature_raw == -32768)
        {
            uart2_send_string(
                "DS18B20 not detected!\r\n"
            );
        }

        /*
         * Sensor detected.
         */

        else
        {
            uart2_send_string(
                "Temperature: "
            );

            uart2_send_temperature(
                temperature_raw
            );
        }

        /*
         * Wait approximately 1 second.
         */

        delay_us(1000000U);
    }
}
