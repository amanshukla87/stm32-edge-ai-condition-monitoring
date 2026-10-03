#include "mpu6050.h"
#include "i2c.h"
#include "delay.h"
#define PWR_MGMT_1 0x6BU
#define SMPLRT_DIV 0x19U
#define CONFIG_REG 0x1AU
#define GYRO_CONFIG 0x1BU
#define ACCEL_CONFIG 0x1CU
#define ACCEL_XOUT_H 0x3BU
#define TEMP_OUT_H 0x41U
#define GYRO_XOUT_H 0x43U

static uint8_t write_reg(uint8_t r,uint8_t v){
 if(!i2c1_start())return 0; if(!i2c1_address(MPU6050_ADDRESS<<1)){i2c1_stop();return 0;}
 if(!i2c1_write(r)||!i2c1_write(v)){i2c1_stop();return 0;} i2c1_stop(); delay_ms(2); return 1;
}
static uint8_t read_16(uint8_t r,int16_t *v){
 uint8_t h,l; if(!i2c1_read_register_2bytes(MPU6050_ADDRESS,r,&h,&l))return 0; *v=(int16_t)(((uint16_t)h<<8)|l); return 1;
}
uint8_t mpu6050_init(void){
 return write_reg(PWR_MGMT_1,0x00U)&&write_reg(SMPLRT_DIV,0x07U)&&write_reg(CONFIG_REG,0x00U)&&write_reg(GYRO_CONFIG,0x00U)&&write_reg(ACCEL_CONFIG,0x00U);
}
uint8_t mpu6050_read_data(MPU6050_Data *d){
 return read_16(ACCEL_XOUT_H,&d->accel_x)&&read_16(ACCEL_XOUT_H+2U,&d->accel_y)&&read_16(ACCEL_XOUT_H+4U,&d->accel_z)&&read_16(TEMP_OUT_H,&d->temperature_raw)&&read_16(GYRO_XOUT_H,&d->gyro_x)&&read_16(GYRO_XOUT_H+2U,&d->gyro_y)&&read_16(GYRO_XOUT_H+4U,&d->gyro_z);
}
