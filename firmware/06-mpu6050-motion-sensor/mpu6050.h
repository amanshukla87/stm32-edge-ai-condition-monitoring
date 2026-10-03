#ifndef MPU6050_H
#define MPU6050_H
#include <stdint.h>
#define MPU6050_ADDRESS 0x68U
typedef struct { int16_t accel_x, accel_y, accel_z; int16_t temperature_raw; int16_t gyro_x, gyro_y, gyro_z; } MPU6050_Data;
uint8_t mpu6050_init(void);
uint8_t mpu6050_read_data(MPU6050_Data *data);
#endif
