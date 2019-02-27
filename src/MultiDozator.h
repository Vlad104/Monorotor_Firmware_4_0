#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"
#include "DataModel.h"

#ifdef TEST   
    #include "mbed.h"
#endif

class MultiDozator {
public:
    MultiDozator();
    ~MultiDozator();

    void calculate_volume(float volume);
    void calculate_feedrate(float feedrate);
    void calculate_accel(float feedrate);
    void calculate_ratio(float ratio_A);
    void calculate_gear(float gear_A, float gear_B);
    void calculate_all();

    void set_dozator(char dozator);

    void start();
    void stop();
    void continues_start();
    void run(bool& was_stopped);
    /*
    void run(bool& was_stopped) {
        dozators_[0]->run();
        dozators_[1]->run();
        
        if (is_run_ && dozators_[0]->stopped() && dozators_[1]->stopped()) {
            is_run_ = false;
            was_stopped = true;
        }
    }
    */

    #ifdef TEST   
        void print(Serial* port);
    #endif

private:
    DataModel   data_;
    Dozator*    dozators_[2];
    bool        is_run_;
};

#endif // MULTI_DOZATOR_H