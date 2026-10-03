#ifndef OLED_H
#define OLED_H
#include <stdint.h>
#include "mpu6050.h"
void oled_init(void);
void oled_clear(void);
void oled_show_temperature(int16_t raw);
void oled_show_mpu6050(const MPU6050_Data *data);
void oled_show_error(void);
#endif
