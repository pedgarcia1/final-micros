#include "isrTimer.h"
#include "isr.h"

#include <msp430.h>

uint8_t timeoutFlag = 0;

void timerTimeoutStart(unsigned int period){
    timeoutFlag = 0;
    send_to_isr(timerTimeoutStop,period);
}

void timerTimeoutStop(void){
    timeoutFlag = 1;
    remove_from_isr(timerTimeoutStop);
}

uint8_t timerGetTimeout(void){
    return timeoutFlag;
}
