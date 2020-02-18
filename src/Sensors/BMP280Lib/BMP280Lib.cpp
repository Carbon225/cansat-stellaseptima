#include "BMP280Lib.h"
#include "./BMP280_driver/bmp280.h"
#include "BLELogger.h"

void delay_ms(uint32_t period_ms);

int BMP280Lib::_nSensors = 0;
BMP280Lib* BMP280Lib::_sensors[256];

BMP280Lib::BMP280Lib(PinName sda, PinName scl, CSBState csb)
: _i2c(sda, scl), _addr((csb == CSBState::Low ? BMP280_I2C_ADDR_PRIM : BMP280_I2C_ADDR_SEC) << 1)
{
    _sensors[_nSensors++] = this;
    _dev.dev_id = _nSensors - 1;

    _dev.delay_ms = delay_ms;
    _dev.intf = BMP280_I2C_INTF;
    _dev.read = &BMP280Lib::i2c_reg_read;
    _dev.write = &BMP280Lib::i2c_reg_write;
}

BMP280Lib::~BMP280Lib()
{
    _sensors[_dev.dev_id] = _sensors[_nSensors - 1];
    _sensors[_dev.dev_id]->_dev.dev_id = _dev.dev_id;
    _sensors[_nSensors - 1] = NULL;
    _nSensors--;
}

int BMP280Lib::init(uint8_t filter, uint8_t osPres, uint8_t osTemp, uint8_t odr)
{
    int ret = bmp280_init(&_dev);
    // LOGI("BMP init %d\n", ret);

    bmp280_config conf;
    ret = bmp280_get_config(&conf, &_dev);
    // LOGI("BMP get config %d\n", ret);

    conf.filter = filter;
    conf.os_pres = osPres;
    conf.os_temp = osTemp;
    conf.odr = odr;

    ret = bmp280_set_config(&conf, &_dev);
    // LOGI("BMP set config %d\n", ret);

    ret = bmp280_set_power_mode(BMP280_NORMAL_MODE, &_dev);
    // LOGI("BMP power mode %d\n", ret);

    return ret;

}

int BMP280Lib::read()
{
    bmp280_uncomp_data ucomp_data;
    int ret = bmp280_get_uncomp_data(&ucomp_data, &_dev);

    ret = bmp280_get_comp_temp_double(&_temp, ucomp_data.uncomp_temp, &_dev);
    ret = bmp280_get_comp_pres_double(&_press, ucomp_data.uncomp_press, &_dev);
    _press /= 100.0;

    return ret;
}

double BMP280Lib::getPressure()
{
    return _press;
}

double BMP280Lib::getTemperature()
{
    return _temp;
}

void delay_ms(uint32_t period_ms)
{
    ThisThread::sleep_for(period_ms);
}

int8_t BMP280Lib::i2c_reg_write(uint8_t sensorID, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    BMP280Lib *sensor = _sensors[sensorID];
    
    char cmd[length + 1];
    cmd[0] = reg_addr;
    for (int i = 0; i < length; i++) {
        cmd[i + 1] = reg_data[i];
    }

    sensor->_i2c.lock();
    int ret = sensor->_i2c.write(sensor->_addr, cmd, length);
    sensor->_i2c.unlock();

    return ret;
}

int8_t BMP280Lib::i2c_reg_read(uint8_t sensorID, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    BMP280Lib *sensor = _sensors[sensorID];
    
    sensor->_i2c.lock();
    int ret = sensor->_i2c.write(sensor->_addr, (const char*)&reg_addr, 1);
    ret += sensor->_i2c.read(sensor->_addr,  (char*)reg_data, length);
    sensor->_i2c.unlock();

    return ret;
}