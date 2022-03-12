#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin, PinName enable_pin) : AccelStepper(step_pin, dir_pin) {
    enable = new DigitalOut(enable_pin);
    setMinPulseWidth(PULSE_WIDTH);
    setMaxSpeed(MOTOR_MAX_SPEED); 
    setCurrentPosition(0);
    setPinsInverted(false, false, false);
}

long Dozator::prepare_volume(float volume) {
    if (volume > 100000000.0 || volume < -100000000.0) {
        return (long) volume;
    }
    return (volume > 0.0) ? (long) (volume + 0.5) : (long) (volume - 0.5);
}

void Dozator::start_movement(float volume, float feedrate, float accel) {
    long value = prepare_volume(volume);
    setCurrentPosition(0);
    setAcceleration(accel);
    move(value);
    setMaxSpeed(feedrate);
    enable->write(1);
}

void Dozator::continues_movement(float feedrate, float accel) {
    setCurrentPosition(0);
    setAcceleration(accel);
    move(1000000000);
    setMaxSpeed(feedrate);
    enable->write(1);
}

void Dozator::stop_movement() {
    setCurrentPosition(0);
    move(0);
    enable->write(0);
}
