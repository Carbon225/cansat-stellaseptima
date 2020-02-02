#ifndef _BLELOGGER_H_
#define _BLELOGGER_H_

#include "mbed.h"

void disableUSBSerial();

#define LOGI(f_, ...) printf((f_), ##__VA_ARGS__)
void LOGB(const char *fmt, ...);

#endif