#include "oled.h"
#include "delay.h"
#include "i2c.h"
#define OLED_ADDRESS 0x3CU
static uint8_t font(char c,uint8_t*f){uint8_t i;for(i=0;i<5;i++)f[i]=0;
switch(c){
case '0':f[0]=0x3E;f[1]=0x51;f[2]=0x49;f[3]=0x45;f[4]=0x3E;break;case '1':f[1]=0x42;f[2]=0x7F;f[3]=0x40;break;
case '2':f[0]=0x42;f[1]=0x61;f[2]=0x51;f[3]=0x49;f[4]=0x46;break;case '3':f[0]=0x21;f[1]=0x41;f[2]=0x45;f[3]=0x4B;f[4]=0x31;break;
case '4':f[0]=0x18;f[1]=0x14;f[2]=0x12;f[3]=0x7F;f[4]=0x10;break;case '5':f[0]=0x27;f[1]=0x45;f[2]=0x45;f[3]=0x45;f[4]=0x39;break;
case '6':f[0]=0x3C;f[1]=0x4A;f[2]=0x49;f[3]=0x49;f[4]=0x30;break;case '7':f[0]=1;f[1]=0x71;f[2]=9;f[3]=5;f[4]=3;break;
case '8':f[0]=0x36;f[1]=0x49;f[2]=0x49;f[3]=0x49;f[4]=0x36;break;case '9':f[0]=6;f[1]=0x49;f[2]=0x49;f[3]=0x29;f[4]=0x1E;break;
case 'A':f[0]=0x7E;f[1]=9;f[2]=9;f[3]=9;f[4]=0x7E;break;case 'C':f[0]=0x3E;f[1]=0x41;f[2]=0x41;f[3]=0x41;f[4]=0x22;break;
case 'E':f[0]=0x7F;f[1]=0x49;f[2]=0x49;f[3]=0x49;f[4]=0x41;break;case 'G':f[0]=0x3E;f[1]=0x41;f[2]=0x49;f[3]=0x49;f[4]=0x7A;break;
case 'M':f[0]=0x7F;f[1]=2;f[2]=0x0C;f[3]=2;f[4]=0x7F;break;case 'P':f[0]=0x7F;f[1]=9;f[2]=9;f[3]=9;f[4]=6;break;
case 'T':f[0]=1;f[1]=1;f[2]=0x7F;f[3]=1;f[4]=1;break;case 'U':f[0]=0x3F;f[1]=0x40;f[2]=0x40;f[3]=0x40;f[4]=0x3F;break;
case 'X':f[0]=0x63;f[1]=0x14;f[2]=8;f[3]=0x14;f[4]=0x63;break;case 'Y':f[0]=7;f[1]=8;f[2]=0x70;f[3]=8;f[4]=7;break;
case 'Z':f[0]=0x61;f[1]=0x51;f[2]=0x49;f[3]=0x45;f[4]=0x43;break;case 'I':f[0]=0x41;f[1]=0x41;f[2]=0x7F;f[3]=0x41;f[4]=0x41;break;
case 'R':f[0]=0x7F;f[1]=9;f[2]=0x19;f[3]=0x29;f[4]=0x46;break;case ' ':break;case '-':for(i=0;i<5;i++)f[i]=8;break;case '.':f[1]=0x60;f[2]=0x60;break;default:return 0;}return 1;}
static uint8_t cmd(uint8_t v){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(0)||!i2c1_write(v)){i2c1_stop();return 0;}i2c1_stop();return 1;}
static void pos(uint8_t p,uint8_t c){cmd(0xB0|p);cmd(c&0x0F);cmd(0x10|((c>>4)&0x0F));}
static uint8_t data_start(void){if(!i2c1_start())return 0;if(!i2c1_address(OLED_ADDRESS<<1)||!i2c1_write(0x40)){i2c1_stop();return 0;}return 1;}
static void print(const char*s){uint8_t f[5],i;if(!data_start())return;while(*s){if(font(*s,f)){for(i=0;i<5;i++)i2c1_write(f[i]);i2c1_write(0);}s++;}i2c1_stop();}
static void clear_page(uint8_t p){uint16_t i;pos(p,0);if(!data_start())return;for(i=0;i<128;i++)if(!i2c1_write(0))break;i2c1_stop();}
void oled_init(void){delay_us(100000);cmd(0xAE);cmd(0xD5);cmd(0x80);cmd(0xA8);cmd(0x3F);cmd(0xD3);cmd(0);cmd(0x40);cmd(0x8D);cmd(0x14);cmd(0x20);cmd(0);cmd(0xA1);cmd(0xC8);cmd(0xDA);cmd(0x12);cmd(0x81);cmd(0x7F);cmd(0xD9);cmd(0xF1);cmd(0xDB);cmd(0x40);cmd(0xA4);cmd(0xA6);cmd(0xAF);delay_us(100000);}
void oled_clear(void){uint8_t p;for(p=0;p<8;p++)clear_page(p);}
static void signed_int(int16_t v){char t[8];uint8_t n=0;int32_t x=v;if(x<0){t[n++]='-';x=-x;}if(x>=10000)t[n++]=(char)('0'+(x/10000)%10);if(x>=1000)t[n++]=(char)('0'+(x/1000)%10);if(x>=100)t[n++]=(char)('0'+(x/100)%10);if(x>=10)t[n++]=(char)('0'+(x/10)%10);t[n++]=(char)('0'+x%10);t[n]=0;print(t);}
void oled_show_temperature(int16_t raw){int32_t x=((int32_t)raw*10)/16;char t[12];uint8_t n=0;oled_clear();pos(1,52);print("TEMP");if(x<0){t[n++]='-';x=-x;}if(x>=1000)t[n++]=(char)('0'+(x/1000)%10);if(x>=100)t[n++]=(char)('0'+(x/100)%10);t[n++]=(char)('0'+(x/10)%10);t[n++]='.';t[n++]=(char)('0'+x%10);t[n++]=' ';t[n++]='C';t[n]=0;pos(3,43);print(t);}
void oled_show_mpu6050(const MPU6050_Data*d){oled_clear();pos(0,43);print("MPU");pos(1,0);print("X ");signed_int(d->accel_x);pos(2,0);print("Y ");signed_int(d->accel_y);pos(3,0);print("Z ");signed_int(d->accel_z);pos(4,0);print("T ");signed_int(d->temperature_raw);pos(5,0);print("G X ");signed_int(d->gyro_x);pos(6,0);print("G Y ");signed_int(d->gyro_y);pos(7,0);print("G Z ");signed_int(d->gyro_z);}
void oled_show_error(void){oled_clear();pos(1,52);print("TEMP");pos(3,49);print("ERR");}
