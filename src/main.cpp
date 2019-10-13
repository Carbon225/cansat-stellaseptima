#include "mbed.h"
#include "rtos.h"

#include "SHT31Sensor.h"
#include "BMP280Sensor.h"
#include "MS5611Sensor.h"
#include "DoubleTemp.h"

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

SizedQueue<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioQueue;
SizedMempool<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioMempool;

// SizedQueue<SensorData, MBED_CONF_APP_FEC_GROUP_SIZE> fecQueue;
// SizedMempool<SensorDataUnion, MBED_CONF_APP_SENSOR_MEMPOOL_SIZE> sensorMempool;

BMP280Sensor BMP280_1(MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);
BMP280Sensor BMP280_2(MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL);

MS5611Sensor MS5611(MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL);

SHT31Sensor SHT31_1(MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);

// DoubleTemp DoubleSHT31(
//     MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL,
//     MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL
// );


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
    // DoubleSHT31.setDataStore(&store);
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
        PressureData *msData = (PressureData*) MS5611.lastValue();
        
        SHT31Data *shtData1 = (SHT31Data*) SHT31_1.lastValue();
        // SHT31Data *shtData2 = (SHT31Data*) SHT31_2.lastValue();
        
        PressureData *bmpData1 = (PressureData*) BMP280_1.lastValue();
        PressureData *bmpData2 = (PressureData*) BMP280_2.lastValue();

        if (shtData1->valid())
            LOGI("s1T %.2f s1H %.1f\n", shtData1->temperature, shtData1->humidity);
        else
            LOGI("SHT1 data invalid\n");

/*
        if (shtData2->valid())
            LOGI("s2T %.2f s2H %.1f\n", shtData2->temperature, shtData2->humidity);
        else
            LOGI("SHT2 data invalid\n");
*/


        if (msData->valid())
            LOGI("mP %.2f\n", msData->pressure);
        else
            LOGI("MS data invalid\n");

        if (bmpData1->valid())
            LOGI("b1P %.2f\n", bmpData1->pressure);
        else
           LOGI("BMP1 data invalid\n");

        if (bmpData2->valid())
            LOGI("b2P %.2f\n", bmpData2->pressure);
        else
           LOGI("BMP2 data invalid\n");

        LOGI("\n");
/*
        LOGI("Generating packet...\n");

        RadioPacket *packet = radioMempool.alloc();
        
        if (packet != nullptr) {
            new(packet) RadioPacket(7, shtData->temperature, bmpData->pressure);
            radioQueue.put(packet);
            LOGI("%#x\n", *packet);
        }
        else {
            LOGI("Radio queue full\n");
        }
*/

        ThisThread::sleep_for(1000);
    }
}

int main(void)
{
    LOGI("Starting... %d %d %d\n", 1, 2, 3);
   
    button.fall(buttonPress);

    CansatBLE::init();

    BMP280_1.start(500);
    BMP280_2.start(500);

    MS5611.start(500);
    // DoubleSHT31.start(1000);

    SHT31_1.start(500);

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