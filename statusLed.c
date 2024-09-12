
#include <msp430.h>
#include "isr.h"
#include "gpio.h"
#include "board.h"
#include "statusLed.h"

unsigned int period_global = 1000;

void statusLedISR(void);

void statusLed_init(void){
    gpioMode(STATUS_LED,OUTPUT);
    send_to_isr(statusLedISR,period_global);

    gpioMode(PC_LED,OUTPUT);
    gpioWrite(PC_LED,LOW);
}

void statusLedISR(void){
    gpioToggle(STATUS_LED);
    send_to_isr(statusLedISR,period_global); // Change ISR period, only after cycle complete
}

void statusLed_setPeriod(unsigned int period)
{
    period_global = period;
}

void statusLed_setState(status value)
{
    switch (value)
    {
    case ERROR:
        statusLed_setPeriod(300); // 0.15s
        break;
    case NORMAL:
        statusLed_setPeriod(2000); // 1s
        break;
    }
}

void statusLed_PC(status value){
    switch (value)
        {
        case ERROR:
            gpioWrite(PC_LED,LOW);
            break;
        case NORMAL:
            gpioWrite(PC_LED,HIGH);
            break;
        }
}
