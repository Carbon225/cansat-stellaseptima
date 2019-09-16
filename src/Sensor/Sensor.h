#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mbed.h"
#include "rtos.h"
#include "DataTypes.h"
#include "SensorDataStore.h"

class Sensor : NonCopyable<Sensor>
{
public:
    Sensor(size_t data_size, const char *name);
    virtual ~Sensor();

    SensorData* lastValue();

    void setDataStore(SensorDataStore *store);
    void start(int delay_ms);
    void stop();

protected:
    virtual mbed_error_status_t setup() = 0;
    virtual mbed_error_status_t read(SensorData*) = 0;

private:
    int _delay_ms;

    SensorData *_last_value;

    SensorDataStore *_store;
    
    Thread _sensor_thread;
    void _sensor_task();
};

#endif // _SENSOR_H_