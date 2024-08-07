/*
 UART example for MSP4302553G2
 This example uses RX interrupts to receive one character and echoes
 the next one.
 E.g. If character 'A' is received then character 'B' is echoed back

*/
#include <msp430.h>

void uart_putchar(char c);
void uart_puts(const char *str);
#define LED BIT0  // LED conectado al pin P1.0

int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT

/* Use Calibration values for 1MHz Clock DCO*/
DCOCTL  = CALDCO_8MHZ;
BCSCTL1 = CALBC1_8MHZ;
BCSCTL2 = DIVS_3;

/* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
P1SEL = BIT1 | BIT2 ;
P1SEL2 = BIT1 | BIT2;

// Setting UART registers
UCA0CTL0  = 0;
UCA0CTL1 |= UCSSEL_2;                      // SMCLK => DCOCLK
UCA0BR0   = 104;                           // See baud rate divider (104 + 256*0 = 104)
UCA0BR1   = 0;
UCA0MCTL  = UCBRS0; //+ UCBRS1 + UCBRS2;               // Modulation UCBRSx = 6
UCA0CTL1 &= ~UCSWRST;                      // Initialize USCI state machine

/* Enable USCI_A0 RX interrupt */
IE2 |= UCA0RXIE;

P1DIR |= LED;              // Configurar el pin P1.0 como salida

__bis_SR_register(GIE);  // Habilitar interrupciones globales

while(1){
    uart_puts("hola como va \n");
    P1OUT ^= LED;          // Alternar el estado del LED
    __delay_cycles(10000);

}

}


// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
UCA0TXBUF = UCA0RXBUF + 1; // Echo next character i.e. RX character+1
}


void uart_putchar(char c) {
    while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;               // Envía el carácter
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putchar(*str++);
    }
}
