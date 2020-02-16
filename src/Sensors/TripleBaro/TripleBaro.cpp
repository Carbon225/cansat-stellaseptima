#include "TripleBaro.h"
#include "BLELogger.h"

TripleBaro::TripleBaro(const char name[], PinName sda1, PinName scl1, PinName sda2, PinName scl2)
: Sensor(name), _last_value(name),
  _ms1(sda1, scl1, ms5611::CSBpin_1), _bmp1(sda1, scl1), _bmp2(sda2, scl2)
{

}

TripleBaro::~TripleBaro()
{

}

SensorData* TripleBaro::lastValue()
{
    return &_last_value;
}

mbed_error_status_t TripleBaro::setup()
{
    _ms1.cmd_reset();
    _bmp1.initialize();
    _bmp2.initialize();

    LOGI("Triple baro started\n");
    return 0;
}

mbed_error_status_t TripleBaro::read()
{;
    // _ms1.calcTemp();
    double p1 = _ms1.calcPressure();
    
    _bmp1.getTemperature();
    double p2 = _bmp1.getPressure();
    
    _bmp2.getTemperature();
    double p3 = _bmp2.getPressure();

    _last_value.pressure1 = p1;
    _last_value.pressure2 = p2;
    _last_value.pressure3 = p3;

    bool data1Valid = PressureData(p1).valid();
    bool data2Valid = PressureData(p2).valid();
    bool data3Valid = PressureData(p3).valid();

    uint8_t validMask = data3Valid | (data2Valid << 1) | (data1Valid << 2);

    switch (validMask) {
        case 0b111:
        _last_value.pressure = calculateTMR(p1, p2, p3, PRESS_DIVERGENCE);
        break;

        case 0b110:
        _last_value.pressure = (p1 + p2) / 2.f;
        break;

        case 0b101:
        _last_value.pressure = (p1 + p3) / 2.f;
        break;

        case 0b011:
        _last_value.pressure = (p2 + p3) / 2.f;
        break;

        case 0b001:
        _last_value.pressure = p3;
        break;

        case 0b010:
        _last_value.pressure = p2;
        break;

        case 0b100:
        _last_value.pressure = p1;
        break;

        default:
        _last_value.pressure = 0.f;
        return MBED_ERROR_READ_FAILED;
    }

    return MBED_SUCCESS;
}

double TripleBaro::calculateTMR(double v1, double v2, double v3, const double D)
{
    static double tab[3];
    if (v1 <= v2) {
        if (v3 <= v1) {
            tab[0] = v3;
            tab[1] = v1;
            tab[2] = v2;
        }
        else if (v2 <= v3) {
            tab[0] = v1;
            tab[1] = v2;
            tab[2] = v3;
        }
        else {
            tab[0] = v1;
            tab[1] = v3;
            tab[2] = v2;
        }
    }
    else {
        if (v3 <= v2) {
            tab[0] = v3;
            tab[1] = v2;
            tab[2] = v1;
        }
        else if (v3 <= v1) {
            tab[0] = v2;
            tab[1] = v3;
            tab[2] = v1;
        }
        else {
            tab[0] = v2;
            tab[1] = v1;
            tab[2] = v3;
        }
    }

    double d1, d2;
    d1 = tab[1] - tab[0];
    d2 = tab[2] - tab[1];

    if (d1 < 0.f || d2 < 0.f) {
        // MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_ASSERTION_FAILED), "Array not sorted" );
        return 0.f;
    }

    if (d1 > D) {
        return (tab[1] + tab[2]) / 2.f;
    }
    else if (d2 > D) {
        return (tab[0] + tab[1]) / 2.f;
    }
    else {
        return (tab[0] + tab[1] + tab[2]) / 3.f;
    }
}