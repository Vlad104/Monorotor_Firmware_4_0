#ifndef DOZATOR_H
#define DOZATOR_H

#include "mbed.h"
#include "params.h"
#include "AccelStepper.h"

class Dozator : public AccelStepper {
public:
    Dozator(PinName step_pin, PinName dir_pin, PinName enable_pin);
    ~Dozator() {};

    void start_movement(float volume, float feedrate, float accel);
    void stop_movement();
    void continues_movement(float feedrate, float accel);

private:
    DigitalOut* enable;
    long prepare_volume(float volume);
};

#endif // Dozator