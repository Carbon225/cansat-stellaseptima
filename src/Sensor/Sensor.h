#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mbed.h"
#include "rtos.h"
#include "DataTypes.h"
#include "SensorDataStore.h"

class Sensor : NonCopyable<Sensor>
{
public:
    Sensor(const char *name);
    virtual ~Sensor();

    virtual SensorData* lastValue() = 0;

    void start(int delay_ms);
    void stop();

protected:
    virtual mbed_error_status_t setup() = 0;
    virtual mbed_error_status_t read() = 0;

private:
    int _eventId;

    static EventQueue _sensorsEvQueue;
    static Thread _sensorsThread;
};

#endif // _SENSOR_H_