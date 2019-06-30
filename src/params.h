#ifndef PARAMS_H
#define PARAMS_H

// #define TEST

// PINS 
#define STEP_A  PC_0
#define STEP_B  PC_2
#define DIR_A   PC_1
#define DIR_B   PC_3
#define EN_A    PB_0
#define EN_B    PA_4
#define UART_EN PC_12
#define UART_TX PC_10 
#define UART_RX PC_11 
#define UART2_TX PA_9
#define UART2_RX PA_10
#define USB_TX  PA_2
#define USB_RX  PA_3

#define PEDAL     USER_BUTTON
#define ALERT_A   PB_7
// #define ALERT_B   PB_11
#define ALERT_B   PB_6


// UART
#define UART_BAUDRATE 19200
#define STR_SIZE 20

// FLASH
#define FLASH_START_ADR  0x0800FC00
#define FLASH_PAGE_SIZES 1024
#define FLASH_FIELD_SIZE 10

// STEPPER
#define MOTOR_MAX_SPEED  500000
#define PULSE_WIDTH      8
#define INFINITE         100000000

#endif // PARAMS_H