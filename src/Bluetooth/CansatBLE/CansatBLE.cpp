#include "CansatBLE.h"
#include "BLELogger.h"

const static char DEVICE_NAME[] = MBED_CONF_APP_BLE_NAME;
static CansatBLE BLEController;

void CansatBLE::_schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
    _event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

void CansatBLE::init()
{
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
:   _adv_data_builder(_adv_buffer),
    _uartService(NULL),
    _sensorService(NULL),
    _parachuteService(NULL),
    _configService(NULL)
{
}

CansatBLE::~CansatBLE()
{
    delete _uartService;
    delete _sensorService;
    delete _parachuteService;
    delete _configService;
}

void CansatBLE::start()
{
    _ble.gap().setEventHandler(this);

    _ble.init(this, &CansatBLE::on_init_complete);
}

UARTService* CansatBLE::uart()
{
    return _uartService;
}

SensorService* CansatBLE::sensors()
{
    return _sensorService;
}

ConfigService* CansatBLE::configService()
{
    return _configService;
}

void CansatBLE::on_init_complete(BLE::InitializationCompleteCallbackContext *params)
{
    if (params->error != BLE_ERROR_NONE) {
        LOGI("Ble initialization failed.");
        return;
    }

    _uartService = new UARTService(_ble);
    _sensorService = new SensorService(_ble);
    _parachuteService = new ParachuteService(_ble, 900.f, 50.f);
    _configService = new ConfigService(_ble);

    _ble.gattServer().onDataWritten(this, &CansatBLE::on_data_written);

    start_advertising();
}

void CansatBLE::start_advertising()
{
    ble::AdvertisingParameters adv_parameters(
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t(ble::millisecond_t(1000))
    );

    static const UUID services[] = {
        UARTServiceUUID,
        SensorService::SERVICE_UUID,
        ParachuteService::SERVICE_UUID,
        ConfigService::SERVICE_UUID
    };

    _adv_data_builder.setFlags();
    _adv_data_builder.setAppearance(ble::adv_data_appearance_t::GENERIC_COMPUTER);
    _adv_data_builder.setLocalServiceList(mbed::make_Span(services, 4));
    _adv_data_builder.setName(DEVICE_NAME);

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
    _parachuteService->onData(params);
    _configService->onData(params);
    // if ((params->handle == _parachuteService->getValueHandle()) && (params->len == 1)) {
    //     if (_actuated_led.is_connected())
    //         _actuated_led = *(params->data);
    // }
}

void CansatBLE::onDisconnectionComplete(const ble::DisconnectionCompleteEvent&)
{
    _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
}