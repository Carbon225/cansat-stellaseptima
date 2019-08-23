#include "Sensor.h"

Sensor::Sensor()
: _dataQueue(nullptr), _memPool(nullptr), _store(nullptr), _delay_ms(10)
{

}

Sensor::~Sensor()
{

}

void Sensor::setDataStore(SensorDataStore *store)
{
    _store = store;
}

void Sensor::setQueue(QueueInterface<SensorData> *queue, MempoolInterface<SensorDataUnion> *memPool)
{
    _dataQueue = queue;
    _memPool = memPool;
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
        printf("Sensor returned error code %d\n", ret);

        return;
    }

    while (true) {
        SensorData *data = (SensorData*)_memPool->alloc();
        if (!data) {
            printf("Mempool alloc error\n");
        }
        else {
            if (read(data) == MBED_SUCCESS && _dataQueue) {
                _dataQueue->put(data);
                if (_store) {
                    _store->saveData(data);
                }
            }
            else {
                _memPool->free((SensorDataUnion*)data);
            }
        }

        wait_ms(_delay_ms);
    }
}