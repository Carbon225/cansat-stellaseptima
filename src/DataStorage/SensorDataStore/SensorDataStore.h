#ifndef _SENSORDATASTORE_H_
#define _SENSORDATASTORE_H_

#include "mbed.h"
#include "DataTypes.h"
#include "Sensor.h"

class Sensor;

class SensorDataStore {
public:
    SensorDataStore();
    ~SensorDataStore();

    virtual mbed_error_status_t init() = 0;
    virtual mbed_error_status_t deinit() = 0;
    virtual mbed_error_status_t saveData(Sensor* sensor) = 0;
    virtual mbed_error_status_t reset() = 0;

    void schedule(Sensor *sensor, int delay);

    bool isActive() { return _active; }

protected:
    bool _active = false;

    virtual int _encode(SensorData *data, uint8_t *out = nullptr) = 0;

private:
    EventQueue _saveQueue;
    Thread _saveThread;
};


#endif