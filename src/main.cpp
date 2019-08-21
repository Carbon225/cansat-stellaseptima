#include "mbed.h"
#include "rtos.h"

#include "MS5611Sensor.h"
#include "SHT31Sensor.h"

#include "SizedQueue.h"
#include "SizedMempool.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1, 0);
InterruptIn button(BUTTON1, PullUp);

SizedQueue<SensorData, MBED_CONF_APP_FEC_GROUP_SIZE> fecQueue;
SizedMempool<SensorDataUnion, MBED_CONF_APP_SENSOR_MEMPOOL_SIZE> sensorMempool;

MS5611Sensor MS5611(MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);
SHT31Sensor SHT31(MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);


void buttonPress()
{
    led = !led;
}

int main(void)
{
    button.fall(buttonPress);
    pc.baud(115200);

    MS5611.setQueue(&fecQueue, &sensorMempool);
    MS5611.start(1000);

    SHT31.setQueue(&fecQueue, &sensorMempool);
    SHT31.start(1000);

    while (true) {
        osEvent evt = fecQueue.get();
        if (evt.status == osEventMessage) {
            SensorData *data = (SensorData*) evt.value.p;

            switch (data->type) {
                case DataTypes::MS5611_dt:
                pc.printf("Temp: %.2f degC Barometer: %.2f mB\n",
                    ((MS5611Data*)data)->temperature,
                    ((MS5611Data*)data)->pressure
                );
                break;

                case DataTypes::SHT31_dt:
                pc.printf("Temp: %.2f degC Hum: %.2f%%\n",
                    ((SHT31Data*)data)->temperature,
                    ((SHT31Data*)data)->humidity
                );
                break;

                default:
                pc.printf("Unknown type\n");
                break;
            }

            sensorMempool.free((SensorDataUnion*)data);
        }
    }

    return 0;
}