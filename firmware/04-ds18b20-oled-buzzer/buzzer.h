#ifndef BUZZER_H
#define BUZZER_H
#include <stdint.h>
#define BUZZER_THRESHOLD_RAW (27 * 16)
void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
#endif
