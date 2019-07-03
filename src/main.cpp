#include "mbed.h"
#include "rtos.h"
#include "MS5611Sensor.h"
#include "Sht31.h"

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


void buttonPress()
{
    led = !led;
}

void shtTask()
{
    Sht31 sht(P0_22, P0_23); 

    pc.printf("SHT31 started\n");

    while(1) {
        double temp = sht.readTemperature();
        double hum = sht.readHumidity();

        pc.printf("ShtT: %.2f degC ShtH: %.1f%%\n", temp, hum);
        wait(2.0);
    }
}


int main(void)
{
    button.fall(buttonPress);
    pc.baud(115200);

    MS5611.setQueue(&sdQueue);
    MS5611.start(1000);

    // sht31_th.start(shtTask);

    while (true) {
        osEvent evt = sdQueue.get();
        if (evt.status == osEventMessage) {
            pc.printf("Queue data:\n");

            SensorData *data = (SensorData*) evt.value.p;

            switch (data->type) {
                case DataTypes::MS5611_dt:
                pc.printf("Temp: %.2f degC Barometer: %.1f mB\n",
                    ((MS5611Data*)data)->temperature, ((MS5611Data*)data)->pressure
                );
                break;

                default:
                pc.printf("Unknown type\n");
                break;
            }

            pc.printf("\n\n");

            Sensor::free(data);
        }
    }

    return 0;
}