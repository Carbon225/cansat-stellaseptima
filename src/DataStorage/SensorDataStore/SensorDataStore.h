#ifndef _SENSORDATASTORE_H_
#define _SENSORDATASTORE_H_

#include "mbed.h"
#include "DataTypes.h"

class SensorDataStore {
public:
    virtual mbed_error_status_t init() = 0;
    virtual mbed_error_status_t deinit() = 0;
    virtual mbed_error_status_t saveData(SensorData* data) = 0;
    virtual mbed_error_status_t reset() = 0;

    bool isActive() { return _active; }

protected:
    bool _active = false;
};


#endif