#ifndef I2C_H
#define I2C_H
#include <stdint.h>

void i2c1_init(void);
uint8_t i2c1_start(void);
void i2c1_stop(void);
uint8_t i2c1_address(uint8_t address);
uint8_t i2c1_write(uint8_t data);
uint8_t i2c1_read_register_2bytes(uint8_t address, uint8_t reg,
                                  uint8_t *high, uint8_t *low);

#endif
