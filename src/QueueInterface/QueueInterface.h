#ifndef _QUEUEINTERFACE_H_
#define _QUEUEINTERFACE_H_

#import "mbed.h"

template <typename T>
class QueueInterface {
public:
    virtual bool empty() = 0;
    virtual bool full() = 0;
    
    virtual osStatus put(T *data, uint32_t millisec = 0, uint8_t prio = 0) = 0;
    virtual osEvent get(uint32_t millisec = osWaitForever) = 0;
};

#endif