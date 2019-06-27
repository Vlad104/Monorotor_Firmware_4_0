#include "MultiDozator.h"

MultiDozator::MultiDozator() {
    is_run_ = false;
    // calculate_ratio(0.5);
}

MultiDozator::~MultiDozator() {}

void MultiDozator::activate_dozators(Dozator* A, Dozator* B) {     
    dozator_A_ = A;    
    dozator_B_ = B;
}

void MultiDozator::calculate_volume(float volume) {
    data_.volume = volume;

    float volume_A = data_.volume * data_.ratio_A * data_.gear_A;
    float volume_B = data_.volume * data_.ratio_B * data_.gear_B;

    dozator_A_->set_volume(volume_A); 
    dozator_B_->set_volume(volume_B); 
}

void MultiDozator::calculate_feedrate(float feedrate) {   
    data_.feedrate = feedrate;

    float feedrate_A = data_.feedrate * data_.ratio_A * data_.gear_A / 60.0;
    float feedrate_B = data_.feedrate * data_.ratio_B * data_.gear_B / 60.0;

    dozator_A_->set_feedrate(feedrate_A); 
    dozator_B_->set_feedrate(feedrate_B); 
}

void MultiDozator::calculate_accel(float accel) {
    data_.accel = accel;

    float accel_A = data_.accel * data_.ratio_A * data_.gear_A / 60.0;
    float accel_B = data_.accel * data_.ratio_B * data_.gear_B / 60.0;

    dozator_A_->set_accel(accel_A);   
    dozator_B_->set_accel(accel_B);   
}

void MultiDozator::calculate_ratio(float ratio_A) {
    data_.ratio_A = ratio_A;
    data_.ratio_B = 1.0 - ratio_A;
    calculate_all();

    // experimental
    if (data_.ratio_A == data_.ratio_B) {
        times_A_ = 1;
        times_B_ = 1;
    }
    else if (data_.ratio_A < data_.ratio_B) {
        float temp = (1.0 - data_.ratio_A) / data_.ratio_A;
        temp += 0.5; // for rounding
        int temp_round = (int) temp;
        times_A_ = 1;
        times_B_ = temp_round;
    } else {
        float temp = (1.0 - data_.ratio_B) / data_.ratio_B;
        temp += 0.5; // for rounding
        int temp_round = (int) temp;
        times_A_ = temp_round;
        times_B_ = 1;
    }
}

void MultiDozator::calculate_gear(float gear_A, float gear_B) {
    if (gear_A > 0.0) {
        data_.gear_A = gear_A;
    }
    if (gear_B > 0.0) {
        data_.gear_B = gear_B;
    }
    calculate_all();  
}

void MultiDozator::calculate_all() {
    calculate_volume(data_.volume);
    calculate_feedrate(data_.feedrate);
    calculate_accel(data_.accel);
}

void MultiDozator::set_dozator(char dozator) {
    data_.dozator = dozator;
}

void MultiDozator::start() {
    is_run_ = true;
    switch (data_.dozator) {
        case '2':
            dozator_A_->setMinPulseWidth(PULSE_WIDTH / 2);
            dozator_B_->setMinPulseWidth(PULSE_WIDTH / 2);
            dozator_A_->start_movement();
            dozator_B_->start_movement();
            break;
        case '0':
            dozator_A_->setMinPulseWidth(PULSE_WIDTH);
            dozator_A_->start_movement();
            break;
        case '1':
            dozator_B_->setMinPulseWidth(PULSE_WIDTH);
            dozator_B_->start_movement();
            break;
        default:
            stop();
    }
}

void MultiDozator::stop() {
    dozator_A_->stop_movement();
    dozator_B_->stop_movement();
    is_run_ = true;
}

void MultiDozator::continues_start() {
    is_run_ = true;
    switch (data_.dozator) {
        case '2':
            dozator_A_->setMinPulseWidth(PULSE_WIDTH / 2);
            dozator_B_->setMinPulseWidth(PULSE_WIDTH / 2);
            dozator_A_->continues_movement();
            dozator_B_->continues_movement();
            break;
        case '0':
            dozator_A_->setMinPulseWidth(PULSE_WIDTH);
            dozator_A_->continues_movement();
            break;
        case '1':
            dozator_B_->setMinPulseWidth(PULSE_WIDTH);
            dozator_B_->continues_movement();
            break;
        default:
            stop();
    }
}

#ifdef TEST   
    void MultiDozator::print(Serial* port) {            
        port->printf("\r\nDataModel\r\n");
        port->printf("data_.dozator: %c\r\n", data_.dozator);
        port->printf("data_.volume: %f\r\n", data_.volume);
        port->printf("data_.feedrate: %f\rn", data_.feedrate);
        port->printf("data_.accel: %f\r\n", data_.accel);
        port->printf("data_.gear_A: %f\r\n", data_.gear_A);
        port->printf("data_.gear_B: %f\r\n", data_.gear_B);
        port->printf("data_.ratio_A: %f\r\n", data_.ratio_A);
        port->printf("data_.ratio_B: %f\r\n", data_.ratio_B);
        port->printf("times_A_: %d\r\n", times_A_);
        port->printf("times_B_: %d\r\n", times_B_);

        dozator_A_->print(port);
        dozator_B_->print(port);
    }
#endif