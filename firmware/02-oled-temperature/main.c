#include <stdint.h>

#define RCC_BASE 0x40023800U
#define RCC_CR (*(volatile uint32_t *)(RCC_BASE+0x00U))
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE+0x08U))
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE+0x30U))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE+0x40U))
#define GPIOA_BASE 0x40020000U
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+0x00U))
#define GPIOA_OTYPER (*(volatile uint32_t *)(GPIOA_BASE+0x04U))
#define GPIOA_PUPDR (*(volatile uint32_t *)(GPIOA_BASE+0x0CU))
#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE+0x10U))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE+0x14U))
#define GPIOA_AFRL (*(volatile uint32_t *)(GPIOA_BASE+0x20U))
#define GPIOB_BASE 0x40020400U
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE+0x00U))
#define GPIOB_OTYPER (*(volatile uint32_t *)(GPIOB_BASE+0x04U))
#define GPIOB_OSPEEDR (*(volatile uint32_t *)(GPIOB_BASE+0x08U))
#define GPIOB_PUPDR (*(volatile uint32_t *)(GPIOB_BASE+0x0CU))
#define GPIOB_AFRH (*(volatile uint32_t *)(GPIOB_BASE+0x24U))
#define USART2_BASE 0x40004400U
#define USART2_SR (*(volatile uint32_t *)(USART2_BASE+0x00U))
#define USART2_DR (*(volatile uint32_t *)(USART2_BASE+0x04U))
#define USART2_BRR (*(volatile uint32_t *)(USART2_BASE+0x08U))
#define USART2_CR1 (*(volatile uint32_t *)(USART2_BASE+0x0CU))
#define USART2_CR2 (*(volatile uint32_t *)(USART2_BASE+0x10U))
#define USART2_CR3 (*(volatile uint32_t *)(USART2_BASE+0x14U))
#define I2C1_BASE 0x40005400U
#define I2C1_CR1 (*(volatile uint32_t *)(I2C1_BASE+0x00U))
#define I2C1_CR2 (*(volatile uint32_t *)(I2C1_BASE+0x04U))
#define I2C1_OAR1 (*(volatile uint32_t *)(I2C1_BASE+0x08U))
#define I2C1_DR (*(volatile uint32_t *)(I2C1_BASE+0x10U))
#define I2C1_SR1 (*(volatile uint32_t *)(I2C1_BASE+0x14U))
#define I2C1_SR2 (*(volatile uint32_t *)(I2C1_BASE+0x18U))
#define I2C1_CCR (*(volatile uint32_t *)(I2C1_BASE+0x1CU))
#define I2C1_TRISE (*(volatile uint32_t *)(I2C1_BASE+0x20U))
#define SYSTICK_BASE 0xE000E010U
#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE+0x00U))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE+0x04U))
#define SYSTICK_VAL (*(volatile uint32_t *)(SYSTICK_BASE+0x08U))
#define DS18B20_PIN 6U
#define OLED_ADDRESS 0x3CU

static void delay_init(void){SYSTICK_LOAD=15U;SYSTICK_VAL=0U;SYSTICK_CTRL=(1U<<2)|(1U<<0);}
static void delay_us(uint32_t us){while(us--){while(!(SYSTICK_CTRL&(1U<<16))){} }}
static void pin_output(void){GPIOA_MODER&=~(3U<<(DS18B20_PIN*2U));GPIOA_MODER|=(1U<<(DS18B20_PIN*2U));GPIOA_OTYPER|=(1U<<DS18B20_PIN);}
static void pin_input(void){GPIOA_MODER&=~(3U<<(DS18B20_PIN*2U));}
static void one_wire_low(void){pin_output();GPIOA_ODR&=~(1U<<DS18B20_PIN);}
static void one_wire_release(void){pin_input();}
static uint8_t one_wire_read_pin(void){return(GPIOA_IDR&(1U<<DS18B20_PIN))?1U:0U;}
static uint8_t ds18b20_reset(void){uint8_t p;one_wire_low();delay_us(480U);one_wire_release();delay_us(70U);p=(one_wire_read_pin()==0U);delay_us(410U);return p;}
static void ds18b20_write_bit(uint8_t b){one_wire_low();if(b){delay_us(6U);one_wire_release();delay_us(64U);}else{delay_us(60U);one_wire_release();delay_us(10U);}}
static uint8_t ds18b20_read_bit(void){uint8_t b;one_wire_low();delay_us(2U);one_wire_release();delay_us(10U);b=one_wire_read_pin();delay_us(50U);return b;}
static void ds18b20_write_byte(uint8_t d){uint8_t i;for(i=0;i<8;i++){ds18b20_write_bit(d&1U);d>>=1;}}
static uint8_t ds18b20_read_byte(void){uint8_t d=0,i;for(i=0;i<8;i++)if(ds18b20_read_bit())d|=(1U<<i);return d;}
static int16_t ds18b20_read_temperature_raw(void){uint8_t l,m;if(!ds18b20_reset())return-32768;ds18b20_write_byte(0xCC);ds18b20_write_byte(0x44);delay_us(750000U);if(!ds18b20_reset())return-32768;ds18b20_write_byte(0xCC);ds18b20_write_byte(0xBE);l=ds18b20_read_byte();m=ds18b20_read_byte();return(int16_t)(((uint16_t)m<<8)|l);}

static void uart2_init(void){RCC_AHB1ENR|=(1U<<0);RCC_APB1ENR|=(1U<<17);GPIOA_MODER&=~(3U<<4);GPIOA_MODER|=(2U<<4);GPIOA_AFRL&=~(0xFU<<8);GPIOA_AFRL|=(7U<<8);GPIOA_PUPDR&=~(3U<<4);USART2_BRR=139U;USART2_CR1=0;USART2_CR2=0;USART2_CR3=0;USART2_CR1|=(1U<<3)|(1U<<13);}
static void uart2_send_char(char c){while(!(USART2_SR&(1U<<7))){}USART2_DR=(uint8_t)c;}
static void uart2_send_string(const char*s){while(*s)uart2_send_char(*s++);}
static void uart2_send_uint(uint32_t v){char b[10];uint8_t i=0;if(!v){uart2_send_char('0');return;}while(v){b[i++]=(char)('0'+v%10U);v/=10U;}while(i)uart2_send_char(b[--i]);}
static void uart2_send_temperature(int16_t r){int32_t t=((int32_t)r*10)/16;if(t<0){uart2_send_char('-');t=-t;}uart2_send_uint((uint32_t)(t/10));uart2_send_char('.');uart2_send_char((char)('0'+t%10));uart2_send_string(" C\r\n");}

static void i2c1_init(void){RCC_AHB1ENR|=(1U<<1);RCC_APB1ENR|=(1U<<21);GPIOB_MODER&=~((3U<<16)|(3U<<18));GPIOB_MODER|=((2U<<16)|(2U<<18));GPIOB_OTYPER|=(1U<<8)|(1U<<9);GPIOB_OSPEEDR|=(3U<<16)|(3U<<18);GPIOB_PUPDR&=~((3U<<16)|(3U<<18));GPIOB_AFRH&=~((0xFU<<0)|(0xFU<<4));GPIOB_AFRH|=(4U<<0)|(4U<<4);I2C1_CR1=0;I2C1_CR1|=(1U<<15);delay_us(10);I2C1_CR1&=~(1U<<15);I2C1_CR2=16U;I2C1_OAR1=(1U<<14);I2C1_CCR=80U;I2C1_TRISE=17U;I2C1_CR1|=1U;}
static uint8_t i2c1_start(void){uint32_t t=100000U;while(I2C1_SR2&(1U<<1))if(--t==0)return 0;I2C1_CR1&=~(1U<<9);I2C1_CR1|=(1U<<8);t=100000U;while(!(I2C1_SR1&1U))if(--t==0)return 0;return 1;}
static void i2c1_stop(void){I2C1_CR1|=(1U<<9);}
static uint8_t i2c1_address(uint8_t a){uint32_t t=100000U;I2C1_DR=a;while(!(I2C1_SR1&((1U<<1)|(1U<<10))))if(--t==0)return 0;if(I2C1_SR1&(1U<<10)){I2C1_SR1&=~(1U<<10);return 0;}(void)I2C1_SR1;(void)I2C1_SR2;return 1;}
static uint8_t i2c1_write(uint8_t d){uint32_t t=100000U;I2C1_DR=d;while(!(I2C1_SR1&(1U<<7)))if(--t==0)return 0;t=100000U;while(!(I2C1_SR1&(1U<<2)))if(--t==0)return 0;return 1;}
static uint8_t oled_command(uint8_t c){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(0)||!i2c1_write(c)){i2c1_stop();return 0;}i2c1_stop();return 1;}
static void oled_init(void){delay_us(100000);oled_command(0xAE);oled_command(0xD5);oled_command(0x80);oled_command(0xA8);oled_command(0x3F);oled_command(0xD3);oled_command(0);oled_command(0x40);oled_command(0x8D);oled_command(0x14);oled_command(0x20);oled_command(0);oled_command(0xA1);oled_command(0xC8);oled_command(0xDA);oled_command(0x12);oled_command(0x81);oled_command(0x7F);oled_command(0xD9);oled_command(0xF1);oled_command(0xDB);oled_command(0x40);oled_command(0xA4);oled_command(0xA6);oled_command(0xAF);delay_us(100000);}
static void oled_set_position(uint8_t p,uint8_t c){oled_command(0xB0U|p);oled_command(c&0x0F);oled_command(0x10U|(c>>4));}
static uint8_t oled_data_start(void){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(0x40)){i2c1_stop();return 0;}return 1;}
static const uint8_t font[10][5]={{0x3E,0x51,0x49,0x45,0x3E},{0,0x42,0x7F,0x40,0},{0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4B,0x31},{0x18,0x14,0x12,0x7F,0x10},{0x27,0x45,0x45,0x45,0x39},{0x3C,0x4A,0x49,0x49,0x30},{1,0x71,9,5,3},{0x36,0x49,0x49,0x49,0x36},{6,0x49,0x49,0x29,0x1E}};
static uint8_t get_font(char c,uint8_t*f){uint8_t i;if(c>='0'&&c<='9'){for(i=0;i<5;i++)f[i]=font[c-'0'][i];return 1;}if(c=='T'){f[0]=1;f[1]=1;f[2]=0x7F;f[3]=1;f[4]=1;return 1;}if(c=='E'){f[0]=0x7F;f[1]=0x49;f[2]=0x49;f[3]=0x49;f[4]=0x41;return 1;}if(c=='M'){f[0]=0x7F;f[1]=2;f[2]=0x0C;f[3]=2;f[4]=0x7F;return 1;}if(c=='P'){f[0]=0x7F;f[1]=9;f[2]=9;f[3]=9;f[4]=6;return 1;}if(c=='C'){f[0]=0x3E;f[1]=0x41;f[2]=0x41;f[3]=0x41;f[4]=0x22;return 1;}if(c==' '){for(i=0;i<5;i++)f[i]=0;return 1;}if(c=='.'){f[0]=0;f[1]=0x60;f[2]=0x60;f[3]=0;f[4]=0;return 1;}if(c=='-'){for(i=0;i<5;i++)f[i]=8;return 1;}return 0;}
static void oled_print_string(const char*s){uint8_t f[5],i;if(!oled_data_start())return;while(*s){if(get_font(*s,f)){for(i=0;i<5;i++)i2c1_write(f[i]);i2c1_write(0);}s++;}i2c1_stop();}
static void oled_clear_page(uint8_t p){uint16_t i;oled_set_position(p,0);if(!oled_data_start())return;for(i=0;i<128;i++)if(!i2c1_write(0))break;i2c1_stop();}
static void oled_clear(void){uint8_t p;for(p=0;p<8;p++)oled_clear_page(p);}
static void oled_show_temperature(int16_t r){int32_t t=((int32_t)r*10)/16;char s[12];uint8_t n=0;if(t<0){s[n++]='-';t=-t;}if(t>=1000)s[n++]=(char)('0'+(t/1000)%10);if(t>=100)s[n++]=(char)('0'+(t/100)%10);s[n++]=(char)('0'+(t/10)%10);s[n++]='.';s[n++]=(char)('0'+t%10);s[n++]=' ';s[n++]='C';s[n]=0;oled_clear_page(3);oled_set_position(1,52);oled_print_string("TEMP");oled_set_position(3,43);oled_print_string(s);}
static void oled_show_error(void){oled_clear_page(3);oled_set_position(1,52);oled_print_string("TEMP");oled_set_position(3,49);oled_print_string("ERR");}

int main(void){int16_t temperature_raw;RCC_AHB1ENR|=1U;RCC_CR|=1U;RCC_CFGR&=~3U;delay_init();uart2_init();one_wire_release();uart2_send_string("\r\n================================\r\nSTM32F446RE DS18B20 + OLED\r\nUART2: 115200 8N1\r\nDS18B20: PA6\r\nOLED: I2C1 PB8/PB9\r\n================================\r\n");i2c1_init();oled_init();oled_clear();while(1){temperature_raw=ds18b20_read_temperature_raw();if(temperature_raw==-32768){uart2_send_string("DS18B20 not detected!\r\n");oled_show_error();}else{uart2_send_string("Temperature: ");uart2_send_temperature(temperature_raw);oled_show_temperature(temperature_raw);}delay_us(1000000U);}}
