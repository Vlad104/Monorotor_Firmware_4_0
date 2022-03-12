#ifndef AccelStepper_h
#define AccelStepper_h

#include <stdlib.h>
#include <mbed.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define LOW false
#define HIGH true

#undef round

class AccelStepper
{
public:
    AccelStepper(PinName pin1 = LED1, PinName pin2 = LED2, PinName pin3 = LED3, PinName pin4 = LED4, bool enable = true);

    void moveTo(long absolute);
    void move(long relative);

    bool run();
    bool runSpeed();
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);
    void setSpeed(float speed);
    float speed();
    long distanceToGo() {
        return _targetPos - _currentPos;
    }
    long targetPosition();
    long currentPosition();
    void setCurrentPosition(long position);
    void runToPosition();
    bool runSpeedToPosition();
    void runToNewPosition(long position);
    void stop();
    virtual void disableOutputs();
    virtual void enableOutputs();

    void setMinPulseWidth(unsigned int minWidth);
    void setEnablePin(PinName enablePin);
    void setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false);
    void setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert);

    bool distanceIsNull()
    {
        return _targetPos - _currentPos == 0;
    }

protected:
    DigitalOut *_pin0;
    DigitalOut *_pin1;
    DigitalOut *_pin2;
    DigitalOut *_pin3;
    typedef enum
    {
        DIRECTION_CCW = 0, ///< Clockwise
        DIRECTION_CW = 1   ///< Counter-Clockwise
    } Direction;

    void computeNewSpeed();
    virtual void setOutputPins(uint8_t mask);
    virtual void step();

private:
    uint8_t _pin[4];
    uint8_t _pinInverted[4];
    long _currentPos; // Steps
    long _targetPos;  // Steps
    float _speed;     // Steps per second
    float _maxSpeed;
    float _acceleration;
    float _sqrt_twoa; // Precomputed sqrt(2*_acceleration)
    unsigned long _stepInterval;
    unsigned long _lastStepTime;
    unsigned int _minPulseWidth;
    bool _enableInverted;
    DigitalOut *_enablePin;
    void (*_forward)();
    void (*_backward)();
    long _n;
    float _c0;
    float _cn;
    float _cmin;     // at max speed
    bool _direction; // 1 == CW
};

#endif
