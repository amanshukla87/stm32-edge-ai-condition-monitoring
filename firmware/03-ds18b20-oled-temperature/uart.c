#include "uart.h"
#define AHB1ENR (*(volatile uint32_t *)(0x40023800U+0x30U))
#define APB1ENR (*(volatile uint32_t *)(0x40023800U+0x40U))
#define MODER (*(volatile uint32_t *)(0x40020000U+0x00U))
#define PUPDR (*(volatile uint32_t *)(0x40020000U+0x0CU))
#define AFRL (*(volatile uint32_t *)(0x40020000U+0x20U))
#define SR (*(volatile uint32_t *)(0x40004400U+0x00U))
#define DR (*(volatile uint32_t *)(0x40004400U+0x04U))
#define BRR (*(volatile uint32_t *)(0x40004400U+0x08U))
#define CR1 (*(volatile uint32_t *)(0x40004400U+0x0CU))
#define CR2 (*(volatile uint32_t *)(0x40004400U+0x10U))
#define CR3 (*(volatile uint32_t *)(0x40004400U+0x14U))
void uart2_init(void){AHB1ENR|=1U;APB1ENR|=1U<<17;MODER&=~(3U<<4);MODER|=2U<<4;AFRL&=~(0xFU<<8);AFRL|=7U<<8;PUPDR&=~(3U<<4);BRR=139U;CR1=0;CR2=0;CR3=0;CR1|=(1U<<3)|(1U<<13);}
void uart2_send_char(char c){while(!(SR&(1U<<7))){}DR=(uint8_t)c;}
void uart2_send_string(const char *s){while(*s)uart2_send_char(*s++);}
void uart2_send_temperature(int16_t raw){int32_t x=((int32_t)raw*10)/16;uint32_t n;uint8_t f;if(x<0){uart2_send_char('-');x=-x;}n=(uint32_t)(x/10);f=(uint8_t)(x%10);if(!n)uart2_send_char('0');else{char b[10];uint8_t i=0;while(n){b[i++]=(char)('0'+n%10);n/=10;}while(i)uart2_send_char(b[--i]);}uart2_send_char('.');uart2_send_char((char)('0'+f));uart2_send_string(" C\r\n");}
