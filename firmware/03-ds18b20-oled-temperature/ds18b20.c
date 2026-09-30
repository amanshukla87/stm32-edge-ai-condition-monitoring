#include "ds18b20.h"
#include "delay.h"
#include "onewire.h"
#define SKIP_ROM 0xCCU
#define CONVERT_T 0x44U
#define READ_SCRATCHPAD 0xBEU
void ds18b20_init(void){onewire_init();}
int16_t ds18b20_read_temperature_raw(void){uint8_t l,m;if(!onewire_reset())return DS18B20_ERROR;onewire_write_byte(SKIP_ROM);onewire_write_byte(CONVERT_T);delay_us(750000);if(!onewire_reset())return DS18B20_ERROR;onewire_write_byte(SKIP_ROM);onewire_write_byte(READ_SCRATCHPAD);l=onewire_read_byte();m=onewire_read_byte();return(int16_t)(((uint16_t)m<<8)|l);}
