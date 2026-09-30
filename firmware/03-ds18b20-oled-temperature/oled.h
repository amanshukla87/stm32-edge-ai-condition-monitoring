#ifndef OLED_H
#define OLED_H
#include <stdint.h>
void oled_init(void);
void oled_clear(void);
void oled_show_temperature(int16_t raw_temp);
void oled_show_error(void);
#endif
