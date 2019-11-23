#ifndef _GPS_H_
#define _GPS_H_

#include "mbed.h"

#define GPS_BUF_SIZE 512

class GPS
{
public:
    GPS(PinName rx);
    virtual ~GPS();

    void start();

private:
    volatile char _buf[GPS_BUF_SIZE];
    volatile int _inputPos;
    volatile int _decoderPos;

    RawSerial _serial;
    EventQueue _evQueue;
    Event<void()> _sentenceEvent;
    InterruptIn _pps;

    void _ppsIrq();
    void _rxIrq();
    void _readSentence();
    void _processSentence();
    bool _empty();
};

#endif