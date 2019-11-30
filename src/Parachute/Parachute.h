#ifndef _PARACHUTE_H_
#define _PARACHUTE_H_

#include "mbed.h"
#include "Sensor.h"

enum ParachuteState {
    Ascending,
    Descending,
    Opening,
    Done
};

class Parachute
{
public:
    Parachute(Sensor *baro, PinName motor);
    ~Parachute();

    void start();
    void open();
    ParachuteState state();

private:
    void _parachuteTask();
    void _motorOn();
    void _motorOff();

    volatile double _descentThreshold = 50.0;
    volatile double _openingPressure = 900.0;

    volatile ParachuteState _state;

    LowPowerTicker _ticker;
    Sensor *_baro;
    DigitalOut _motor;
    LowPowerTimeout _motorTimeout;
};

#endif