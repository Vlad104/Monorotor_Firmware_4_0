#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../mbed.h"
#include <string>
#include <cstdlib>

#include "params.h"
#include "DataModel.h"
#include "MultiDozator.h"

class Controller {
public:
    Controller();
    ~Controller();

    void loop();

private:
    Serial*         rs485_;
    Serial*         usb_; 
    Serial*         last_port_;
    DigitalOut*     transmit_enable_;

    MultiDozator    dozators_;

    void rs485_event();
    void usb_event();
    void serial_event(Serial* port);
    void read_command(Serial* port);
    void read_params(Serial* port);
    void make_command(char command);
    void make_params(char message_type, const char str_buffer[STR_SIZE]);
    //void make_params(char message_type, const std::string& str_buffer);
    float str_to_float(const char str_buffer[STR_SIZE]);
    //float str_to_float(const std::string& str_buffer);
    void send_answer(Serial* port, char answer);

};

#endif // CONTROLLER_H