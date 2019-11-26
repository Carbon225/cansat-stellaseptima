#include "GPS.h"
#include "BLELogger.h"
#include "minmea.h"

Thread GPS::_thread(osPriorityNormal, 1024, NULL, "nmea");
EventQueue GPS::_evQueue(16 * EVENTS_EVENT_SIZE);

GPS::GPS(PinName rx, PinName pps)
: _serial(NC, rx),
  _pps(pps),
  _sentenceEvent(_evQueue.event(callback(this, &GPS::_processSentence))),
  _inputPos(0)
{
    _serial.baud(115200);
    _serial.format(8, Serial::None, 1);
}

GPS::~GPS()
{

}

minmea_sentence_gll GPS::GLL()
{
    return _lastGLL;
}

minmea_sentence_zda GPS::ZDA()
{
    return _lastZDA;
}

void GPS::begin()
{
    if (_thread.get_state() == Thread::Deleted) {
        _thread.start(callback(&GPS::_evQueue, &EventQueue::dispatch_forever));
    }
    
    _serial.attach(callback(this, &GPS::_rxIrq), Serial::RxIrq);
    // _pps.rise(callback(this, &GPS::_ppsIrq));
}

void GPS::_ppsIrq()
{
    
}

void GPS::_rxIrq()
{
    static int sentenceStart = -1;
    do {
        char c = _serial.getc();
        
        if (c == '$') {
            sentenceStart = _inputPos;
        }

        _buf[_inputPos++] = c;
        _inputPos %= GPS_BUF_SIZE;

        if (c == '\r' && sentenceStart != -1) {
            _sentenceEvent.post(sentenceStart);
            sentenceStart = -1;
        }
    } while (_serial.readable());
}

void GPS::_processSentence(int sentenceStart)
{    
    LOGI("Sentence start: %d\n", sentenceStart);

    static char sentence[128];
    int i = 0;
    
    while (_buf[sentenceStart] != '\r' && sentenceStart != _inputPos) {
        sentence[i++] = _buf[sentenceStart++];
        sentenceStart %= GPS_BUF_SIZE;
    }
    
    sentence[i] = '\0';
    LOGI("%s\n", sentence);

    LOGI("Max stack: %u\n", _thread.max_stack());

    // switch (minmea_sentence_id(sentence, false)) {
    //     case MINMEA_SENTENCE_GLL: {
    //         minmea_sentence_gll frame;
    //         if (minmea_parse_gll(&frame, sentence)) {
    //             _lastGLL = frame;
    //             LOGI("lat: %d/%d, lng: %d/%d\n",
    //                 frame.latitude.value, frame.latitude.scale,
    //                 frame.longitude.value, frame.longitude.scale
    //             );
    //         }
    //         else {
    //             LOGI("GLL sentence invalid\n");
    //         }
    //     }
    //     break;

    //     case MINMEA_SENTENCE_ZDA: {
    //         minmea_sentence_zda frame;
    //         if (minmea_parse_zda(&frame, sentence)) {
    //             _lastZDA = frame;
    //             LOGI("Time: %d:%d\n",
    //                 frame.time.minutes,
    //                 frame.time.seconds
    //             );
    //         }
    //         else {
    //             LOGI("ZDA sentence invalid\n");
    //         }
    //     }
    //     break;

    //     default:
    //     break;
    // }
}
