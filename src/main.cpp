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

#include "Radio.h"
#include "SensorPacket.h"
#include "GPSPacket.h"

#include "CansatBLE.h"
#include "BLELogger.h"

Thread packetgen_thread(osPriorityNormal, 1024, NULL, "packetgen");

SizedQueue<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioQueue;
SizedMempool<RadioPacket, MBED_CONF_APP_RADIO_QUEUE_SIZE> radioMempool;

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
GPS gps2(MBED_CONF_APP_GPS_RX);

SDDataStore SDStore("/sd");

void packetGenerator()
{
    while (true) {
        LOGI("\n");

        PressureData *msData = (PressureData*) MS5611.lastValue();
        
        SHT31Data *shtData1 = (SHT31Data*) SHT31_1.lastValue();
        SHT31Data *shtData2 = (SHT31Data*) SHT31_2.lastValue();
        
        PressureData *bmpData1 = (PressureData*) BMP280_1.lastValue();
        // PressureData *bmpData2 = (PressureData*) BMP280_2.lastValue();

        GPSData *gpsData = (GPSData*) gps.lastValue();

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

    BMP280_1.start(100);
    // BMP280_2.start(500);

    MS5611.start(100);
    
    // DoubleSHT31.start(1000);
    SHT31_1.start(100);
    SHT31_2.start(100);

    // gps.start(1000);

    SDStore.schedule(&MS5611, 500);
    SDStore.schedule(&BMP280_1, 500);
    SDStore.schedule(&SHT31_1, 500);
    SDStore.schedule(&SHT31_2, 500);

    packetgen_thread.start(packetGenerator);

    gps2.begin();

    while (true) {
        LOGI("\nalive\n");
        ThisThread::sleep_for(1000);
    }

    return 0;
}