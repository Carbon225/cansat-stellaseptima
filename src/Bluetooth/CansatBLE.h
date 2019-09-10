#include "mbed.h"
#include "events/mbed_events.h"
#include "ble/BLE.h"
#include "UARTService.h"
#include "LEDService.h"

class CansatBLE : ble::Gap::EventHandler {
public:
    CansatBLE(BLE &ble, events::EventQueue &ev_queue)
    :   _ble(ble),
        _alive_led(MBED_CONF_APP_LED2),
        _actuated_led(MBED_CONF_APP_LED3),
        _ev_queue(ev_queue),
        _adv_data_builder(_adv_buffer),
        _led_service(NULL),
        _uart_service(NULL)
    {}

    ~CansatBLE() {
        delete _led_service;
    }

    void start() {
        _ble.gap().setEventHandler(this);
 
        _ble.init(this, &CansatBLE::on_init_complete);
 
        _ev_queue.call_every(500, this, &CansatBLE::blink);
    }

    UARTService* uart()
    {
        return _uart_service;
    }

private:
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params) {
        if (params->error != BLE_ERROR_NONE) {
            printf("Ble initialization failed.");
            return;
        }
 
        _led_service = new LEDService(_ble, false);
        _uart_service = new UARTService(_ble);
 
        _ble.gattServer().onDataWritten(this, &CansatBLE::on_data_written);
 
        // print_mac_address();
 
        start_advertising();
    }
 
    void start_advertising() {
        /* Create advertising parameters and payload */
 
        ble::AdvertisingParameters adv_parameters(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(1000))
        );

        static const UUID services[2] = {
            LEDService::LED_SERVICE_UUID,
            UARTServiceUUID
        };
 
        _adv_data_builder.setFlags();
        _adv_data_builder.setLocalServiceList(mbed::make_Span(services, 2));
        _adv_data_builder.setName("CansatDK");
 
        ble_error_t error = _ble.gap().setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            adv_parameters
        );
 
        if (error) {
            printf("_ble.gap().setAdvertisingParameters() failed\r\n");
            return;
        }
 
        error = _ble.gap().setAdvertisingPayload(
            ble::LEGACY_ADVERTISING_HANDLE,
            _adv_data_builder.getAdvertisingData()
        );
 
        if (error) {
            printf("_ble.gap().setAdvertisingPayload() failed\r\n");
            return;
        }
 
        error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
 
        if (error) {
            printf("_ble.gap().startAdvertising() failed\r\n");
            return;
        }
    }
 
    void on_data_written(const GattWriteCallbackParams *params) {
        if ((params->handle == _led_service->getValueHandle()) && (params->len == 1)) {
            if (_actuated_led.is_connected())
                _actuated_led = *(params->data);
        }
    }
 
    void blink() {
        if (_alive_led.is_connected())
            _alive_led = !_alive_led;
    }
 
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {
        _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
    }

    BLE &_ble;
    events::EventQueue &_ev_queue;
    DigitalOut _alive_led;
    DigitalOut _actuated_led;
 
    LEDService *_led_service;
    UARTService *_uart_service;
 
    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;
};