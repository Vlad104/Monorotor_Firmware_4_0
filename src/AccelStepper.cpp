#include "AccelStepper.h"

void AccelStepper::moveTo(long absolute)
{
    if (_targetPos != absolute)
    {
        _targetPos = absolute;
        computeNewSpeed();
    }
}

void AccelStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
bool AccelStepper::runSpeed()
{
    extern Timer t;
    // Dont do anything unless we actually have a step interval
    if (!_stepInterval)
        return false;

    //unsigned long time = micros();
    unsigned long time = t.read_us();
    unsigned long nextStepTime = _lastStepTime + _stepInterval;
    // Gymnastics to detect wrapping of either the nextStepTime and/or the current time
    if (((nextStepTime >= _lastStepTime) && ((time >= nextStepTime) || (time < _lastStepTime))) || ((nextStepTime < _lastStepTime) && ((time >= nextStepTime) && (time < _lastStepTime))))
    {
        _currentPos = (_direction == DIRECTION_CW) ? _currentPos + 1 : _currentPos - 1;
        step();

        _lastStepTime = time;
        return true;
    }
    else
    {
        return false;
    }
}

// long AccelStepper::distanceToGo()
// {
//     return _targetPos - _currentPos;
// }

// bool AccelStepper::distanceIsNull() {
//     return _targetPos - _currentPos == 0;
// }

long AccelStepper::targetPosition()
{
    return _targetPos;
}

long AccelStepper::currentPosition()
{
    return _currentPos;
}

// Useful during initialisations or after initial positioning
// Sets speed to 0
void AccelStepper::setCurrentPosition(long position)
{
    _targetPos = _currentPos = position;
    _n = 0;
    _stepInterval = 0;
}

void AccelStepper::computeNewSpeed()
{
    long distanceTo = distanceToGo(); // +ve is clockwise from curent location

    long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration)); // Equation 16

    if (distanceTo == 0 && stepsToStop <= 1)
    {
        // We are at the target and its time to stop
        _stepInterval = 0;
        _speed = 0.0;
        _n = 0;
        return;
    }

    if (distanceTo > 0)
    {
        // We are anticlockwise from the target
        // Need to go clockwise from here, maybe decelerate now
        if (_n > 0)
        {
            // Currently accelerating, need to decel now? Or maybe going the wrong way?
            if ((stepsToStop >= distanceTo) || _direction == DIRECTION_CCW)
                _n = -stepsToStop; // Start deceleration
        }
        else if (_n < 0)
        {
            // Currently decelerating, need to accel again?
            if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW)
                _n = -_n; // Start accceleration
        }
    }
    else if (distanceTo < 0)
    {
        // We are clockwise from the target
        // Need to go anticlockwise from here, maybe decelerate
        if (_n > 0)
        {
            // Currently accelerating, need to decel now? Or maybe going the wrong way?
            if ((stepsToStop >= -distanceTo) || _direction == DIRECTION_CW)
                _n = -stepsToStop; // Start deceleration
        }
        else if (_n < 0)
        {
            // Currently decelerating, need to accel again?
            if ((stepsToStop < -distanceTo) && _direction == DIRECTION_CCW)
                _n = -_n; // Start accceleration
        }
    }

    // Need to accelerate or decelerate
    if (_n == 0)
    {
        // First step from stopped
        _cn = _c0;
        _direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    else
    {
        // Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
        _cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1)); // Equation 13
        _cn = max(_cn, _cmin);
    }
    _n++;
    _stepInterval = _cn;
    _speed = 1000000.0 / _cn;
    if (_direction == DIRECTION_CCW)
        _speed = -_speed;
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if the motor is still running to the target position.
bool AccelStepper::run()
{
    if (runSpeed())
        computeNewSpeed();
    // return _speed != 0.0 || distanceToGo() != 0;
    return _speed != 0.0 || !distanceIsNull();
}

AccelStepper::AccelStepper(PinName pin1, PinName pin2, PinName pin3, PinName pin4, bool enable)
{
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 0.0;
    _sqrt_twoa = 1.0;
    _stepInterval = 0;
    _minPulseWidth = 1;
    _lastStepTime = 0;

    _pin0 = new DigitalOut(pin1);
    _pin1 = new DigitalOut(pin2);
    _pin2 = new DigitalOut(pin3);
    _pin3 = new DigitalOut(pin4);

    // NEW
    _n = 0;
    _c0 = 0.0;
    _cn = 0.0;
    _cmin = 1.0;
    _direction = DIRECTION_CCW;

    int i;
    for (i = 0; i < 4; i++)
        _pinInverted[i] = 0;
    if (enable)
        enableOutputs();
    // Some reasonable default
    setAcceleration(1);
}

void AccelStepper::setMaxSpeed(float speed)
{
    if (_maxSpeed != speed)
    {
        _maxSpeed = speed;
        _cmin = 1000000.0 / speed;
        // Recompute _n from current speed and adjust speed if accelerating or cruising
        if (_n > 0)
        {
            _n = (long)((_speed * _speed) / (2.0 * _acceleration)); // Equation 16
            computeNewSpeed();
        }
    }
}

void AccelStepper::setAcceleration(float acceleration)
{
    if (acceleration == 0.0)
        return;
    if (_acceleration != acceleration)
    {
        // Recompute _n per Equation 17
        _n = _n * (_acceleration / acceleration);
        // New c0 per Equation 7, with correction per Equation 15
        _c0 = 0.676 * sqrt(2.0 / acceleration) * 1000000.0; // Equation 15
        _acceleration = acceleration;
        computeNewSpeed();
    }
}

void AccelStepper::setSpeed(float speed)
{
    if (speed == _speed)
        return;
    speed = constrain(speed, -_maxSpeed, _maxSpeed);
    if (speed == 0.0)
        _stepInterval = 0;
    else
    {
        _stepInterval = fabs(1000000.0 / speed);
        _direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    _speed = speed;
}

float AccelStepper::speed()
{
    return _speed;
}

// You might want to override this to implement eg serial output
// bit 0 of the mask corresponds to _pin[0]
// bit 1 of the mask corresponds to _pin[1]
// ....
void AccelStepper::setOutputPins(uint8_t mask)
{
    *_pin0 = (mask & (1 << 0)) ? (HIGH ^ _pinInverted[0]) : (LOW ^ _pinInverted[0]);
    *_pin1 = (mask & (1 << 1)) ? (HIGH ^ _pinInverted[1]) : (LOW ^ _pinInverted[1]);
}

// 1 pin step function (ie for stepper drivers)
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step()
{
    // _pin[0] is step, _pin[1] is direction
    setOutputPins(_direction ? 0b10 : 0b00); // Set direction first else get rogue pulses
    setOutputPins(_direction ? 0b11 : 0b01); // step HIGH
    // Caution 200ns setup time
    // Delay the minimum allowed pulse width
    //delayMicroseconds(_minPulseWidth);
    wait_us(_minPulseWidth);
    setOutputPins(_direction ? 0b10 : 0b00); // step LOW
}

// Prevents power consumption on the outputs
void AccelStepper::disableOutputs()
{
    setOutputPins(0); // Handles inversion automatically
                      // if (_enablePin != 0xff)
    if (_enablePin)
        *_enablePin = LOW ^ _enableInverted;
}

void AccelStepper::enableOutputs()
{
    // if (_enablePin != 0xff)
    if (_enablePin)
    {
        *_enablePin = HIGH ^ _enableInverted;
    }
}

void AccelStepper::setMinPulseWidth(unsigned int minWidth)
{
    _minPulseWidth = minWidth;
}

// void AccelStepper::setEnablePin(uint8_t enablePin)
void AccelStepper::setEnablePin(PinName enablePin)
{
    // _enablePin = enablePin;
    _enablePin = new DigitalOut(enablePin);

    // This happens after construction, so init pin now.
    // if (_enablePin != 0xff)
    if (*_enablePin)
    {
        //pinMode(_enablePin, OUTPUT);
        //digitalWrite(_enablePin, HIGH ^ _enableInverted);
        *_enablePin = HIGH ^ _enableInverted;
    }
}

void AccelStepper::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert)
{
    _pinInverted[0] = stepInvert;
    _pinInverted[1] = directionInvert;
    _enableInverted = enableInvert;
}

void AccelStepper::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert)
{
    _pinInverted[0] = pin1Invert;
    _pinInverted[1] = pin2Invert;
    _pinInverted[2] = pin3Invert;
    _pinInverted[3] = pin4Invert;
    _enableInverted = enableInvert;
}

// Blocks until the target position is reached and stopped
void AccelStepper::runToPosition()
{
    while (run())
        ;
}

bool AccelStepper::runSpeedToPosition()
{
    if (_targetPos == _currentPos)
        return false;
    if (_targetPos > _currentPos)
        _direction = DIRECTION_CW;
    else
        _direction = DIRECTION_CCW;
    return runSpeed();
}

// Blocks until the new target position is reached
void AccelStepper::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

void AccelStepper::stop()
{
    if (_speed != 0.0)
    {
        long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration)) + 1; // Equation 16 (+integer rounding)
        if (_speed > 0)
            move(stepsToStop);
        else
            move(-stepsToStop);
    }
}
