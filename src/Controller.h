#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"
#include <cstdlib>

#include "params.h"
#include "Dozator.h"
#include "MultiDozator.h"

class Controller {
public:
    Controller();
    ~Controller();

    void start_control(Dozator* A, Dozator* B);
    void loop();

private:
    Serial*         usb_; 
    Serial*         rs485_;
    Serial*         remote_;
    Serial*         last_port_;
    DigitalOut*     transmit_enable_;
    
    DigitalOut*     ena_;

    InterruptIn*    pedal_;
    InterruptIn*    alert_A_;
    InterruptIn*    alert_B_;
    DigitalIn*      alert_btnA_;
    DigitalIn*      alert_btnB_;
    DigitalIn*      pedal_btn_;

    MultiDozator    dozators_;

    void usb_event();
    void rs485_event();
    void remote_event();
    void serial_event(Serial* port);
    void read_command(Serial* port);
    void read_params(Serial* port);
    void make_command(char command);
    void make_params(char message_type, const char str_buffer[STR_SIZE]);
    float str_to_float(const char str_buffer[STR_SIZE]);
    void send_answer(Serial* port, char answer);

    void single_loop(bool& was_stopped);

    void pedal_rise();
    void pedal_fall();
    void alert_rise_A();
    void alert_fall_A();
    void alert_rise_B();
    void alert_fall_B();
    
    #ifdef TEST
        Timer t_test;
        long ms1;
        long ms2;
    #endif
};

#endif // CONTROLLER_H