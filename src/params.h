#ifndef PARAMS_H
#define PARAMS_H

// #define DEBUG

// PINS 
#define STEP_A  PC_0
#define DIR_A   PC_1

#define STEP_B  PC_2
#define DIR_B   PC_3

//#define STEP_B  PA_1
//#define DIR_B   PA_0

#define EN_A    PB_0
#define EN_B    PA_4

#define UART_TX PC_10 
#define UART_RX PC_11 
//#define UART2_TX PA_9
//#define UART2_RX PA_10

#define USB_TX  PA_2
#define USB_RX  PA_3

#define PEDAL_PIN PA_5

#define ALERT_A   PB_7
#define ALERT_B   PB_2


// UART
#define UART_BAUDRATE 19200
#define STR_SIZE 100


// STEPPER
#define MOTOR_MAX_SPEED  500000
#define PULSE_WIDTH      8
#define INFINITE         100000000

// BEHAVIOUR
#define REVERSE_MODE true
#define INSTANT_RESPONSE true
//#define STOP_RESPONSE

#endif // PARAMS_H