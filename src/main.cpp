#include "mbed.h"
#include "rtos.h"
#include "MS5611Sensor.h"
#include "SHT31Sensor.h"

// how many packets in error correction group
#define FEC_GROUP_SIZE 5

Serial pc(P0_3, P0_2);

DigitalOut led(P0_7, 1);
InterruptIn button(P0_21, PullUp);

Thread sht31_th;


Queue<SensorData, DATA_QUEUE_SIZE> sdQueue;
Queue<SensorData, 16> radioQueue;
Queue<SensorData, FEC_GROUP_SIZE> encoderQueue;

MS5611Sensor MS5611(P0_22, P0_23);
SHT31Sensor SHT31(P0_22, P0_23);


void buttonPress()
{
    led = !led;
}

int main(void)
{
    button.fall(buttonPress);
    pc.baud(115200);

    MS5611.setQueue(&sdQueue);
    MS5611.start(1000);

    SHT31.setQueue(&sdQueue);
    SHT31.start(1000);

    while (true) {
        osEvent evt = sdQueue.get();
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

            Sensor::free(data);
        }
    }

    return 0;
}