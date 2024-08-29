#include "system.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"
#include "isr.h"
#include "eeprom.h"
#include "GPIO_OW.h"

void AppInit(void);
void AppRun(void);

// I2C protocol
#define I2C_SCL     1,6
#define I2C_SDA     1,7

int otra = 0;

void main(void)
{
    // NO TOCAR
    systemInitFirst();
    boardInit(); // Empty
    AppInit();
    systemInitLast();

    for (;;)
        AppRun();
    // NO TOCAR
}


void AppInit(void)
{
    // Inicializaciones
   // EEPROM_init();

}

void AppRun(void)
{



            // Configure I2C module
            UCB0CTL1 |= UCSWRST;                    // Enable software reset
            UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Master mode, I2C mode, synchronous mode, 7-bit slave address
            UCB0CTL1  = UCSSEL_2 + UCSWRST;         // SMCLK source clock (8MHz)
            UCB0BR0 = 80;                           // Set clock divider for desired SCL frequency (100 kHz)
            UCB0BR1 = 0;
            // The 16-bit value of (UCBxBR0 + UCBxBR1 × 256) forms the prescaler value. (ahora esta puesto en 80)
            UCB0I2CSA = 0x50;
            P1SEL   |= BIT6 + BIT7;     // Select I2C function for these pins
            P1SEL2  |= BIT6 + BIT7;

            UCB0CTL1 &= ~UCSWRST;                   // Release software reset
            IE2 &= ~(UCB0TXIE | UCB0RXIE);             // Enable transmit and receive interrupts
            IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);

            // Reset PINS for starup
                if (UCB0STAT & UCBBUSY){
                    PIN_SEL(I2C_SCL,  LOW);
                    PIN_SEL(I2C_SDA,  LOW);
                    PIN_SEL2(I2C_SCL, LOW);
                    PIN_SEL2(I2C_SDA, LOW);

                    DIGITAL_WRITE(I2C_SCL, LOW);
                    DIGITAL_WRITE(I2C_SDA, LOW);

                    PIN_OUTPUT(I2C_SCL);
                    PIN_OUTPUT(I2C_SDA);

                    PIN_SEL(I2C_SCL,  HIGH);
                    PIN_SEL(I2C_SDA,  HIGH);
                    PIN_SEL2(I2C_SCL, HIGH);
                    PIN_SEL2(I2C_SDA, HIGH);
                }

            // Transmit mode
            UCB0CTL1 |= UCTR;


                // Send start
           UCB0CTL1 |= UCTXSTT;    // I2C start condition
           UCB0TXBUF = 0x1;
           while (UCB0CTL1 & UCTXSTT);

           UCB0TXBUF = 0x1;
           while(!(IFG2 & UCB0TXIFG));

           UCB0TXBUF = 0x4;
           while(!(IFG2 & UCB0TXIFG));


           UCB0TXBUF = 0xFA;
           while(!(IFG2 & UCB0TXIFG));


           UCB0CTL1 |= UCTXSTP;
           while(UCB0CTL1 & UCTXSTP);

           __delay_cycles(8000000);

           UCB0CTL1 |= UCTR;
           UCB0CTL1 |= UCTXSTT;
           UCB0TXBUF = 0x1;
           while (UCB0CTL1 & UCTXSTT);


           //UCB0TXBUF = 0x1;
         while(!(IFG2 & UCB0TXIFG));

         UCB0TXBUF = 0x4;
         while(!(IFG2 & UCB0TXIFG));




         UCB0CTL1 &= ~UCTR;

         UCB0CTL1 |= UCTXSTT;    // I2C start condition

         while (UCB0CTL1 & UCTXSTT);

         UCB0CTL1 |= UCTXSTP;

         uint8_t leido, debug;
         while(!(IFG2 & UCB0RXIFG));
         leido = UCB0RXBUF;
         debug = leido-1;
         otra = leido;

         while(UCB0CTL1 & UCTXSTP);
    /*
     *
    uint8_t dato = 0x55;

    EEPROM_writeData(100, &dato, 1);

    uint8_t dato_leido;

    EEPROM_readData(100, &dato_leido, 1);

    __delay_cycles(1000000);
    */
}
