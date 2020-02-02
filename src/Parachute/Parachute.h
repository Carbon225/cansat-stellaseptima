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
    Parachute(Sensor *baro, PinName motorL, PinName motorH);
    ~Parachute();

    void start();
    void open();
    ParachuteState state();

    void setGroundPressure(double pressure);

private:
    void _parachuteTask();
    void _motorOn();
    void _motorOff();

    const double _pressureOffset = 15.0;
    volatile double _descentThreshold = 15.0;
    volatile double _openingPressure = 900.0;

    volatile ParachuteState _state;

    LowPowerTicker _ticker;
    Sensor *_baro;
    DigitalOut _motorL;
    DigitalOut _motorH;
    LowPowerTimeout _motorTimeout;
};

#endif