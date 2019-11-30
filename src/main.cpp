#include "mbed.h"
#include "rtos.h"

#include "SHT31Sensor.h"
#include "BMP280Sensor.h"
#include "MS5611Sensor.h"
#include "DoubleTemp.h"
#include "GPSSensor.h"

#include "SizedQueue.h"
#include "SizedMempool.h"

#include "SDDataStore.h"

#include "Parachute.h"
#include "Radio.h"
#include "SensorPacket.h"
#include "GPSPacket.h"

#include "CansatBLE.h"
#include "BLELogger.h"

Thread packetgen_thread(osPriorityNormal, 1024, NULL, "packetgen");

SizedQueue<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioQueue;
SizedMempool<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioMempool;

namespace Sensors
{
    BMP280Sensor BMP280_1("bmp1", MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);
    // BMP280Sensor BMP280_2("bmp2", MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL);

    MS5611Sensor MS5611("ms5611", MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL);

    SHT31Sensor SHT31_1("sht1", MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL);
    SHT31Sensor SHT31_2("sht2", MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL);

    // DoubleTemp DoubleSHT31(
    //     "doublesht",
    //     MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL,
    //     MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL
    // );

    GPSSensor gps("gps", MBED_CONF_APP_GPS_RX, NC);
}

SDDataStore SDStore("/sd");

Parachute parachute(&Sensors::MS5611, MBED_CONF_APP_MOTOR_PIN);

void packetGenerator()
{
    while (true) {
        LOGI("\n");

        PressureData *msData = (PressureData*) Sensors::MS5611.lastValue();
        
        SHT31Data *shtData1 = (SHT31Data*) Sensors::SHT31_1.lastValue();
        SHT31Data *shtData2 = (SHT31Data*) Sensors::SHT31_2.lastValue();
        
        PressureData *bmpData1 = (PressureData*) Sensors::BMP280_1.lastValue();
        // PressureData *bmpData2 = (PressureData*) BMP280_2.lastValue();

        GPSData *gpsData = (GPSData*) Sensors::gps.lastValue();

        if (shtData1->valid())
            LOGI("s1T %.2f s1H %.1f\n", shtData1->temperature, shtData1->humidity);
        else
            LOGI("SHT1 data invalid\n");


        if (shtData2->valid())
            LOGI("s2T %.2f s2H %.1f\n", shtData2->temperature, shtData2->humidity);
        else
            LOGI("SHT2 data invalid\n");


        if (msData->valid())
            LOGI("mP %.2f\n", msData->pressure);
        else
            LOGI("MS data invalid\n");


        if (bmpData1->valid())
            LOGI("b1P %.2f\n", bmpData1->pressure);
        else
           LOGI("BMP1 data invalid\n");

        if (gpsData->valid())
            LOGI("lat: %.4f lng: %.4f\n", gpsData->lat, gpsData->lng);
        else
           LOGI("GPS data invalid\n");

/*
        if (bmpData2->valid())
            LOGI("b2P %.2f\n", bmpData2->pressure);
        else
           LOGI("BMP2 data invalid\n");
*/

        switch (parachute.state()) {
            case ParachuteState::Ascending:
            LOGI("Parachute mode: Ascending\n");
            break;

            case ParachuteState::Descending:
            LOGI("Parachute mode: Descending\n");
            break;

            case ParachuteState::Opening:
            LOGI("Parachute mode: Opening\n");
            break;

            case ParachuteState::Done:
            LOGI("Parachute mode: Done\n");
            break;
        }

        LOGI("Generating packet...\n");

        RadioPacket *packet = radioMempool.alloc();
        
        if (packet != nullptr) {
            new(packet) SensorPacket(7, shtData1->temperature, bmpData1->pressure);

            // LOGI("Begin packet\n");
            // radio.beginPacket(false);
            // LOGI("Write packet\n");
            // radio.write((uint8_t*)packet->toBinary(), sizeof(packet_t));
            // LOGI("End packet\n");
            // radio.endPacket(true);


            // radioQueue.put(packet);
            LOGI("%#x\n", *packet);
            radioMempool.free(packet);
        }
        else {
            LOGI("Radio queue full\n");
        }

        LOGI("---STATS---\n");

        mbed_stats_heap_t heap_stats;
        mbed_stats_heap_get(&heap_stats);
        LOGI("Reserved heap: %u\n", heap_stats.reserved_size);
        LOGI("Current heap: %u\n", heap_stats.current_size);
        LOGI("Max heap size: %u\n", heap_stats.max_size);

        int cnt = osThreadGetCount();
        mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

        cnt = mbed_stats_stack_get_each(stats, cnt);
        for (int i = 0; i < cnt; i++) {
            LOGI("Thread: 0x%X, Stack size: %u, Max stack: %u\n", 
                    stats[i].thread_id, stats[i].reserved_size, stats[i].max_size);
        }

        free(stats);

        LOGI("---STATS---\n");

        ThisThread::sleep_for(1000);
    }
}


int main(void)
{
    LOGI("Starting...\n");

    CansatBLE::init();

    if (SDStore.init() != 0) {
        LOGI("Store init failed\n");
        SDStore.deinit();
        return 1;
    }

    Sensors::BMP280_1.start(100);
    // BMP280_2.start(500);

    Sensors::MS5611.start(100);
    
    // // // DoubleSHT31.start(1000);
    Sensors::SHT31_1.start(100);
    Sensors::SHT31_2.start(100);

    // Sensors::gps.start(0);

    SDStore.schedule(&Sensors::MS5611, 500);
    SDStore.schedule(&Sensors::BMP280_1, 500);
    SDStore.schedule(&Sensors::SHT31_1, 500);
    SDStore.schedule(&Sensors::SHT31_2, 500);

    packetgen_thread.start(packetGenerator);

    return 0;
}