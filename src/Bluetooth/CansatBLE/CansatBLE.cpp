#include "CansatBLE.h"
#include "BLELogger.h"

static CansatBLE BLEController;

void CansatBLE::_schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
    _event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

void CansatBLE::init()
{
    // TODO should (probably) check if thread is already running
    if (_ev_thread.get_state() == Thread::Deleted) {
        _ev_thread.start(callback(&_event_queue, &EventQueue::dispatch_forever));
    }

    _ble.onEventsToProcess(_schedule_ble_events);
    BLEController.start();
}

CansatBLE& CansatBLE::Instance()
{
    return BLEController;
}

EventQueue CansatBLE::_event_queue(16 * EVENTS_EVENT_SIZE);
Thread CansatBLE::_ev_thread(osPriorityBelowNormal, 1024, NULL, "ble");
BLE& CansatBLE::_ble(BLE::Instance());

CansatBLE::CansatBLE()
:   _alive_led(MBED_CONF_APP_LED2),
    _actuated_led(MBED_CONF_APP_LED3),
    _adv_data_builder(_adv_buffer),
    _led_service(NULL),
    _uart_service(NULL)
{
}

CansatBLE::~CansatBLE()
{
    delete _led_service;
}

void CansatBLE::start()
{
    _ble.gap().setEventHandler(this);

    _ble.init(this, &CansatBLE::on_init_complete);

    _event_queue.call_every(500, this, &CansatBLE::blink);
}

UARTService* CansatBLE::uart()
{
    if (!_uart_service) {
        
    }

    return _uart_service;
}

void CansatBLE::on_init_complete(BLE::InitializationCompleteCallbackContext *params)
{
    if (params->error != BLE_ERROR_NONE) {
        LOGI("Ble initialization failed.");
        return;
    }

    _led_service = new LEDService(_ble, false);
    _uart_service = new UARTService(_ble);

    _ble.gattServer().onDataWritten(this, &CansatBLE::on_data_written);

    // print_mac_address();

    start_advertising();
}

void CansatBLE::start_advertising()
{
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
    _adv_data_builder.setName(MBED_CONF_APP_BLE_NAME);

    ble_error_t error = _ble.gap().setAdvertisingParameters(
        ble::LEGACY_ADVERTISING_HANDLE,
        adv_parameters
    );

    if (error) {
        LOGI("_ble.gap().setAdvertisingParameters() failed\r\n");
        return;
    }

    error = _ble.gap().setAdvertisingPayload(
        ble::LEGACY_ADVERTISING_HANDLE,
        _adv_data_builder.getAdvertisingData()
    );

    if (error) {
        LOGI("_ble.gap().setAdvertisingPayload() failed\r\n");
        return;
    }

    error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error) {
        LOGI("_ble.gap().startAdvertising() failed\r\n");
        return;
    }
}

void CansatBLE::on_data_written(const GattWriteCallbackParams *params)
{
    if ((params->handle == _led_service->getValueHandle()) && (params->len == 1)) {
        if (_actuated_led.is_connected())
            _actuated_led = *(params->data);
    }
}

void CansatBLE::blink()
{
    if (_alive_led.is_connected())
        _alive_led = !_alive_led;
}

void CansatBLE::onDisconnectionComplete(const ble::DisconnectionCompleteEvent&)
{
    _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
}