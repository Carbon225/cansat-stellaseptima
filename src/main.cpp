#include "mbed.h"

#include "TripleBaro.h"
#include "DoubleTemp.h"
#include "GPSSensor.h"

#include "InternalDataStore.h"
#include "CopyData.h"
#include "DataToPC.h"
#include "Partitions.h"
#include "USBDrive.h"
#include "ConfigManager.h"

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

Parachute parachute(&Sensors::baro, MBED_CONF_APP_MOTOR_PIN_L, MBED_CONF_APP_MOTOR_PIN_H);

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
        TMRData *pressureData = (TMRData*) Sensors::baro.lastValue();
        DoubleTempData *shtData = (DoubleTempData*) Sensors::sht.lastValue();
        GPSData *gpsData = (GPSData*) Sensors::gps.lastValue();

        if (CansatBLE::Instance().sensors()) {
            CansatBLE::Instance().sensors()->setMS(pressureData->pressure1);
            CansatBLE::Instance().sensors()->setBMP1(pressureData->pressure2);
            CansatBLE::Instance().sensors()->setBMP2(pressureData->pressure3);

            CansatBLE::Instance().sensors()->setSHT1(shtData->temp1, shtData->hum1);
            CansatBLE::Instance().sensors()->setSHT2(shtData->temp2, shtData->hum2);

            CansatBLE::Instance().sensors()->setGPS(gpsData->lat, gpsData->lng);
        }


        if (shtData->valid()) {
            LOGI("Temp1 %.2f Hum1 %.1f\n", shtData->temp1, shtData->hum1);
            LOGI("Temp2 %.2f Hum2 %.1f\n", shtData->temp2, shtData->hum2);
            LOGB("Temp %.2f Hum %.1f\n\r", shtData->temperature, shtData->humidity);
        }
        else {
            LOGB("SHT data invalid\n\r");
        }


        if (pressureData->valid()) {
            LOGI("Press1 %.2f\n", pressureData->pressure1);
            LOGI("Press2 %.2f\n", pressureData->pressure2);
            LOGI("Press3 %.2f\n", pressureData->pressure3);
            LOGB("Press %.2f\n\r", pressureData->pressure);
        }
        else {
            LOGB("Pressure data invalid\n\r");
        }


        if (gpsData->valid()) {
            LOGB("lat: %.4f lng: %.4f\n\r", gpsData->lat, gpsData->lng);
        }
        else {
           LOGB("GPS data invalid\n\r");
        }


        switch (parachute.state()) {
            case ParachuteState::Ascending:
            LOGB("Parachute mode: Ascending\n\r");
            break;

            case ParachuteState::Descending:
            LOGB("Parachute mode: Descending\n\r");
            break;

            case ParachuteState::Opening:
            LOGB("Parachute mode: Opening\n\r");
            break;

            case ParachuteState::Done:
            LOGB("Parachute mode: Done\n\r");
            break;
        }

        static int packetID = 0;
        packet_t packet;

        if ((packetID % 4) == 0) {
            // packet = new GPSPacket(packetID++, gpsData->lat, gpsData->lng);
            packet = GPSPacket::encode(packetID++, gpsData->lat, gpsData->lng);
        }
        else {
            // packet = new SensorPacket(packetID++, shtData->temperature, pressureData->pressure);
            packet = SensorPacket::encode(packetID++, shtData->temperature, pressureData->pressure, shtData->humidity);
        }

        radio.beginPacket(4);
        radio.write((uint8_t*)&packet, 4);
        radio.endPacket(true);

        LOGI("%#x\n", packet);

        // delete packet;

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
        // USBDrive::prepareFS();
        disableUSBSerial();
        USBDrive::connect();
    }
    
    LOGI("Starting...\n");

    LOGI("Starting BLE\n");
    CansatBLE::init();

    LOGI("Reading configuration\n");
    if (ConfigManager::Instance().init() != MBED_SUCCESS) {
        LOGI("Error initializing configuration\n");
    }

    if (ConfigManager::Instance().readConfig() != MBED_SUCCESS) {
        LOGI("Reading config failed\n");
    }

    const long freq = ConfigManager::Instance().getLoraFreq();
    const int txPower = 20;
    const int sf = 8;
    const long sbw = 31.25E3;
    const int crd = 8;

    while (!radio.begin(freq)) {
        LOGI("Starting Lora failed\n");
        ThisThread::sleep_for(500);
    }

    // radio.setTxPower(txPower);
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

    Sensors::gps.start(0);
    Sensors::baro.start(125);
    ThisThread::sleep_for(100);
    Sensors::sht.start(500);

    parachute.setGroundPressure(ConfigManager::Instance().getGroundPressure());
    parachute.start();

    // internalFlash.schedule(&Sensors::baro, 2000);
    // internalFlash.schedule(&Sensors::sht, 2000);
    // internalFlash.schedule(&Sensors::gps, 2000);

    radio.beginPacket(4);
    radio.write(0xff);
    radio.write(0xff);
    radio.write(0xff);
    radio.write(0xff);
    radio.endPacket(false);

    packetgen_thread.start(packetGenerator);

    LOGI("\nSYSTEM READY\n\n");

    return 0;
}