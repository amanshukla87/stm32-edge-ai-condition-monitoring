#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>

#define DS18B20_ERROR (-32768)

void ds18b20_init(void);
int16_t ds18b20_read_temperature_raw(void);

#endif
