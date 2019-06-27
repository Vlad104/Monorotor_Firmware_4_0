#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"

#ifdef TEST   
    #include "mbed.h"
#endif

class MultiDozator {
public:
    MultiDozator();
    ~MultiDozator();

    void activate_dozators(Dozator* A, Dozator* B);

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

    // inline func
//    void run(bool& was_stopped)  {
//        bool stopped_A = !dozator_A_->run();
//        bool stopped_B = !dozator_B_->run();
//    
//        if (is_run_ && stopped_A && stopped_B) {
//            is_run_ = false;
//            was_stopped = true;
//        }
//    }

    // inline func
    void run(bool& was_stopped) {
        int counter_A = 0;
        int counter_B = 0;

        // ход конем так сказать
        while (dozator_A_->run() && ++counter_A < times_A_);
        while (dozator_B_->run() && ++counter_B < times_B_);

        if (is_run_ && dozator_A_->distanceIsNull() && dozator_B_->distanceIsNull()) {
            is_run_ = false;
            was_stopped = true;
        }
    }

    // void run(bool& was_stopped) {
    //     switch (data_.dozator)
    //     {
    //     case 'D':
    //         run_AB(was_stopped);
    //         break;
    //     case 'A':
    //         run_A(was_stopped);
    //         break;
    //     case 'B':
    //         run_A(was_stopped);
    //         break;
    //     default:
    //         break;
    //     }
    // }

    void run_AB(bool& was_stopped) {
        int counter_A = 0;
        int counter_B = 0;

        // ход конем так сказать
        while (dozator_A_->run() && counter_A++ < times_A_);
        while (dozator_B_->run() && counter_B++ < times_B_);

        if (is_run_ && (dozator_A_->distanceIsNull() || dozator_B_->distanceIsNull())) {
            dozator_A_->stop();
            dozator_B_->stop();
            while (dozator_A_->run());
            while (dozator_B_->run());
            is_run_ = false;
            was_stopped = true;
        }
    }

    void run_A(bool& was_stopped) {
        if (is_run_ && !dozator_A_->run()) {
            is_run_ = false;
            was_stopped = true;
        }
    }

    void run_B(bool& was_stopped) {
        if (is_run_ && !dozator_B_->run()) {
            is_run_ = false;
            was_stopped = true;
        }
    }

    #ifdef TEST   
        void print(Serial* port);
    #endif

private:
    struct DataModel {
        DataModel() :
//            dozator('2'),
//            volume(0.0),
//            feedrate(0.0),
//            accel(0.0),
//            gear_A(1.0),
//            gear_B(1.0),
//            ratio_A(0.5),
//            ratio_B(0.5)
            dozator('2'),
            volume(100.0),
            feedrate(200.0),
            accel(1000.0),
            gear_A(2030.0),
            gear_B(2030.0),
            ratio_A(0.5),
            ratio_B(0.5)
        {}
        char    dozator;
        float   volume;     // ul
        float   feedrate;   // ul/min
        float   accel;      // ul/min*sec
        float   gear_A;     // ul to impulses
        float   gear_B;     // ul to impulses
        float   ratio_A;    // ratio A to all
        float   ratio_B;    // ratio A to all
    };
    DataModel   data_;
    Dozator*    dozator_A_;
    Dozator*    dozator_B_;
    bool        is_run_;

    int times_A_;
    int times_B_;
};

#endif // MULTI_DOZATOR_H