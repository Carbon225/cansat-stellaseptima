#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mbed.h"
#include "rtos.h"
#include "DataTypes.h"
#include "QueueInterface.h"
#include "MempoolInterface.h"
#include "SensorDataStore.h"

class Sensor : NonCopyable<Sensor>
{
public:
    Sensor();
    ~Sensor();

    void setDataStore(SensorDataStore *store);
    void setQueue(QueueInterface<SensorData> *queue, MempoolInterface<SensorDataUnion> *memPool);
    void start(int delay_ms);
    void stop();

protected:
    virtual mbed_error_status_t setup() = 0;
    virtual mbed_error_status_t read(SensorData*) = 0;

private:
    int _delay_ms;

    SensorDataStore *_store;
    QueueInterface<SensorData> *_dataQueue;
    MempoolInterface<SensorDataUnion> *_memPool;

    Thread _sensor_thread;
    void _sensor_task();
};

#endif // _SENSOR_H_