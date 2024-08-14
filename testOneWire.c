/*
 One Wire test
 implemented with a DS18B20 temperature sensor
 and and watch dog timer

*/
#include <msp430g2553.h>
#include "DS18B20.h"
#include <stdint.h>
#include "GPIO_OW.h"

void uart_putchar(char c);
void uart_puts(const char *str);

#define GREEN_LED 1,6

#define HIGH_TEMP   40
#define LOW_TEMP    20
#define MAX_RANGE   8
#define MIN_RANGE   1

uint16_t count = 0;

int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    WDTCTL = WDT_MDLY_0_5; // Chequeado con arduino, interrupcion cada 8ms

    IE1 |= WDTIE; // Habilitar la interrupci�n del WDT

    PIN_OUTPUT(GREEN_LED);

    // Set clock at 8 MHz
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL2 = DIVS_3;

    // Enable global interrupts
    _BIS_SR(GIE);

    temp_Init();

    while (1)
    {
        uint8_t presence = 0;
        float TEMP = 0;
        float TEMP2 = 0;
        uint8_t power = 0; // 0 parasite power, 1 external power supply (Vdd)
        uint8_t power_save = 0;

        switch (temp_CheckState())
        {
        case STANDBY:
            presence = temp_Reset();
            if (presence)
            {
                temp_StartConversion(power); // Set t_state to CONVERTING_T
            }
            break;
        case CONVERSION_DONE:
            presence = temp_Reset();
            if (presence)
            {
                TEMP = temp_ReadTemperature(); // Set t_state to STANDBY
            }
            TEMP2 = TEMP - 1;
            break;
        default:
            break;
        }
    }
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
}

// ISR del Watchdog Timer
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    // 1 interrupcion de timer cada 0.5ms
    if (temp_CheckState() == CONVERTING_T)
    {
        count++;
        if (count >= 1000 * 2)
        {
            P1OUT ^= BIT6; // Alternar el LED en P1.6
            temp_SetState(CONVERSION_DONE);
            count = 0;
        }
    }
}
