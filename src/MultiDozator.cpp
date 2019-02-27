#include "MultiDozator.h"

MultiDozator::MultiDozator()
{
    dozators_[0] = new Dozator(STEP_A, DIR_A);
    dozators_[1] = new Dozator(STEP_B, DIR_B);
    is_run_ = false;
}

MultiDozator::~MultiDozator() {
    delete dozators_[0];
    delete dozators_[1];
}

void MultiDozator::calculate_volume(float volume) {
    data_.volume = volume;

    float volume_A = data_.volume * data_.ratio_A * data_.gear_A;
    float volume_B = data_.volume * data_.ratio_B * data_.gear_B;

    dozators_[0]->set_volume(volume_A); 
    dozators_[1]->set_volume(volume_B); 
}

void MultiDozator::calculate_feedrate(float feedrate) {
    data_.feedrate = feedrate;

    float feedrate_A = data_.feedrate * data_.ratio_A * data_.gear_A / 60;
    float feedrate_B = data_.feedrate * data_.ratio_B * data_.gear_B / 60;

    dozators_[0]->set_feedrate(feedrate_A); 
    dozators_[1]->set_feedrate(feedrate_B); 
}

void MultiDozator::calculate_accel(float accel) {
    data_.accel = accel;

    float accel_A = data_.accel * data_.ratio_A * data_.gear_A / 60;
    float accel_B = data_.accel * data_.ratio_B * data_.gear_B / 60;

    dozators_[0]->set_accel(accel_A);   
    dozators_[1]->set_accel(accel_B);   
}

void MultiDozator::calculate_ratio(float ratio_A) {
    data_.ratio_A = ratio_A;
    data_.ratio_B = 1.0 - ratio_A;  
    calculate_all();  
}

void MultiDozator::calculate_gear(float gear_A, float gear_B) {
    if (gear_A > 0) {
        data_.gear_A = gear_A;
    }
    if (gear_B > 0) {
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
    if (data_.dozator == '2') {
        dozators_[0]->start_movement();
        dozators_[1]->start_movement();
    } else if (data_.dozator == '0') {        
        dozators_[0]->start_movement();
    } else if (data_.dozator == '1') {      
        dozators_[1]->start_movement();        
    } else {
        stop();
    }
}

void MultiDozator::stop() {
    dozators_[0]->stop_movement();
    dozators_[1]->stop_movement();
    is_run_ = true;
}

void MultiDozator::continues_start() {
    is_run_ = true;
    if (data_.dozator == '2') {
        dozators_[0]->continues_movement();
        dozators_[1]->continues_movement();
    } else if (data_.dozator == '0') {        
        dozators_[0]->continues_movement();
    } else if (data_.dozator == '1') {      
        dozators_[1]->continues_movement();        
    } else {
        stop();
    }
}


void MultiDozator::run(bool& was_stopped) {
    dozators_[0]->run();
    dozators_[1]->run();

    if (is_run_ && dozators_[0]->stopped() && dozators_[1]->stopped()) {
        is_run_ = false;
        was_stopped = true;
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

        dozators_[0]->print(port);
        dozators_[1]->print(port);
    }
#endif