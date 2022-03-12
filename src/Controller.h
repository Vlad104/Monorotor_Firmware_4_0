#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"
#include <cstdlib>

#include "params.h"
#include "Store.h"

typedef void (*ControllerCallback)();

class Controller {
public:
    Controller(Store* store, ControllerCallback start, ControllerCallback continues, ControllerCallback stop);
    ~Controller();

    void send_answer(char answer);
    #ifdef DEBUG
    void send_store();
    void send_delay(long delay);
    void send_times(long t1, long t2);
    #endif

#ifndef TESTS
private:
#endif

    Serial*         usb_; 
    Serial*         rs485_;
    Serial*         last_port_;

    Store*          store_;

    ControllerCallback start_;
    ControllerCallback continues_;
    ControllerCallback stop_;

    void usb_event();
    void rs485_event();
    void serial_event(Serial* port);

    void read_command(Serial* port);
    void read_params(Serial* port);

    void make_command(char command);
    void make_params(char message_type, const char* str_buffer);

    bool is_param_symbol(char symbol);
    void parse_params(const char* str_buffer, int size);
    float str_to_float(const char* str_buffer);
};

#endif // CONTROLLER_H