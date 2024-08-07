#include "i2c_msp430.h"

// Include the necessary I2C library headers here

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    i2c_init();
    i2c_switchSlave(0x50);

    // while (1)
    // {
    //     i2c_writeData(0x00, 0x00);
    //     __delay_cycles(1e6); // Delay for 1 second
    // }
    
    return 0;
}
