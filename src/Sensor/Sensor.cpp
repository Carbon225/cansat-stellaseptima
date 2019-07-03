#include "Sensor.h"

extern Serial pc;


MemoryPool<SensorDataUnion, DATA_QUEUE_SIZE> Sensor::_memPool;

osStatus Sensor::free(SensorData *block)
{
    pc.printf("Freeing mempool\n");
    return _memPool.free((SensorDataUnion*) block);
}

Sensor::Sensor()
: _dataQueue(nullptr), _delay_ms(10)
{

}

Sensor::~Sensor()
{

}

void Sensor::setQueue(Queue<SensorData, DATA_QUEUE_SIZE> *queue)
{
    _dataQueue = queue;
}

void Sensor::start(int delay_ms)
{
    _delay_ms = delay_ms;
    _sensor_thread.start(callback(this, &Sensor::_sensor_task));
}

void Sensor::stop()
{
    _sensor_thread.terminate();
}

void Sensor::_sensor_task()
{
    int ret = setup();
    if (ret != 0) {
        pc.printf("Sensor return error code %d\n", ret);

        return;
    }

    while (true) {
        SensorData *data = read();
        
        if (_dataQueue && data) {
            pc.printf("Adding to queue\n");
            _dataQueue->put(data);
        }

        wait_ms(_delay_ms);
    }
}