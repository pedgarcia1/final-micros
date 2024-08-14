#include <msp430g2553.h>
#include "74HC595.h"

#define LED BIT0

unsigned char value = 0;

void setup_timer() {
    // Configura el temporizador
    TACCR0 = 32767;            // Establece el valor de comparación para 500 ms (aproximadamente).
    TACTL = TASSEL_1 + MC_1;   // Usa ACLK (32.768 kHz) y modo Up.
    TACCTL0 = CCIE;            // Habilita la interrupción para CCR0.
}

void main() {
    WDTCTL = WDTPW + WDTHOLD;  // Detiene el temporizador watchdog.
    P1DIR |= LED;              // Configura el pin del LED como salida.

    __enable_interrupt();      // Habilita las interrupciones globales.
    setup_timer();             // Configura el temporizador.

    // Inicializa el controlador del registro de desplazamiento.
    shiftRegisterInit();

    while (1) {
        // Bucle infinito, la funcionalidad principal se maneja mediante interrupciones.
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A() {
    shiftRegisterWrite(value);  // Envía el valor actual al registro de desplazamiento.
    value++;           // Incrementa el valor.
    P1OUT ^= LED;      // Alterna el estado del LED para indicar actividad.

    if (value > 255) { // Resetea el valor si supera 255.
        value = 0;
    }
}
