#ifndef _CANSATBLE_H_
#define _CANSATBLE_H_

#include "mbed.h"
#include "events/mbed_events.h"
#include "ble/BLE.h"
#include "UARTService.h"
#include "SensorService.h"
#include "ParachuteService.h"

class CansatBLE : ble::Gap::EventHandler, NonCopyable<CansatBLE> {
public:
    CansatBLE();
    ~CansatBLE();

    static void init();
    static CansatBLE& Instance();

    void start();
    UARTService* uart();
    SensorService* sensors();

private:
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params);
    void start_advertising();
    void on_data_written(const GattWriteCallbackParams *params);
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);
 
    UARTService *_uartService;
    SensorService *_sensorService;
    ParachuteService *_parachuteService;
 
    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;

    static BLE &_ble;
    static EventQueue _event_queue;
    static Thread _ev_thread;
    static void _schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);
};

#endif