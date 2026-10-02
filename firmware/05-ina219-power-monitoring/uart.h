#ifndef UART_H
#define UART_H
#include <stdint.h>
void uart2_init(void);
void uart2_send_char(char c);
void uart2_send_string(const char *s);
void uart2_send_temperature(int16_t raw);
void uart2_send_ina219(uint32_t bus_mV, int32_t current_mA, uint32_t power_mW);
#endif
