#include "FSDataStore.h"

FSDataStore::FSDataStore(FileSystem *fs)
: _fs(fs)
{

}

mbed_error_status_t FSDataStore::init()
{
    _active = true;
    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::deinit()
{
    _active = false;
    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::saveData(SensorData *data)
{
    if (!isActive()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_NOT_READY), "Sensor data store not initialized");
        return MBED_ERROR_NOT_READY;
    }

    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::reset()
{
    if (!isActive()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_NOT_READY), "Sensor data store not initialized");
        return MBED_ERROR_NOT_READY;
    }

    return MBED_SUCCESS;
}