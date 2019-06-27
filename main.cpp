#include "mbed.h"
#include "src/params.h"
#include "src/Controller.h"
#include "src/Dozator.h"
//#include "src/MultiDozator.h"
//#include <FlashIAP.h>

Timer t;
//FlashIAP flash;

Dozator dozator_A(STEP_A, DIR_A);
Dozator dozator_B(STEP_B, DIR_B);

int main() {
    t.start();
    Controller control;
    control.start_control(&dozator_A, &dozator_B);
    control.loop();
}

