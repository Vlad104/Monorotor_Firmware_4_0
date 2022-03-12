#include "Controller.h"

Controller::Controller(Store* store, ControllerCallback start, ControllerCallback continues, ControllerCallback stop) {
    store_ = store;
    start_ = start;
    continues_ = continues;
    stop_ = stop;

    usb_ = new Serial(USB_TX, USB_RX);
    rs485_ = new Serial(UART_TX, UART_RX);
    last_port_ = usb_;

    usb_->baud(UART_BAUDRATE);
    rs485_->baud(UART_BAUDRATE);

    usb_->attach(this, &Controller::usb_event, Serial::RxIrq);
    rs485_->attach(this, &Controller::rs485_event, Serial::RxIrq);
}

Controller::~Controller() {
    delete rs485_;
    delete usb_;
}

void Controller::rs485_event() {
    serial_event(rs485_);
}

void Controller::usb_event() {
    serial_event(usb_);
}

inline void Controller::serial_event(Serial* port) {
    last_port_ = port;
    char message_type = port->getc();

    #ifdef DEBUG
//        t_test.stop();
//        t_test.reset();
//        t_test.start();
//        ms1 = t_test.read_ms();      
    #endif

    switch (message_type) {
    case '=':
        read_command(port);
        break;
    case '@':
        read_params(port);
        break;    
    default:
        send_answer('?');
        break;
    }
}

inline void Controller::read_command(Serial* port) {
    char temp = port->getc();
    make_command(temp);
}

inline void Controller::read_params(Serial* port) {
    char temp = port->getc();

    char buffer[STR_SIZE+1]; // +1 for \0 sym
    int size = 0;

    // if parameter is negative
    if (temp == '-') {
        buffer[0] = '-';
        size++;
        temp = port->getc();
    }

    while (temp != '*' && size < STR_SIZE) {
        buffer[size] = temp;
        temp = port->getc();
        size++;
    }

    buffer[size] = '\0';

    parse_params(buffer, size);
    send_answer('!');
    #ifdef STOP_RESPONSE
        send_answer('S');
    #endif
}

inline bool Controller::is_param_symbol(char symbol) {
    if (symbol == 'D' ||
        symbol == 'V' ||
        symbol == 'R' ||
        symbol == 'x' ||
        symbol == 'F' ||
        symbol == 'A' ||
        symbol == 'a' ||
        symbol == 'b' ||
        symbol == 'r') {

        return true;
    }

    return false;
}

inline void Controller::parse_params(const char* str_buffer, int size) {
    int i = 0;
    while(i < size) {
        char param_type = str_buffer[i];
        i += 1;
        if (!is_param_symbol(param_type)) {
            continue;
        }

        int one_param_size = 0;
        char one_param_buffer[STR_SIZE];

        while (i < size && !is_param_symbol(str_buffer[i])) {
            one_param_buffer[one_param_size] = str_buffer[i];
            i += 1;
            one_param_size += 1;
        }

        one_param_buffer[one_param_size] = '\0';
        make_params(param_type, one_param_buffer);
    }
    store_->calculate();
}


inline void Controller::make_command(char command) {
    switch (command) {
        case '0':
            stop_();
            break;
        case '1':
            start_();
            break;
        case '2':
            continues_();
            break;
        default:
            break;
    }

    if (INSTANT_RESPONSE) {
        send_answer('!');
    }
    
    #ifdef DEBUG
    send_store(); ////////////
    #endif
}


inline void Controller::make_params(char params_type, const char* str_buffer) {
    float value = (params_type != 'D') ? str_to_float(str_buffer) : 0.0;

    switch (params_type) {
        case 'D':
            store_->buffer.D = str_buffer[0];
            break;
        case 'V':
            store_->buffer.V = value;
            break;
        case 'R':
            store_->buffer.R = value;
            break;
        case 'x':
            store_->buffer.x = value;
            break;
        case 'F':
            store_->buffer.F = value;
            break;
        case 'A':
            store_->buffer.A = value;
            break;
        case 'a':
            store_->buffer.a = value;
            break;
        case 'b':
            store_->buffer.b = value;
            break;
        case 'r':
            store_->buffer.r = value;
            break;
        default:
            break;
    }
}

inline float Controller::str_to_float(const char* str_buffer) {
    return (float) atof(str_buffer);
}

void Controller::send_answer(char answer) {
    last_port_->putc(answer);
}

#ifdef DEBUG
void Controller::send_store() {
    usb_->printf("\r\nBuffer:\r\n");
    usb_->printf("\r\nD: %c\r\n", store_->buffer.D);
    usb_->printf("\r\nV: %f\r\n", store_->buffer.V);
    usb_->printf("\r\nF: %f\r\n", store_->buffer.F);
    usb_->printf("\r\nA: %f\r\n", store_->buffer.A);
    usb_->printf("\r\nR: %f\r\n", store_->buffer.R);
    usb_->printf("\r\nx: %f\r\n", store_->buffer.x);
    usb_->printf("\r\na: %f\r\n", store_->buffer.a);
    usb_->printf("\r\nb: %f\r\n", store_->buffer.b);
    usb_->printf("\r\nr: %f\r\n", store_->buffer.r);
    usb_->printf("\r\nDozator A:\r\n");
    usb_->printf("\r\nVolumeA: %ld\r\n", store_->data[0].volume);
    usb_->printf("\r\nReverseA: %ld\r\n", store_->data[0].reverse);
    usb_->printf("\r\nFeedrateA: %f\r\n", store_->data[0].feedrate);
    usb_->printf("\r\nAccelA: %f\r\n", store_->data[0].accel);
    usb_->printf("\r\nDozator B:\r\n");
    usb_->printf("\r\nVolumeB: %ld\r\n", store_->data[1].volume);
    usb_->printf("\r\nReverseB: %ld\r\n", store_->data[1].reverse);
    usb_->printf("\r\nFeedrateB: %f\r\n", store_->data[1].feedrate);
    usb_->printf("\r\nAccelB: %f\r\n", store_->data[1].accel);
}

void Controller::send_delay(long delay) {
    usb_->printf("\r\Delay: %ld\r\n", delay);
}
void Controller::send_times(long t1, long t2) {
    usb_->printf("\r\Times: %ld and %ld\r\n", t1, t2);
}
#endif
