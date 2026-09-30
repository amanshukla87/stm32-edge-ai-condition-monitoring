#ifndef I2C_H
#define I2C_H
#include <stdint.h>
void i2c1_init(void);
uint8_t i2c1_start(void);
void i2c1_stop(void);
uint8_t i2c1_address(uint8_t address);
uint8_t i2c1_write(uint8_t data);
#endif
