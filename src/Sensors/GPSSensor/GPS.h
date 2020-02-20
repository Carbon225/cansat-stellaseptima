#ifndef _GPS_H_
#define _GPS_H_

#include "mbed.h"
#include "DataTypes.h"
#include "minmea.h"

#define GPS_BUF_SIZE 256

class GPS
{
public:
    GPS(PinName rx, PinName pps = NC);
    virtual ~GPS();

    void begin();
    void read();

    minmea_sentence_gll GLL();
    minmea_sentence_zda ZDA();

private:
    minmea_sentence_gll _lastGLL;
    minmea_sentence_zda _lastZDA;

    volatile char _buf[GPS_BUF_SIZE];
    volatile int _inputPos;

    UARTSerial _serial;
    Event<void(int)> _sentenceEvent;
    InterruptIn _pps;

    static EventQueue _evQueue;
    static Thread _thread;

    void _ppsIrq();
    void _rxIrq();
    void _processSentence(int sentenceStart);
};

#endif