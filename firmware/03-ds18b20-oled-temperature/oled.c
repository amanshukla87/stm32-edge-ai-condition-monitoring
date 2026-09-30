#include "oled.h"
#include "delay.h"
#include "i2c.h"

#define OLED_ADDRESS 0x3CU
static const uint8_t digits[10][5]={{0x3E,0x51,0x49,0x45,0x3E},{0x00,0x42,0x7F,0x40,0x00},{0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4B,0x31},{0x18,0x14,0x12,0x7F,0x10},{0x27,0x45,0x45,0x45,0x39},{0x3C,0x4A,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},{0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1E}};
static uint8_t font(char c,uint8_t *f){uint8_t i;if(c>='0'&&c<='9'){for(i=0;i<5;i++)f[i]=digits[c-'0'][i];return 1;}if(c=='T'){f[0]=1;f[1]=1;f[2]=0x7F;f[3]=1;f[4]=1;return 1;}if(c=='E'){f[0]=0x7F;f[1]=0x49;f[2]=0x49;f[3]=0x49;f[4]=0x41;return 1;}if(c=='M'){f[0]=0x7F;f[1]=2;f[2]=0x0C;f[3]=2;f[4]=0x7F;return 1;}if(c=='P'){f[0]=0x7F;f[1]=9;f[2]=9;f[3]=9;f[4]=6;return 1;}if(c=='C'){f[0]=0x3E;f[1]=0x41;f[2]=0x41;f[3]=0x41;f[4]=0x22;return 1;}if(c==' '){for(i=0;i<5;i++)f[i]=0;return 1;}if(c=='.'){f[0]=0;f[1]=0x60;f[2]=0x60;f[3]=0;f[4]=0;return 1;}if(c=='-'){for(i=0;i<5;i++)f[i]=8;return 1;}return 0;}
static uint8_t command(uint8_t c){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(0)){i2c1_stop();return 0;}if(!i2c1_write(c)){i2c1_stop();return 0;}i2c1_stop();return 1;}
static void position(uint8_t page,uint8_t col){command(0xB0U|page);command(col&0x0FU);command(0x10U|((col>>4)&0x0FU));}
static uint8_t data_start(void){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(0x40U)){i2c1_stop();return 0;}return 1;}
static void print_string(const char *s){uint8_t f[5],i;if(!data_start())return;while(*s){if(font(*s,f))for(i=0;i<5;i++)i2c1_write(f[i]);i2c1_write(0);s++;}i2c1_stop();}
static void clear_page(uint8_t page){uint16_t i;position(page,0);if(!data_start())return;for(i=0;i<128;i++)if(!i2c1_write(0))break;i2c1_stop();}
void oled_init(void){i2c1_init();delay_us(100000);command(0xAE);command(0xD5);command(0x80);command(0xA8);command(0x3F);command(0xD3);command(0);command(0x40);command(0x8D);command(0x14);command(0x20);command(0);command(0xA1);command(0xC8);command(0xDA);command(0x12);command(0x81);command(0x7F);command(0xD9);command(0xF1);command(0xDB);command(0x40);command(0xA4);command(0xA6);command(0xAF);delay_us(100000);}
void oled_clear(void){for(uint8_t p=0;p<8;p++)clear_page(p);}
void oled_show_temperature(int16_t raw){int32_t x=((int32_t)raw*10)/16;char text[12];uint8_t p=0;clear_page(3);position(1,52);print_string("TEMP");if(x<0){text[p++]='-';x=-x;}if(x>=1000)text[p++]=(char)('0'+(x/1000)%10);if(x>=100)text[p++]=(char)('0'+(x/100)%10);text[p++]=(char)('0'+(x/10)%10);text[p++]='.';text[p++]=(char)('0'+x%10);text[p++]=' ';text[p++]='C';text[p]=0;position(3,43);print_string(text);}
void oled_show_error(void){clear_page(3);position(1,52);print_string("TEMP");position(3,49);print_string("ERR");}
