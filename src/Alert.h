#ifndef ALERT_H
#define ALERT_H

#include "mbed.h"
#include "params.h"

typedef void (*AlertCallback)(char);

class Alert {
public:
    Alert(AlertCallback handler);
    ~Alert();

private:
    AlertCallback handler_;

    InterruptIn*    alert_A_;
    InterruptIn*    alert_B_;
    DigitalIn*      alert_btnA_;
    DigitalIn*      alert_btnB_;

    void alert_fall_A();
    void alert_fall_B();
};

#endif // ALERT_H