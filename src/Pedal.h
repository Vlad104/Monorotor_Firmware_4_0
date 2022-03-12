#ifndef PEDAL_H
#define PEDAL_H

#include "mbed.h"
#include "params.h"

typedef void (*PedalCallback)();

class Pedal {
public:
    Pedal(PedalCallback rise_handler, PedalCallback fall_handler);
    ~Pedal();

private:
    PedalCallback rise_handler_;
    PedalCallback fall_handler_;

    InterruptIn*    pedal_;
    DigitalIn*      pedal_btn_;
    bool pedal_pressed_;
    void pedal_rise();
    void pedal_fall();
};

#endif // PEDAL_H