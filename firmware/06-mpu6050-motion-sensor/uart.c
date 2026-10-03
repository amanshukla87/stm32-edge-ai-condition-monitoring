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
void uart2_send_string(const char*s){while(*s)uart2_send_char(*s++);}
static void send_uint(uint32_t v){char b[10];uint8_t i=0;if(!v){uart2_send_char('0');return;}while(v){b[i++]=(char)('0'+v%10U);v/=10U;}while(i)uart2_send_char(b[--i]);}
void uart2_send_int(int32_t v){if(v<0){uart2_send_char('-');v=-v;}send_uint((uint32_t)v);}
void uart2_send_temperature(int16_t raw){int32_t x=((int32_t)raw*10)/16;if(x<0){uart2_send_char('-');x=-x;}send_uint((uint32_t)(x/10));uart2_send_char('.');uart2_send_char((char)('0'+x%10));uart2_send_string(" C\r\n");}
void uart2_send_ina219(const INA219_Measurements*m){int32_t current=m->current_mA;uart2_send_string("INA219:\r\nBus Voltage: ");send_uint(m->bus_voltage_mV/1000U);uart2_send_char('.');uart2_send_char((char)('0'+(m->bus_voltage_mV/100U)%10U));uart2_send_char((char)('0'+(m->bus_voltage_mV/10U)%10U));uart2_send_string(" V\r\nCurrent: ");if(current<0){uart2_send_char('-');current=-current;}send_uint((uint32_t)(current/1000));uart2_send_char('.');uart2_send_char((char)('0'+(current/100)%10));uart2_send_char((char)('0'+(current/10)%10));uart2_send_char((char)('0'+current%10));uart2_send_string(" A\r\nPower: ");send_uint(m->power_mW/1000U);uart2_send_char('.');uart2_send_char((char)('0'+(m->power_mW/100U)%10U));uart2_send_char((char)('0'+(m->power_mW/10U)%10U));uart2_send_char((char)('0'+m->power_mW%10U));uart2_send_string(" W\r\n-------------------------\r\n");}
void uart2_send_mpu6050(const MPU6050_Data*d){uart2_send_string("\r\nMPU6050 DATA\r\n");uart2_send_string("Accel X: ");uart2_send_int(d->accel_x);uart2_send_string("\r\nAccel Y: ");uart2_send_int(d->accel_y);uart2_send_string("\r\nAccel Z: ");uart2_send_int(d->accel_z);uart2_send_string("\r\nTemperature raw: ");uart2_send_int(d->temperature_raw);uart2_send_string("\r\nGyro X: ");uart2_send_int(d->gyro_x);uart2_send_string("\r\nGyro Y: ");uart2_send_int(d->gyro_y);uart2_send_string("\r\nGyro Z: ");uart2_send_int(d->gyro_z);uart2_send_string("\r\n-------------------------\r\n");}
