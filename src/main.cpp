#include "mbed.h"
#include "rtos.h"

#include "MS5611Sensor.h"
#include "SHT31Sensor.h"

#include "SizedQueue.h"
#include "SizedMempool.h"

#include "SDBlockDevice.h"
#include "FSDataStore.h"

#include "RadioPacket.h"

#include "CansatBLE.h"
#include "BLELogger.h"

Thread packetgen_thread(osPriorityNormal, 1024, NULL, "packetgen");

DigitalOut led(MBED_CONF_APP_LED1, 0);
InterruptIn button(MBED_CONF_APP_BUTTON1, PullUp);

SizedQueue<packet_t, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioQueue;
SizedMempool<packet_t, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioMempool;

// SizedQueue<SensorData, MBED_CONF_APP_FEC_GROUP_SIZE> fecQueue;
// SizedMempool<SensorDataUnion, MBED_CONF_APP_SENSOR_MEMPOOL_SIZE> sensorMempool;

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

void packetGenerator()
{
    while (true) {
        MS5611Data *msData = (MS5611Data*) MS5611.lastValue();
        SHT31Data *shtData = (SHT31Data*) SHT31.lastValue();

        msData->pressure = 0;

        bool x = msData->valid();
        bool y = shtData->valid();
        LOGI("---- %d %d ----\n", x, y);

        // if (msData->valid())
            LOGI("sT %.2f sH %.1f\n", shtData->temperature, shtData->humidity);
        // else
            // LOGI("MS data invalid\n");

        // if (shtData->valid())
            LOGI("mT %.2f mP %.2f\n", msData->temperature, msData->pressure);
        // else
            // LOGI("SHT data invalid\n");

        ThisThread::sleep_for(1000);
    }
}

int main(void)
{
    LOGI("Starting... %d %d %d\n", 1, 2, 3);

    // MS5611Data test(8000, 0);
    // SensorData *ptr = (SensorData*)(void*) &packetgen_thread;
    // LOGI("%d\n", ptr->valid());
    
    button.fall(buttonPress);

    CansatBLE::init();

    // MS5611.setQueue(&fecQueue, &sensorMempool);
    MS5611.start(1000);

    // SHT31.setQueue(&fecQueue, &sensorMempool);
    SHT31.start(1000);

    ThisThread::sleep_for(2000);

    packetgen_thread.start(packetGenerator);


    /*while (true) {
        osEvent evt = fecQueue.get();
        if (evt.status == osEventMessage) {
            SensorData *data = (SensorData*) evt.value.p;

            switch (data->type) {
                case DataTypes::MS5611_dt:
                LOGI("T: %.2f P: %.2f\n",
                    ((MS5611Data*)data)->temperature,
                    ((MS5611Data*)data)->pressure
                );
                break;

                case DataTypes::SHT31_dt:
                LOGI("T: %.2f H: %.2f\n",
                    ((SHT31Data*)data)->temperature,
                    ((SHT31Data*)data)->humidity
                );
                break;

                default:
                LOGI("Unknown type\n");
                break;
            }

            sensorMempool.free((SensorDataUnion*)data);
        }
    }*/

    return 0;
}