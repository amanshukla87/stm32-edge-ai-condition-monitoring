#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>

void onewire_init(void);
uint8_t onewire_reset(void);
void onewire_write_bit(uint8_t bit);
uint8_t onewire_read_bit(void);
void onewire_write_byte(uint8_t data);
uint8_t onewire_read_byte(void);

#endif
