#include "Controller.h"

Controller::Controller() {
    usb_ = new Serial(USB_TX, USB_RX);
    rs485_ = new Serial(UART_TX, UART_RX);
    remote_ = new Serial(UART2_TX, UART2_RX);
    last_port_ = usb_;

    pedal_ = new InterruptIn(PEDAL);
    pedal_btn_ = new DigitalIn(PEDAL);
    alert_A_ = new InterruptIn(ALERT_A);
    alert_B_ = new InterruptIn(ALERT_B);
    alert_btnA_ = new DigitalIn(ALERT_A);
    alert_btnB_ = new DigitalIn(ALERT_B);
    
    alert_btnA_->mode(PullUp);
    alert_btnB_->mode(PullUp);
    pedal_btn_->mode(PullUp);

    usb_->baud(UART_BAUDRATE);
    rs485_->baud(UART_BAUDRATE);
    remote_->baud(UART_BAUDRATE);

    usb_->attach(this, &Controller::usb_event, Serial::RxIrq);
    rs485_->attach(this, &Controller::rs485_event, Serial::RxIrq);
    remote_->attach(this, &Controller::remote_event, Serial::RxIrq);

    transmit_enable_ = new DigitalOut(UART_EN);
    transmit_enable_->write(0);
    
    ena_ = new DigitalOut(PB_0);
    ena_->write(1);

    pedal_->rise(this, &Controller::pedal_rise);
    pedal_->fall(this, &Controller::pedal_fall);
    alert_A_->rise(this, &Controller::alert_rise_A);
    alert_A_->fall(this, &Controller::alert_fall_A);
    alert_B_->rise(this, &Controller::alert_rise_B);
    alert_B_->fall(this, &Controller::alert_fall_B);

    #ifdef TEST
        usb_->printf("Monorotor Firmware 3.2\r\n");
        ms1 = 0;
        ms2 = 0;
        t_test.start();
    #endif
}

Controller::~Controller() {
    delete rs485_;
    delete usb_;
    delete transmit_enable_;
    delete pedal_;
    delete alert_A_;
    delete alert_B_;
}

void Controller::start_control(Dozator* A, Dozator* B) {
    dozators_.activate_dozators(A, B);
}

void Controller::rs485_event() {
    serial_event(rs485_);
}

void Controller::usb_event() {
    serial_event(usb_);
}

void Controller::remote_event() {
    serial_event(remote_);
}

void Controller::serial_event(Serial* port) {
    last_port_ = port;
    char message_type = port->getc();

    if (message_type == '=') {    
        #ifdef TEST
            t_test.stop();
            t_test.reset();
            t_test.start();
            ms1 = t_test.read_ms();      
        #endif  
        read_command(port);
    } else if (message_type == '@') {
        read_params(port);
    } else {
        send_answer(port, '?');
    }
}

void Controller::read_command(Serial* port) {
    char temp = port->getc();
    make_command(temp);
}

void Controller::read_params(Serial* port) {
    char params_type = port->getc();
    char temp = port->getc();

    char buffer_1[STR_SIZE+1];
    char buffer_2[STR_SIZE+1];

    int size = 0;
    while (temp != '*' && size < STR_SIZE) {
        buffer_1[size] = temp;
        temp = port->getc();
        size++;
    }

    for (int i = size; i < STR_SIZE; i++) {
        buffer_1[i] = '0';
    }

    int start = 0;
    if (buffer_1[0] == '-') {
        buffer_2[0] = '-';
        start = 1;
    }

    int ofset = STR_SIZE - size;
    for (int i = 0; i < ofset; i++) {
        buffer_2[i + start] = '0';
    }

    for (int i = start; i < size; i++) {
        buffer_2[ofset + i] = buffer_1[i];
    }

    buffer_1[STR_SIZE] = '\0';
    buffer_2[STR_SIZE] = '\0';

    #ifdef TEST
        usb_->printf("command: %c: %s\r\n", params_type, buffer_1);
        usb_->printf("command: %c: %s\r\n", params_type, buffer_2);
    #endif
    make_params(params_type, buffer_2);
    send_answer(port, '!');
}

//void Controller::read_params(Serial* port) {
//    char params_type = port->getc();
//    char temp = port->getc();
//
//    char buffer[STR_SIZE+1];
//    int size = 0;
//
//    if (temp == '-') {
//        buffer[0] = '-';
//        size++;
//        temp = port->getc();
//    }
//
//    while (temp != '*' && size < STR_SIZE) {
//        buffer[size] = temp;
//        temp = port->getc();
//        size++;
//    }
//
//    buffer[size] = '\0';
//
//    #ifdef TEST
//        usb_->printf("command: %c: %s\r\n", params_type, buffer);
//    #endif
//    make_params(params_type, buffer);
//    send_answer(port, '!');
//}

void Controller::make_command(char command) {
    #ifdef TEST
        usb_->printf("command: %c\r\n", command);
        dozators_.print(usb_);
    #endif
    switch (command) {
        case '0':
            #ifdef TEST
                usb_->printf("stop\r\n");
            #endif
            dozators_.stop();
            break;
        case '1':
            #ifdef TEST
                usb_->printf("start\r\n");
            #endif
            dozators_.start();
            break;
        case '2':
            #ifdef TEST
                usb_->printf("continues\r\n");
            #endif
            dozators_.continues_start();
            break;
        default:
            break;
    }
}


void Controller::make_params(char params_type, const char str_buffer[STR_SIZE]) {
    float value = str_to_float(str_buffer);
    #ifdef TEST
        usb_->printf("params value: %c: %f\r\n", params_type, value);
    #endif
    switch (params_type) {
        case 'D':     
            dozators_.set_dozator(str_buffer[STR_SIZE-1]);
            break;
        case 'V':     
            dozators_.calculate_volume(value);
            break;
        case 'F':
            dozators_.calculate_feedrate(value);
            break;
        case 'A':
            dozators_.calculate_accel(value);
            break;
        case 'a':
            dozators_.calculate_gear(value, -1);
            break;
        case 'b':
            dozators_.calculate_gear(-1, value);
            break;
        case 'r':
            dozators_.calculate_ratio(value);
            break;
        default:
            break;
    }
}

float Controller::str_to_float(const char str_buffer[STR_SIZE]) {
    return (float) atof(str_buffer);
}

void Controller::loop() {
    bool was_stopped = false;
    while (1) {
        // dozators_.run(was_stopped);
        single_loop(was_stopped);

        if (was_stopped) {
            was_stopped = false;
            send_answer(last_port_, '!');
        }
    }
}

void Controller::single_loop(bool& was_stopped) {
    while(!was_stopped) {
        dozators_.run(was_stopped);
    }
}

void Controller::send_answer(Serial* port, char answer) {
    #ifdef TEST   
        ms2 = t_test.read_ms();
        t_test.stop();
        port->printf("\r\nT: %ld\r\n", ms2-ms1);
    #endif
    transmit_enable_->write(1);
    port->putc(answer);
    transmit_enable_->write(0);
}


void Controller::pedal_rise() {
//    dozators_.continues_start();
    dozators_.stop();
}

void Controller::pedal_fall() {
//    dozators_.stop();
    dozators_.continues_start();
}

void Controller::alert_rise_A() {
    send_answer(last_port_, 'A');
}

void Controller::alert_fall_A() {
    send_answer(last_port_, 'a');
}

void Controller::alert_rise_B() {
    send_answer(last_port_, 'B');
}

void Controller::alert_fall_B() {
    send_answer(last_port_, 'b');
}