#include <stdint.h>
#include "delay.h"
#include "ds18b20.h"
#include "i2c.h"
#include "ina219.h"
#include "oled.h"
#include "uart.h"
#include "buzzer.h"

#define RCC_CR   (*(volatile uint32_t *)(0x40023800U + 0x00U))
#define RCC_CFGR (*(volatile uint32_t *)(0x40023800U + 0x08U))

int main(void)
{
    int16_t temperature_raw;
    INA219_Measurements measurements;
    uint8_t ina219_ok;

    /* Use the STM32F446RE HSI as the 16 MHz system clock. */
    RCC_CR |= (1U << 0);
    RCC_CFGR &= ~(3U << 0);

    delay_init();
    uart2_init();
    buzzer_init();
    ds18b20_init();
    i2c1_init();
    oled_init();
    oled_clear();

    uart2_send_string("\r\n================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 + INA219\r\n");
    uart2_send_string("UART2: 115200 8N1\r\n");
    uart2_send_string("DS18B20: PA6\r\n");
    uart2_send_string("I2C1: PB8/PB9\r\n");
    uart2_send_string("INA219: 0x40\r\n");
    uart2_send_string("================================\r\n");

    ina219_ok = ina219_init();

    if (ina219_ok)
        uart2_send_string("INA219 initialization: OK\r\n");
    else
        uart2_send_string("INA219 initialization: FAILED\r\n");

    uart2_send_string("System started.\r\n\r\n");

    while (1)
    {
        temperature_raw = ds18b20_read_temperature_raw();

        if (temperature_raw == DS18B20_ERROR)
        {
            uart2_send_string("DS18B20 not detected!\r\n");
            oled_show_error();
            buzzer_off();
        }
        else
        {
            uart2_send_string("Temperature: ");
            uart2_send_temperature(temperature_raw);
            oled_show_temperature(temperature_raw);

            if (temperature_raw >= (30 * 16))
                buzzer_on();
            else
                buzzer_off();
        }

        if (ina219_ok)
        {
            if (ina219_read_measurements(&measurements))
            {
                uart2_send_ina219(
                    measurements.bus_voltage_mV,
                    measurements.current_mA,
                    measurements.power_mW
                );
                oled_show_ina219(&measurements);
            }
            else
            {
                uart2_send_string("INA219 read error!\r\n");
            }
        }

        delay_us(1000000U);
    }
}
