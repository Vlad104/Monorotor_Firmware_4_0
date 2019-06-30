#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin) :
    // AccelStepper(1, step_pin, dir_pin),
    AccelStepper(step_pin, dir_pin),
    volume_(0), feedrate_(0.0), accel_(0.0)
{
    setMinPulseWidth(PULSE_WIDTH);
    setMaxSpeed(MOTOR_MAX_SPEED); 
    setCurrentPosition(0);
    setPinsInverted(false, false, false);
}

long local_round(float value) {
    long floor_value = (long) value;
    float mod = value - (float)floor_value;
    return (mod > 0.5) ? floor_value + 1 : floor_value;
}

void Dozator::set_volume(float volume) {
    volume_ = local_round(volume);
}

void Dozator::set_feedrate(float feedrate) {
    feedrate_ = feedrate;
}

void Dozator::set_accel(float accel) {
    accel_ = accel;
}

void Dozator::start_movement() {
    setCurrentPosition(0);
    setAcceleration(accel_);  
    move(volume_);
    setMaxSpeed(feedrate_);
}

void Dozator::stop_movement() {
    setCurrentPosition(0); 
    move(0);
}

void Dozator::continues_movement() {
    setCurrentPosition(0);
    setAcceleration(accel_);  
    move(1000000000);
    setMaxSpeed(feedrate_);   
}

// bool Dozator::stopped() { // inline
//     return distanceToGo() == 0;
// }

#ifdef TEST   
    void Dozator::print(Serial* port) {            
        port->printf("\r\nDozator:\r\n");
        port->printf("volume_: %ld\r\n", volume_);
        port->printf("feedrate_: %f\r\n", feedrate_);
        port->printf("accel_: %f\r\n", accel_);
    }
#endif