#include "buzzer.h"
#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U+0x30U))
#define GPIOB_MODER (*(volatile uint32_t *)(0x40020400U+0x00U))
#define GPIOB_OTYPER (*(volatile uint32_t *)(0x40020400U+0x04U))
#define GPIOB_ODR (*(volatile uint32_t *)(0x40020400U+0x14U))
void buzzer_init(void){RCC_AHB1ENR|=1U<<1; GPIOB_MODER&=~3U; GPIOB_MODER|=1U; GPIOB_OTYPER&=~1U; GPIOB_ODR&=~1U;}
void buzzer_on(void){GPIOB_ODR|=1U;}
void buzzer_off(void){GPIOB_ODR&=~1U;}
