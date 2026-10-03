#include "ina219.h"
#include "i2c.h"
#define REG_CONFIG 0x00U
#define REG_SHUNT 0x01U
#define REG_BUS 0x02U
#define REG_CURRENT 0x04U
#define REG_CALIB 0x05U
static uint8_t write_reg(uint8_t r,uint16_t v){if(!i2c1_start())return 0;if(!i2c1_address(INA219_ADDRESS<<1)){i2c1_stop();return 0;}if(!i2c1_write(r)||!i2c1_write((uint8_t)(v>>8))||!i2c1_write((uint8_t)v)){i2c1_stop();return 0;}i2c1_stop();return 1;}
static uint8_t read_reg(uint8_t r,uint16_t*v){uint8_t h,l;if(!i2c1_read_register_2bytes(INA219_ADDRESS,r,&h,&l))return 0;*v=((uint16_t)h<<8)|l;return 1;}
uint8_t ina219_init(void){return write_reg(REG_CALIB,4096U)&&write_reg(REG_CONFIG,0x199FU);}
uint8_t ina219_read_measurements(INA219_Measurements*m){uint16_t b,s,c;if(!read_reg(REG_BUS,&b))return 0;if(!read_reg(REG_SHUNT,&s))return 0;if(!read_reg(REG_CURRENT,&c))return 0;m->bus_voltage_mV=(uint32_t)(b>>3)*4U;m->shunt_voltage_uV=(int32_t)(int16_t)s*10;m->current_mA=(int32_t)(int16_t)c/10;m->power_mW=m->bus_voltage_mV*(uint32_t)(m->current_mA>=0?m->current_mA:-m->current_mA)/1000U;return 1;}
