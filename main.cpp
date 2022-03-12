#include "mbed.h"
#include "src/params.h"
#include "src/Controller.h"
#include "src/Dozator.h"
#include "src/Store.h"
#include "src/Alert.h"
#include "src/Pedal.h"


void event_loop();
void run();
void start();
void reverse();
void continues_start();
void stop();
void alert_handler(char alert_type);
void rise_handler();
void fall_handler();

// Timer for AccelStepper lib
// makes delay for step driver
Timer t;

#ifdef DEBUG
Timer t_test;
long ms1 = 0;
long ms2 = 0;
#endif

Dozator dozator_A(STEP_A, DIR_A, EN_A);
Dozator dozator_B(STEP_B, DIR_B, EN_B);

Store* store = new Store();
Controller* controller = new Controller(store, &start, &continues_start, &stop);
Alert* alert = new Alert(&alert_handler);
Pedal* pedal = new Pedal(&rise_handler, &fall_handler);

bool is_run = false;
bool need_reverse = false;
bool enable_reverse_after_stop = true;
bool was_stopped = false;
int times_A = 0;
int times_B = 0;

int main() {
    t.start();
    #ifdef DEBUG
    t_test.start();
    #endif
    event_loop();
}

void event_loop() {
    while (1) {
        run();

        if (was_stopped) {
            dozator_A.stop_movement();
            dozator_B.stop_movement();

            was_stopped = false;

            if (!INSTANT_RESPONSE) {
                controller->send_answer('!');
            }
            #ifdef STOP_RESPONSE
                controller->send_answer('S');
            #endif
        }
    }
}

void run() {
    while(!was_stopped) {
        int counter_A = 0;
        int counter_B = 0;

        while (dozator_A.run() && ++counter_A < times_A);
        while (dozator_B.run() && ++counter_B < times_B);

        #ifdef DEBUG
        if (ms1 == 0 && is_run && dozator_A.distanceIsNull()) {
            ms1 = t_test.read_ms();
        }     
           
        if (ms2 == 0 && is_run && dozator_B.distanceIsNull()) {
            ms2 = t_test.read_ms();
        }
        #endif

        if (is_run && dozator_A.distanceIsNull() && dozator_B.distanceIsNull()) {
            #ifdef DEBUG
            controller->send_delay(ms2 - ms1);
            ms2 = 0;
            ms1 = 0;
            #endif
            is_run = false;
            if (need_reverse) {
                reverse();
            } else {
                was_stopped = true;
            }
        }
    }
}

void start() {
    store->calculate_times(times_A, times_B);
    #ifdef DEBUG
    controller->send_times(times_A, times_B);
    #endif
    need_reverse = (store->buffer.V != 0) ? true : false;
    is_run = true;

    dozator_A.start_movement(store->data[0].volume, store->data[0].feedrate, store->data[0].accel);
    dozator_B.start_movement(store->data[1].volume, store->data[1].feedrate, store->data[1].accel);
}

void reverse() {
    store->calculate_times(times_A, times_B);
    #ifdef DEBUG
    controller->send_times(times_A, times_B);
    #endif
    need_reverse = false;
    is_run = true;

    dozator_A.start_movement(store->data[0].reverse, store->data[0].feedrate, store->data[0].accel);
    dozator_B.start_movement(store->data[1].reverse, store->data[1].feedrate, store->data[1].accel);
}

void continues_start() {
    store->calculate_times(times_A, times_B);
    #ifdef DEBUG
    controller->send_times(times_A, times_B);
    #endif
    need_reverse = false;
    is_run = true;

    dozator_A.continues_movement(store->data[0].feedrate, store->data[0].accel);
    dozator_B.continues_movement(store->data[1].feedrate, store->data[1].accel);
}

void stop() {
    dozator_A.stop_movement();
    dozator_B.stop_movement();

    need_reverse = enable_reverse_after_stop;
    is_run = true;
}

void alert_handler(char alert_type) {
//    stop();
    controller->send_answer(alert_type);
}

void rise_handler() {
   stop();
}

void fall_handler() {
   continues_start();
}