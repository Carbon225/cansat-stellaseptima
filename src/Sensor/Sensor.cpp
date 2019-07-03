#include "Sensor.h"

extern Serial pc;


MemoryPool<SensorDataUnion, DATA_QUEUE_SIZE> Sensor::_memPool;

osStatus Sensor::free(SensorData *block)
{
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
    if (ret != MBED_SUCCESS) {
        pc.printf("Sensor returned error code %d\n", ret);

        return;
    }

    while (true) {
        // alocate space for new data object
        SensorData *data = (SensorData*)_memPool.alloc();
        if (!data) {
            pc.printf("Mempool alloc error\n");
        }
        else {
            // if sensor was read and queue exists add data to queue
            if (read(data) == MBED_SUCCESS && _dataQueue) {
                _dataQueue->put(data);
            }
            // if data wasn't queued free allocated space
            else {
                _memPool.free((SensorDataUnion*)data);
            }
        }

        wait_ms(_delay_ms);
    }
}