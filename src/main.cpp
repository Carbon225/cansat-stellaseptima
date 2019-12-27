#include "mbed.h"

#include "TripleBaro.h"
#include "DoubleTemp.h"
#include "GPSSensor.h"

#include "InternalDataStore.h"
#include "CopyData.h"
#include "Partitions.h"
#include "USBDrive.h"

#include "Parachute.h"
#include "Radio.h"
#include "SensorPacket.h"
#include "GPSPacket.h"

#include "CansatBLE.h"
#include "BLELogger.h"

Thread packetgen_thread(osPriorityNormal, 1024, NULL, "packetgen");

DigitalIn usbButton(MBED_CONF_APP_BUTTON1, PinMode::PullUp);

namespace Sensors
{
    TripleBaro baro(
        "baro",
        MBED_CONF_APP_I2C1_SDA,
        MBED_CONF_APP_I2C1_SCL,
        MBED_CONF_APP_I2C2_SDA,
        MBED_CONF_APP_I2C2_SCL
    );

    DoubleTemp sht(
        "sht",
        MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL,
        MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL
    );

    GPSSensor gps("gps", MBED_CONF_APP_GPS_RX, MBED_CONF_APP_GPS_PPS);
}

InternalDataStore internalFlash("/int");

Parachute parachute(&Sensors::baro, MBED_CONF_APP_MOTOR_PIN);

Radio radio(
    MBED_CONF_APP_SPI1_MOSI,
    MBED_CONF_APP_SPI1_MISO,
    MBED_CONF_APP_SPI1_CLK,
    MBED_CONF_APP_LORA_CS,
    MBED_CONF_APP_LORA_RST,
    MBED_CONF_APP_LORA_DIO0
);

void packetGenerator()
{
    while (true) {
        PressureData *pressureData = (PressureData*) Sensors::baro.lastValue();
        SHT31Data *shtData = (SHT31Data*) Sensors::sht.lastValue();
        GPSData *gpsData = (GPSData*) Sensors::gps.lastValue();


        if (shtData->valid())
            LOGI("Temp %.2f Hum %.1f\n", shtData->temperature, shtData->humidity);
        else
            LOGI("SHT data invalid\n");


        if (pressureData->valid())
            LOGI("Press %.2f\n", pressureData->pressure);
        else
            LOGI("Pressure data invalid\n");


        // if (gpsData->valid())
        //     LOGI("lat: %.4f lng: %.4f\n", gpsData->lat, gpsData->lng);
        // else
        //    LOGI("GPS data invalid\n");


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

        static int packetID = 0;
        static RadioPacket packet;

        if ((packetID % 4) == 0) {
            new(&packet) GPSPacket(packetID++, 50.069082, 19.943569);
        }
        else {
            new(&packet) SensorPacket(packetID++, shtData->temperature, pressureData->pressure);
        }

        radio.beginPacket(8);
        radio.write((uint8_t*)packet.toBinary(), 4);
        radio.write(0xff);
        radio.write(0xff);
        radio.write(0xff);
        radio.write(0xff);
        radio.endPacket(true);

        LOGI("%#x\n", *packet.toBinary());

        // LOGI("---STATS---\n");

        // mbed_stats_heap_t heap_stats;
        // mbed_stats_heap_get(&heap_stats);
        // LOGI("Reserved heap: %u\n", heap_stats.reserved_size);
        // LOGI("Current heap: %u\n", heap_stats.current_size);
        // LOGI("Max heap size: %u\n", heap_stats.max_size);

        // int cnt = osThreadGetCount();
        // mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

        // cnt = mbed_stats_stack_get_each(stats, cnt);
        // for (int i = 0; i < cnt; i++) {
        //     LOGI("Thread: 0x%X, Stack size: %u, Max stack: %u\n", 
        //             stats[i].thread_id, stats[i].reserved_size, stats[i].max_size);
        // }

        // free(stats);

        // LOGI("---STATS---\n");

        ThisThread::sleep_for(1000);
    }
}


int main(void)
{
    Partitions::initialize();
    
    if (usbButton == 0) {
        USBDrive::prepareFS();
        disableUSBSerial();
        USBDrive::connect();
    }
    
    LOGI("Starting...\n");

    CansatBLE::init();

    int txPower = 20;
    int sf = 8;
    long sbw = 31.25E3;
    int crd = 8;

    while (!radio.begin(4346E5)) {
        LOGI("Starting Lora failed\n");
        ThisThread::sleep_for(500);
    }

    radio.setTxPower(txPower);
    radio.setSpreadingFactor(sf);
    radio.setSignalBandwidth(sbw);
    radio.setCodingRate4(crd);

    LOGI("Lora started\n");

    if (internalFlash.init() != MBED_SUCCESS) {
        LOGI("Internal flash init failed\n");
        internalFlash.deinit();
        return 1;
    }
    
    internalFlash.listFiles();

    // Sensors::gps.start(0);
    Sensors::baro.start(200);
    ThisThread::sleep_for(100);
    Sensors::sht.start(200);

    parachute.start();

    internalFlash.schedule(&Sensors::baro, 2000);
    internalFlash.schedule(&Sensors::sht, 2000);
    // internalFlash.schedule(&Sensors::gps, 2000);

    packetgen_thread.start(packetGenerator);

    LOGI("\nSYSTEM READY\n\n");

    return 0;
}