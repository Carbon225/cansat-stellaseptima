#ifndef _SIZEDMEMPOOL_H_
#define _SIZEDMEMPOOL_H_

#include "mbed.h"
#include "MempoolInterface.h"

template <typename T, int SIZE>
class SizedMempool : public MempoolInterface<T>, private MemoryPool<T, SIZE> {
public:
    virtual T* alloc(){
        return MemoryPool<T, SIZE>::alloc();
    }

    virtual T* alloc_for (uint32_t millisec) {
        return MemoryPool<T, SIZE>::alloc_for(millisec);
    }

    virtual T* alloc_until (uint64_t millisec) {
        return MemoryPool<T, SIZE>::alloc_until(millisec);
    }
    
    
    virtual T* calloc () {
        return MemoryPool<T, SIZE>::calloc();
    }
    
    virtual T* calloc_for (uint32_t millisec) {
        return MemoryPool<T, SIZE>::calloc_for(millisec);
    }
    
    virtual T* calloc_until (uint64_t millisec) {
        return MemoryPool<T, SIZE>::calloc_until(millisec);
    }
    
    virtual osStatus free(T *block) {
        return MemoryPool<T, SIZE>::free(block);
    }
};
#endif