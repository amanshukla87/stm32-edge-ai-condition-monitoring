#include "ina219.h"
#include "i2c.h"

#define INA219_ADDRESS 0x40U
#define REG_CONFIG     0x00U
#define REG_SHUNT      0x01U
#define REG_BUS        0x02U
#define REG_POWER      0x03U
#define REG_CURRENT    0x04U
#define REG_CALIB      0x05U

static uint8_t write_register(uint8_t reg, uint16_t value)
{
    if (!i2c1_start()) return 0U;
    if (!i2c1_address((INA219_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg) ||
        !i2c1_write((uint8_t)(value >> 8)) ||
        !i2c1_write((uint8_t)value))
    {
        i2c1_stop();
        return 0U;
    }

    i2c1_stop();
    return 1U;
}

static uint8_t read_register(uint8_t reg, uint16_t *value)
{
    uint8_t high;
    uint8_t low;
    uint32_t timeout = 100000U;

    if (!i2c1_start()) return 0U;
    if (!i2c1_address((INA219_ADDRESS << 1) | 0U))
    {
        i2c1_stop();
        return 0U;
    }

    if (!i2c1_write(reg))
    {
        i2c1_stop();
        return 0U;
    }

    /* Repeated START for the read transaction. */
    I2C1_CR1_PLACEHOLDER;
    return 0U;
}

uint8_t ina219_init(void)
{
    return write_register(REG_CALIB, 4096U) &&
           write_register(REG_CONFIG, 0x399FU);
}

uint8_t ina219_read_measurements(INA219_Measurements *m)
{
    uint16_t raw;
    int16_t signed_raw;

    if (!read_register(REG_BUS, &raw)) return 0U;
    raw >>= 3;
    m->bus_voltage_mV = (uint32_t)raw * 4U;

    if (!read_register(REG_SHUNT, &raw)) return 0U;
    signed_raw = (int16_t)raw;
    m->shunt_voltage_uV = (int32_t)signed_raw * 10;

    if (!read_register(REG_CURRENT, &raw)) return 0U;
    signed_raw = (int16_t)raw;
    m->current_mA = (signed_raw >= 0) ?
                    ((int32_t)signed_raw + 5) / 10 :
                    ((int32_t)signed_raw - 5) / 10;

    if (!read_register(REG_POWER, &raw)) return 0U;
    m->power_mW = (uint32_t)raw * 2U;

    return 1U;
}
