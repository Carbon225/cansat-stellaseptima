#ifndef _SIZEDQUEUE_H_
#define _SIZEDQUEUE_H_

#include "mbed.h"
#include "QueueInterface.h"

template <typename T, int SIZE>
class SizedQueue : public QueueInterface<T>, private Queue<T, SIZE> {
public:
    virtual bool empty() {
        return Queue<T, SIZE>::empty();
    }
    virtual bool full() {
        return Queue<T, SIZE>::full();
    }
    
    virtual osStatus put(T *data, uint32_t millisec = 0, uint8_t prio = 0) {
        return Queue<T, SIZE>::put(data);
    };

    virtual osEvent get(uint32_t millisec = osWaitForever) {
        return Queue<T, SIZE>::get(millisec);
    }
};
#endif