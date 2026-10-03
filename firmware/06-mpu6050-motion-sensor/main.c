#include <stdint.h>
#include "delay.h"
#include "ds18b20.h"
#include "i2c.h"
#include "ina219.h"
#include "mpu6050.h"
#include "oled.h"
#include "uart.h"
#include "buzzer.h"
int main(void){
 int16_t temperature_raw; INA219_Measurements power; MPU6050_Data motion; uint8_t ina219_ok,mpu6050_ok;
 delay_init(); uart2_init(); i2c1_init(); ds18b20_init(); buzzer_init(); oled_init();
 uart2_send_string("\r\n================================\r\nSTM32F446RE MULTISENSOR MONITOR\r\nDS18B20 + INA219 + MPU6050 + OLED\r\n================================\r\n");
 buzzer_on(); delay_ms(100U); buzzer_off();
 ina219_ok=ina219_init(); mpu6050_ok=mpu6050_init();
 uart2_send_string(ina219_ok?"INA219 Initialized\r\n":"INA219 Initialization Failed!\r\n");
 uart2_send_string(mpu6050_ok?"MPU6050 Initialized\r\n":"MPU6050 Initialization Failed!\r\n");
 while(1){
  temperature_raw=ds18b20_read_temperature_raw();
  if(temperature_raw==DS18B20_ERROR){uart2_send_string("DS18B20: SENSOR ERROR\r\n");oled_show_error();}
  else{uart2_send_string("Temperature: ");uart2_send_temperature(temperature_raw);oled_show_temperature(temperature_raw);}
  if(ina219_ok&&ina219_read_measurements(&power))uart2_send_ina219(&power);else uart2_send_string("INA219: READ ERROR\r\n");
  if(mpu6050_ok&&mpu6050_read_data(&motion)){uart2_send_mpu6050(&motion);oled_show_mpu6050(&motion);}else uart2_send_string("MPU6050: READ ERROR\r\n");
  delay_ms(1000U);
 }
}
