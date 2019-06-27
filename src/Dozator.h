#ifndef DOZATOR_H
#define DOZATOR_H

#include "mbed.h"
#include "params.h"
#include "AccelStepper.h"

class Dozator : public AccelStepper {
public:
    Dozator(PinName step_pin, PinName dir_pin);
    ~Dozator() {};

    void set_volume(float volume);
    void set_feedrate(float feedrate);
    void set_accel(float accel);

    void start_movement();
    void stop_movement();
    void continues_movement();

    // bool stopped();

    #ifdef TEST
        void print(Serial* port);
    #endif

private:
    long    volume_;    // imp
    float   feedrate_;  // imp/sec
    float   accel_;     // imp/sec^2
};

#endif // Dozator