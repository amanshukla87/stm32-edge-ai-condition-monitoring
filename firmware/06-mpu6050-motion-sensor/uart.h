#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "mpu6050.h"
void uart2_init(void);
void uart2_send_char(char c);
void uart2_send_string(const char *s);
void uart2_send_int(int32_t value);
void uart2_send_temperature(int16_t raw);
void uart2_send_mpu6050(const MPU6050_Data *data);
#endif
