#include "Alert.h"

Alert::Alert(AlertCallback handler) {
    handler_ = handler;

    alert_A_ = new InterruptIn(ALERT_A);
    alert_B_ = new InterruptIn(ALERT_B);
    alert_btnA_ = new DigitalIn(ALERT_A);
    alert_btnB_ = new DigitalIn(ALERT_B);
    alert_btnA_->mode(PullUp);
    alert_btnB_->mode(PullUp);

    alert_A_->fall(this, &Alert::alert_fall_A);
    alert_B_->fall(this, &Alert::alert_fall_B);
}

void Alert::alert_fall_A() {
    handler_('A');
}

void Alert::alert_fall_B() {
    handler_('B');
}
