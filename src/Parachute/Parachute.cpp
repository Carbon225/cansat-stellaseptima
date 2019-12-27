#include "Parachute.h"
#include "PressureData.h"

#define AVG_SIZE 8
#define MOTOR_ON true

Parachute::Parachute(Sensor *baro, PinName motor)
: _baro(baro), _motor(motor, !MOTOR_ON), _state(ParachuteState::Ascending)
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

void Parachute::start()
{
    _ticker.attach(callback(this, &Parachute::_parachuteTask), 0.5);
}

void Parachute::_parachuteTask()
{
    static double minPressure = -1.0;

    static double p[AVG_SIZE] = {0.0};
    static int oldest = 0;
    static double avgSum = 0.0;

    PressureData *data = (PressureData*) _baro->lastValue();

    if (!data->valid()) {
        return;
    }

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
    _motor = MOTOR_ON;
}

void Parachute::_motorOff()
{
    _motor = !MOTOR_ON;
}

void Parachute::open()
{
    _motorOn();
    _motorTimeout.attach(callback(this, &Parachute::_motorOff), MBED_CONF_APP_MOTOR_ON_TIME);
}