
#include "pwm.h"
#include <msp430.h>

void PWM_Init(void) {
    P2DIR |= PWM_PIN;  // pin 2.2
    P2SEL |= PWM_PIN;

    TA1CCR0 |= 100;
    TA1CCTL1 |= OUTMOD_7;
    TA1CCR1 = 0;
    TA1CTL |= TASSEL_2  | TIMER_UP | ID_0 | MC_1;
}

void PWM_setDC(uint8_t value) { // Value = [0 - 100] %
    duty_cycle = value - 1 ;
    TA1CCR1 = duty_cycle;
}


