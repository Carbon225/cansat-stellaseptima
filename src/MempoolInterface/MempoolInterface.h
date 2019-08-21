#ifndef _MEMPOOLINTERFACE_H_
#define _MEMPOOLINTERFACE_H_

#import "mbed.h"

template <typename T>
class MempoolInterface {
public:
    virtual T* alloc() = 0;
    virtual T* alloc_for (uint32_t millisec) = 0;
    virtual T* alloc_until (uint64_t millisec) = 0;
    
    virtual T* calloc (void) = 0;
    virtual T* calloc_for (uint32_t millisec) = 0;
    virtual T* calloc_until (uint64_t millisec) = 0;
    
    virtual osStatus free(T *block) = 0;
};

#endif