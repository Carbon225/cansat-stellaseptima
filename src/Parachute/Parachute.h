#ifndef _PARACHUTE_H_
#define _PARACHUTE_H_

#include "mbed.h"
#include "Sensor.h"

#define MBPERMETER 0.1145043

enum ParachuteState {
    Idle,
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

    const double _maxPressureChange = 100.0 * MBPERMETER;

    const double _openingOffset = 100.0 * MBPERMETER;
    const double _ascentOffset = 50.0 * MBPERMETER;
    const double _descentOffset = 50.0 * MBPERMETER;

    volatile double _ascentPressure = 970.0;
    volatile double _openingPressure = 1000.0;

    volatile ParachuteState _state;

    LowPowerTicker _ticker;
    Sensor *_baro;
    DigitalOut _motorL;
    DigitalOut _motorH;
    LowPowerTimeout _motorTimeout;
};

#endif