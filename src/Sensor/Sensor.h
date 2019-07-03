#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mbed.h"
#include "rtos.h"
#include "DataTypes.h"

#define DATA_QUEUE_SIZE 5


class Sensor : NonCopyable<Sensor>
{
public:
    Sensor();
    ~Sensor();

    void setQueue(Queue<SensorData, DATA_QUEUE_SIZE> *queue);
    void start(int delay_ms);
    void stop();

    static osStatus free(SensorData*);

protected:
    virtual int setup() = 0;
    virtual SensorData* read() = 0;

    static MemoryPool<SensorDataUnion, DATA_QUEUE_SIZE> _memPool;

private:
    int _delay_ms;

    Queue<SensorData, DATA_QUEUE_SIZE> *_dataQueue;

    Thread _sensor_thread;
    void _sensor_task();
};

#endif // _SENSOR_H_