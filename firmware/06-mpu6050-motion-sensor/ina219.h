#ifndef INA219_H
#define INA219_H
#include <stdint.h>
#define INA219_ADDRESS 0x40U
typedef struct { uint32_t bus_voltage_mV; int32_t shunt_voltage_uV; int32_t current_mA; uint32_t power_mW; } INA219_Measurements;
uint8_t ina219_init(void);
uint8_t ina219_read_measurements(INA219_Measurements *m);
#endif
