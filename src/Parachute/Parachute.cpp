#include "Parachute.h"
#include "PressureData.h"
#include "BLELogger.h"

#define AVG_SIZE 0

Parachute::Parachute(Sensor *baro, PinName motorL, PinName motorH)
: _baro(baro), _motorL(motorL, 1), _motorH(motorH, 0), _state(ParachuteState::Ascending)
{

}

Parachute::~Parachute()
{
    _ticker.detach();
}

ParachuteState Parachute::state()
{
    return _state;
}

void Parachute::setGroundPressure(double pressure)
{
    _openingPressure = pressure - _pressureOffset;
}

void Parachute::start()
{
    _ticker.attach(callback(this, &Parachute::_parachuteTask), 0.2);
}

void Parachute::_parachuteTask()
{
    PressureData *data = (PressureData*) _baro->lastValue();
    if (!data->valid()) {
        return;
    }

    static double minPressure = -1.0;

    #if AVG_SIZE > 0
        static double p[AVG_SIZE] = {0.0};
        static int oldest = 0;
        static double avgSum = 0.0;

        if (minPressure < 0) {
            minPressure = data->pressure;
            for (int i = 0; i < AVG_SIZE; i++) {
                p[i] = minPressure;
            }
            avgSum = minPressure * AVG_SIZE;
        }
        
        avgSum += data->pressure - p[oldest];
        p[oldest++] = data->pressure;
        oldest %= AVG_SIZE;

        double pressure = avgSum / (double) AVG_SIZE;
    #else
        double pressure = data->pressure;
        
        static double lastPressure = -1.0;
        if (lastPressure < 0.0) {
            lastPressure = pressure;
        }

        if (minPressure < 0.0) {
            minPressure = pressure;
        }

        if (abs(pressure - lastPressure) > 5.0) {
            lastPressure = pressure;
            return;
        }
        lastPressure = pressure;
        
    #endif

    switch (_state) {
        case ParachuteState::Ascending:
        if (pressure - minPressure > _descentThreshold) {
            _state = ParachuteState::Descending;
        }
        break;

        case ParachuteState::Descending:
        if (pressure < minPressure) {
            _state = ParachuteState::Ascending;
        }
        else if (pressure > _openingPressure) {
            _state = ParachuteState::Opening;
        }
        break;

        case ParachuteState::Opening:
        open();
        _state = ParachuteState::Done;
        break;

        case ParachuteState::Done:
        _ticker.detach();
        break;
    }

    if (pressure < minPressure) {
        minPressure = pressure;
    }
}

void Parachute::_motorOn()
{
    _motorL = 0;
    _motorH = 1;
}

void Parachute::_motorOff()
{
    _motorL = 1;
    _motorH = 0;
}

void Parachute::open()
{
    _motorOn();
    _motorTimeout.attach(callback(this, &Parachute::_motorOff), MBED_CONF_APP_MOTOR_ON_TIME);
}