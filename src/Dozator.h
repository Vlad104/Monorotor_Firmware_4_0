#ifndef DOZATOR_H
#define DOZATOR_H

#include "params.h"
#include "AccelStepper.h"
#include "mbed.h"

class Dozator {
public:
    Dozator(PinName step_pin, PinName dir_pin);
    ~Dozator();

    void set_volume(float volume);
    void set_feedrate(float feedrate);
    void set_accel(float accel);

    void start_movement();
    void stop_movement();
    void continues_movement();

    void run();
    bool stopped();

    #ifdef TEST
        void print(Serial* port);
    #endif

private:
    int32_t     volume_;    // imp
    uint32_t    feedrate_;  // imp/sec
    uint32_t    accel_;     // imp/sec^2

    AccelStepper* stepper_;
};

#endif // Dozator