#ifndef _CANSATBLE_H_
#define _CANSATBLE_H_

#include "mbed.h"
#include "events/mbed_events.h"
#include "ble/BLE.h"
#include "UARTService.h"
#include "LEDService.h"

class CansatBLE : ble::Gap::EventHandler, NonCopyable<CansatBLE> {
public:
    CansatBLE();
    ~CansatBLE();

    static void init();
    static CansatBLE& Instance();

    void start();
    UARTService* uart();

private:
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params);
    void start_advertising();
    void on_data_written(const GattWriteCallbackParams *params);
    void blink();
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);

    static BLE &_ble;

    DigitalOut _alive_led;
    DigitalOut _actuated_led;
 
    LEDService *_led_service;
    UARTService *_uart_service;
 
    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;


    static EventQueue _event_queue;
    static Thread _ev_thread;
    static void _schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);
};

#endif