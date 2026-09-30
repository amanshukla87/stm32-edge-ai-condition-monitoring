#include "ds18b20.h"
#include "delay.h"
#include "onewire.h"

#define DS18B20_SKIP_ROM       0xCCU
#define DS18B20_CONVERT_T      0x44U
#define DS18B20_READ_SCRATCH   0xBEU

void ds18b20_init(void)
{
    onewire_init();
}

int16_t ds18b20_read_temperature_raw(void)
{
    uint8_t temp_lsb;
    uint8_t temp_msb;

    if (!onewire_reset())
    {
        return DS18B20_ERROR;
    }

    onewire_write_byte(DS18B20_SKIP_ROM);
    onewire_write_byte(DS18B20_CONVERT_T);

    /* Maximum conversion time at 12-bit resolution. */
    delay_us(750000U);

    if (!onewire_reset())
    {
        return DS18B20_ERROR;
    }

    onewire_write_byte(DS18B20_SKIP_ROM);
    onewire_write_byte(DS18B20_READ_SCRATCH);

    temp_lsb = onewire_read_byte();
    temp_msb = onewire_read_byte();

    return (int16_t)(((uint16_t)temp_msb << 8) | temp_lsb);
}
