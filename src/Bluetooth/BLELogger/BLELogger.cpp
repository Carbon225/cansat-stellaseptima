#include "BLELogger.h"

void LOGI(const char *fmt, ...)
{
    
    va_list args;
    va_start(args, fmt);

    if (vprintf(fmt, args) < 64) {
        static char buf[64];
        memset(buf, 0, 64);
        vsprintf(buf, fmt, args);
        
        if (BLEController.uart())
            BLEController.uart()->write(buf, 20);
    }

    va_end(args);
}