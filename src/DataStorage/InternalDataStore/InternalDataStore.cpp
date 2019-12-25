#include "InternalDataStore.h"
#include "BLELogger.h"
#include "RadioPacket.h"

InternalDataStore::InternalDataStore(const char mount[]) // ex. /sd
: FSDataStore(mount),
  _fs(mount + 1) // trim /
{
    
}

InternalDataStore::~InternalDataStore()
{
    deinit();
}

/*
SHT = 9 bits temperature + 7 bits humidity
Pressure 16 bits
GPS = 12 bits + 12 bits
 */

int InternalDataStore::_encode(SensorData *data, uint8_t *out)
{
    switch (data->type) {
        case DataTypes::Pressure_dt:
        if (out) {
            int pressure = scaleToBits(((PressureData*)data)->pressure, 700, 1100, 65535);
            memcpy(out, &pressure, 2);
        }
        return 2;

        case DataTypes::SHT31_dt:
        if (out) {
            int temp = scaleToBits(((SHT31Data*)data)->temperature, -10, 40, 511);
            int hum = scaleToBits(((SHT31Data*)data)->humidity, 0, 100, 128);

            int data = temp;
            data |= hum << 9;

            memcpy(out, &data, 2);
        }
        return 2;

        case DataTypes::GPS_dt:
        if (out) {
            memcpy(out, &((GPSData*)data)->lat, sizeof(double));
            memcpy(out + sizeof(double), &((GPSData*)data)->lng, sizeof(double));
        }
        return 2 * sizeof(double);
        break;
    }

    return 0;
}

mbed_error_status_t InternalDataStore::init()
{
    LOGI("Starting internal flash\n");
    _bd.init();

    LOGI("Flash block device size: %llu\n",         _bd.size());
    LOGI("Flash block device read size: %llu\n",    _bd.get_read_size());
    LOGI("Flash block device program size: %llu\n", _bd.get_program_size());
    LOGI("Flash block device erase size: %llu\n",   _bd.get_erase_size());
    
    LOGI("Mounting filesystem\n");
    int err = _fs.mount(&_bd);
    if (err) {
        LOGI("Mounting failed\n");
        LOGI("Reformatting\n");
        
        if (_fs.reformat(&_bd) != 0) {
            LOGI("Reformatting failed\n");
            _bd.deinit();
            _active = false;
            return MBED_ERROR_INITIALIZATION_FAILED;
        }
    }
    LOGI("Internal flash OK\n");

    return FSDataStore::init();
}

mbed_error_status_t InternalDataStore::deinit()
{
    FSDataStore::deinit();
    _fs.unmount();
    _bd.deinit();
    _active = false;
    return MBED_SUCCESS;
}