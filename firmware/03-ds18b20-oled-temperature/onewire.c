#include "onewire.h"
#include "delay.h"
#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800U+0x30U))
#define MODER (*(volatile uint32_t *)(0x40020000U+0x00U))
#define OTYPER (*(volatile uint32_t *)(0x40020000U+0x04U))
#define IDR (*(volatile uint32_t *)(0x40020000U+0x10U))
#define ODR (*(volatile uint32_t *)(0x40020000U+0x14U))
#define MASK (1U<<6)
static void out(void){MODER&=~(3U<<12);MODER|=(1U<<12);OTYPER|=MASK;}
static void in(void){MODER&=~(3U<<12);}
static void low(void){out();ODR&=~MASK;}
static void rel(void){in();}
static uint8_t pin(void){return(IDR&MASK)?1U:0U;}
void onewire_init(void){RCC_AHB1ENR|=1U;rel();}
uint8_t onewire_reset(void){uint8_t p;low();delay_us(480);rel();delay_us(70);p=(pin()==0U);delay_us(410);return p;}
static void wb(uint8_t b){low();if(b){delay_us(6);rel();delay_us(64);}else{delay_us(60);rel();delay_us(10);}}
static uint8_t rb(void){uint8_t b;low();delay_us(2);rel();delay_us(10);b=pin();delay_us(50);return b;}
void onewire_write_byte(uint8_t d){for(uint8_t i=0;i<8;i++){wb(d&1U);d>>=1;}}
uint8_t onewire_read_byte(void){uint8_t d=0;for(uint8_t i=0;i<8;i++)if(rb())d|=1U<<i;return d;}
