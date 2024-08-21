
#include "pwm.h"
#include "isr.h"
#include <msp430.h>

void PMW_ISR(void);

void PWM_Init(void) {
    P2DIR |= BIT2;  // pin 2.2
    // gpioMode(PWM_PIN, OUTPUT);
    P2SEL |= BIT2; // pin 2.2
    // BITSET(P2SEL2, PIN2NUM(PWM_PIN));

    TA1CCR0 |= 100;
    TA1CCTL1 |= OUTMOD_7;
    TA1CCR1 |= 0;
    TA1CTL |= TASSEL_2  | TIMER_UP | ID_0 | MC_1;

}

void PWM_setDC(uint8_t value) { // Value = [0 - 100] %
    duty_cycle = value - 1 ;
    TA1CCR1 = duty_cycle;
}

void PMW_ISR(void){
}

