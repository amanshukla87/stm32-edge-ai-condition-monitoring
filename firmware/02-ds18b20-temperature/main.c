#include <stdint.h>
#include "delay.h"
#include "ds18b20.h"
#include "uart.h"

int main(void)
{
    int16_t temperature_raw;

    delay_init();
    uart2_init();
    ds18b20_init();

    uart2_send_string("\r\n");
    uart2_send_string("================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 MONITOR\r\n");
    uart2_send_string("UART2: 115200 8N1\r\n");
    uart2_send_string("DS18B20: PA6\r\n");
    uart2_send_string("================================\r\n");

    while (1)
    {
        temperature_raw = ds18b20_read_temperature_raw();

        if (temperature_raw == DS18B20_ERROR)
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
