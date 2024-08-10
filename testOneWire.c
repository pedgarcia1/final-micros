/*
 UART example for MSP4302553G2
 This example uses RX interrupts to receive one character and echoes
 the next one.
 E.g. If character 'A' is received then character 'B' is echoed back

*/
#include <msp430.h>
#include "DS18B20.h"
#include <stdint.h>

void uart_putchar(char c);
void uart_puts(const char *str);

#define GREEN_LED 1,6

#define DCOCLK_FREQ_HZ          8000000UL // 8MHz
#define MCLK_PRESCALER_2POW     0
#define SMCLK_PRESCALER_2POW    0
#define MCLK_FREQ_HZ            (DCOCLK_FREQ_HZ/(1UL<<MCLK_PRESCALER_2POW)) // 8MHz
#define SMCLK_FREQ_HZ           (DCOCLK_FREQ_HZ/(1UL<<SMCLK_PRESCALER_2POW)) // 8MHz

#define HIGH_TEMP   40
#define LOW_TEMP    20
#define MAX_RANGE   8
#define MIN_RANGE   1




int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Set clock at 16 MHz
    DCOCTL  = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL2 = DIVS_3;

// Enable global interrupts
_BIS_SR(GIE);

temp_Init();
while (1)
{
    uint8_t RES = 0;
    float TEMP = 0;
    float TEMP2 = 0;
    uint8_t power = 0;

    RES = temp_Reset();

    if (RES)
    {

        temp_StartConversion(power);
        RES = temp_Reset();

        if (RES)
        {
            TEMP = temp_ReadTemperature();
            TEMP2 = TEMP;
        }
    }
}
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
}

// Definir interrupcion del timer para evitar que se cuelgue el uC
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void){

    /*  Count++;

    if(Count == Time){
        STATE = NORMAL;
        Count = 0;
        Time = (uint8_t)(Values[0]);
    }
    */

}

