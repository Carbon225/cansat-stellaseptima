#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mbed.h"
#include "rtos.h"
#include "DataTypes.h"
#include "QueueInterface.h"
#include "MempoolInterface.h"

class Sensor : NonCopyable<Sensor>
{
public:
    Sensor();
    ~Sensor();

    void setQueue(QueueInterface<SensorData> *queue, MempoolInterface<SensorDataUnion> *memPool);
    void start(int delay_ms);
    void stop();

protected:
    virtual int setup() = 0;
    virtual int read(SensorData*) = 0;

private:
    int _delay_ms;

    QueueInterface<SensorData> *_dataQueue;
    MempoolInterface<SensorDataUnion> *_memPool;

    Thread _sensor_thread;
    void _sensor_task();
};

#endif // _SENSOR_H_