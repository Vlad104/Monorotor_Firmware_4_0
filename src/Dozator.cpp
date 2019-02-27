#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin) :
    volume_(0), feedrate_(0), accel_(0)
{
    stepper_ = new AccelStepper(1, step_pin, dir_pin);
    stepper_->setMinPulseWidth(PULSE_WIDTH);
    stepper_->setMaxSpeed(MOTOR_MAX_SPEED); 
    stepper_->setCurrentPosition(0);
    stepper_->setPinsInverted(true, true, false);
}

Dozator::~Dozator() {
    delete stepper_;
}

void Dozator::set_volume(float volume) {
    volume_ = ((int32_t) volume) + 1;     // + 1 is rounds
}

void Dozator::set_feedrate(float feedrate) {
    feedrate_ = ((uint32_t) feedrate) + 1;   // + 1 is rounds
}

void Dozator::set_accel(float accel) {
    accel_ = ((uint32_t) accel) + 1;      // + 1 is rounds
}

void Dozator::start_movement() {
    stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(accel_);  
    stepper_->move(volume_);
    stepper_->setMaxSpeed(feedrate_);
}

void Dozator::stop_movement() {
    stepper_->setCurrentPosition(0); 
    stepper_->move(0);
}

void Dozator::continues_movement() {
    stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(accel_);  
    stepper_->move(1000000000);
    stepper_->setMaxSpeed(feedrate_);   
}

void Dozator::run() { // inline
    stepper_->run();
}

bool Dozator::stopped() { // inline
    return stepper_->distanceToGo() == 0;
}

#ifdef TEST   
    void Dozator::print(Serial* port) {            
        port->printf("\r\nDozator 1:\r\n");
        port->printf("volume_: %d\r\n", volume_);
        port->printf("feedrate_: %d\r\n", feedrate_);
        port->printf("accel_: %d\r\n", accel_);
    }
#endif