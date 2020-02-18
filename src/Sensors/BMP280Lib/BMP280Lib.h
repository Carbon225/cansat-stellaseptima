#ifndef _BMP280LIB_H_
#define _BMP280LIB_H_

#include "mbed.h"
#include "bmp280_defs.h"

class BMP280Lib
{
public:
    enum CSBState {
        High,
        Low
    };

    BMP280Lib(PinName sda, PinName scl, CSBState csb = CSBState::High);
    ~BMP280Lib();

    int init(uint8_t filter = BMP280_FILTER_COEFF_2, uint8_t osPres = BMP280_OS_4X, uint8_t osTemp = BMP280_OS_4X, uint8_t odr = BMP280_ODR_250_MS);
    int read();
    double getPressure();
    double getTemperature();

private:
    bmp280_dev _dev;
    I2C _i2c;
    uint8_t _addr;

    double _temp = 0.0;
    double _press = 0.0;

    static BMP280Lib *_sensors[];
    static int _nSensors;

    static int8_t i2c_reg_write(uint8_t sensorID, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
    static int8_t i2c_reg_read(uint8_t sensorID, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
};

#endif