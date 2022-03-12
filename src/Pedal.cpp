#include "Pedal.h"

Pedal::Pedal(PedalCallback rise_handler, PedalCallback fall_handler) {
    rise_handler_ = rise_handler;
    fall_handler_ = fall_handler;

    pedal_pressed_ = false;
    pedal_ = new InterruptIn(PEDAL_PIN);
    pedal_btn_ = new DigitalIn(PEDAL_PIN);
    pedal_btn_->mode(PullUp); 
    pedal_->rise(this, &Pedal::pedal_rise);
    pedal_->fall(this, &Pedal::pedal_fall);
}

void Pedal::pedal_rise() {
    if (!pedal_pressed_) {
       return;
    }

    pedal_pressed_ = false;
    rise_handler_();
}

void Pedal::pedal_fall() {
    if (pedal_pressed_) {
       return;
    }

    pedal_pressed_ = true;
    fall_handler_();
}
