#include "i2c.h"
#include "delay.h"
#define AHB1ENR (*(volatile uint32_t *)(0x40023800U+0x30U))
#define APB1ENR (*(volatile uint32_t *)(0x40023800U+0x40U))
#define MODER (*(volatile uint32_t *)(0x40020400U+0x00U))
#define OTYPER (*(volatile uint32_t *)(0x40020400U+0x04U))
#define OSPEEDR (*(volatile uint32_t *)(0x40020400U+0x08U))
#define PUPDR (*(volatile uint32_t *)(0x40020400U+0x0CU))
#define AFRH (*(volatile uint32_t *)(0x40020400U+0x24U))
#define CR1 (*(volatile uint32_t *)(0x40005400U+0x00U))
#define CR2 (*(volatile uint32_t *)(0x40005400U+0x04U))
#define OAR1 (*(volatile uint32_t *)(0x40005400U+0x08U))
#define DR (*(volatile uint32_t *)(0x40005400U+0x10U))
#define SR1 (*(volatile uint32_t *)(0x40005400U+0x14U))
#define SR2 (*(volatile uint32_t *)(0x40005400U+0x18U))
#define CCR (*(volatile uint32_t *)(0x40005400U+0x1CU))
#define TRISE (*(volatile uint32_t *)(0x40005400U+0x20U))
void i2c1_init(void){AHB1ENR|=1U<<1;APB1ENR|=1U<<21;MODER&=~((3U<<16)|(3U<<18));MODER|=(2U<<16)|(2U<<18);OTYPER|=(1U<<8)|(1U<<9);OSPEEDR|=(3U<<16)|(3U<<18);PUPDR&=~((3U<<16)|(3U<<18));AFRH&=~((0xFU<<0)|(0xFU<<4));AFRH|=(4U<<0)|(4U<<4);CR1=0;CR1|=1U<<15;delay_us(10);CR1&=~(1U<<15);CR2=16;OAR1=1U<<14;CCR=80;TRISE=17;CR1|=1U;}
uint8_t i2c1_start(void){uint32_t t=100000;while(SR2&(1U<<1))if(--t==0)return 0;CR1&=~(1U<<9);CR1|=1U<<8;t=100000;while(!(SR1&1U))if(--t==0)return 0;return 1;}
void i2c1_stop(void){CR1|=1U<<9;}
uint8_t i2c1_address(uint8_t a){uint32_t t=100000;DR=a;while(!(SR1&((1U<<1)|(1U<<10))))if(--t==0)return 0;if(SR1&(1U<<10)){SR1&=~(1U<<10);return 0;}(void)SR1;(void)SR2;return 1;}
uint8_t i2c1_write(uint8_t d){uint32_t t=100000;DR=d;while(!(SR1&(1U<<7)))if(--t==0)return 0;t=100000;while(!(SR1&(1U<<2)))if(--t==0)return 0;return 1;}
