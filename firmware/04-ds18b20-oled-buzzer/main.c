#include <stdint.h>
#include "delay.h"
#include "ds18b20.h"
#include "oled.h"
#include "uart.h"
#include "buzzer.h"

int main(void)
{
    int16_t temperature_raw;
    delay_init();
    uart2_init();
    ds18b20_init();
    oled_init();
    buzzer_init();
    uart2_send_string("\r\n================================\r\n");
    uart2_send_string("STM32F446RE DS18B20 + OLED + Buzzer\r\n");
    uart2_send_string("UART2: 115200 8N1\r\nDS18B20: PA6\r\nOLED: I2C1 PB8/PB9\r\nBuzzer: PB0\r\n");
    uart2_send_string("================================\r\n");
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
            if (temperature_raw > BUZZER_THRESHOLD_RAW) buzzer_on();
            else buzzer_off();
        }
        delay_us(1000000U);
    }
}
