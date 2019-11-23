#include "GPS.h"
#include "BLELogger.h"

GPS::GPS(PinName rx)
: _serial(NC, rx),
  _pps(MBED_CONF_APP_GPS_PPS), 
  _evQueue(32 * EVENTS_EVENT_SIZE),
  _sentenceEvent(_evQueue.event(callback(this, &GPS::_processSentence))),
  _inputPos(0), _decoderPos(0)
{
    _serial.baud(9600);
    _serial.format(8, Serial::None, 1);
}

GPS::~GPS()
{

}

DigitalOut gpsled(LED4);

Thread gpsThread(osPriorityNormal, 1024, NULL, "gps");

void GPS::start()
{
    gpsled = 1;
    
    _serial.attach(callback(this, &GPS::_rxIrq), Serial::RxIrq);
    // _pps.rise(callback(this, &GPS::_ppsIrq));

    gpsThread.start(callback(&_evQueue, &EventQueue::dispatch_forever));
    // _evQueue.dispatch_forever();
}

void GPS::_ppsIrq()
{
    gpsled = 0;
    _evQueue.call(callback(this, &GPS::_readSentence));
}

void GPS::_rxIrq()
{
    do {
        char c = _serial.getc();
        _buf[_inputPos++] = c;
        _inputPos %= GPS_BUF_SIZE - 1;

        if (c == '\r') {
            gpsled = !gpsled;
            // _evQueue.call(callback(this, &GPS::_processSentence));
            _sentenceEvent.post();
        }
    } while (_serial.readable());
}

void GPS::_readSentence()
{
    while (_serial.readable()) {
        char c = _serial.getc();
        _buf[_inputPos++] = c;
        _inputPos %= GPS_BUF_SIZE - 1;

        if (c == '\r') {
            _evQueue.call(callback(this, &GPS::_processSentence));
        }
    }
}

void GPS::_processSentence()
{    
    while (_buf[_decoderPos] != '$') {
        if (_empty()) {
            // puts("Not found\n");
            return;
        }
        _decoderPos++;
        _decoderPos %= GPS_BUF_SIZE;
    }

    // printf("Reading sentence\n");
    static char sentence[16];
    for (int i = 0; i < 6 && !_empty(); i++) {
        sentence[i] = _buf[_decoderPos++];
        _decoderPos %= GPS_BUF_SIZE;
    }
    sentence[6] = '\n';
    sentence[7] = '\0';
    printf("%s\n", sentence);
}

bool GPS::_empty()
{
    return _decoderPos == _inputPos;
}