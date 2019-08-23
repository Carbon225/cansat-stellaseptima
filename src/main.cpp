#include "mbed.h"
#include "rtos.h"

#include "MS5611Sensor.h"
#include "SHT31Sensor.h"

#include "SizedQueue.h"
#include "SizedMempool.h"

#include "FSDataStore.h"

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

void sdTest()
{
    BlockDevice *bd = BlockDevice::get_default_instance();
    FileSystem *fs = FileSystem::get_default_instance();

    printf("SD init... ");
    fflush(stdout);
    if (bd->init() != 0) {
        printf("Fail\n");
        return;
    }

    printf("OK\n");

    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs->mount(bd);
    
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        err = fs->reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err) {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }

    static FSDataStore store(fs);
    SHT31.setDataStore(&store);
    MS5611.setDataStore(&store);

    // Display the root directory
    printf("Opening the root directory... ");
    fflush(stdout);
    DIR *d = opendir("/fs/");
    printf("%s\n", (!d ? "Fail :(" : "OK"));
    if (!d) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    printf("root directory:\n");
    while (true) {
        struct dirent *e = readdir(d);
        if (!e) {
            break;
        }

        printf("    %s\n", e->d_name);
    }

    fs->unmount();
    bd->deinit();
}

int main(void)
{
    button.fall(buttonPress);

    sdTest();

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
                printf("Temp: %.2f degC Barometer: %.2f mB\n",
                    ((MS5611Data*)data)->temperature,
                    ((MS5611Data*)data)->pressure
                );
                break;

                case DataTypes::SHT31_dt:
                printf("Temp: %.2f degC Hum: %.2f%%\n",
                    ((SHT31Data*)data)->temperature,
                    ((SHT31Data*)data)->humidity
                );
                break;

                default:
                printf("Unknown type\n");
                break;
            }

            sensorMempool.free((SensorDataUnion*)data);
        }
    }

    return 0;
}