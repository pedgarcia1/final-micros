#include "timer.h"

#include <msp430.h>

void timerInitialization(uint16_t period)
{
    // Configure Timer_A0.
    TA0CTL = TASSEL_2  | TIMER_UP | TAIE | ID_3 ; // 1Mhz para el timer
    TA0CCTL0 = CCIE ;          // CCR0 interrupt enable


    TA0CCR0 = period; // Period.
    // TA0CCTL0 = 0;
}

void timerStop(void)
{
    TA0CTL = TASSEL_2; // Stop timer.
}

void timerStart(void)
{
    TA0CTL |=  MC_1; // Start timer.
}

void timerFlagClear(void)
{
    TACTL &= ~TAIFG;    // Clear interrupt flag for Timer A
}
